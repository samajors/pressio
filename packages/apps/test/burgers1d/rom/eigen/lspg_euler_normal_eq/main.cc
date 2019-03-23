
#include "CORE_ALL"
#include "ODE_ALL"
#include "SOLVERS_NONLINEAR"
#include "ROM_LSPG"
#include "APPS_BURGERS1D"
#include "../lspg_utils.hpp"
#include "../../../fom/fom_gold_states.hpp"

int main(int argc, char *argv[]){
  using fom_t		= rompp::apps::Burgers1dEigen;
  using scalar_t	= typename fom_t::scalar_type;

  using eig_dyn_vec	= Eigen::Matrix<scalar_t, -1, 1>;
  using lspg_state_t	= rompp::core::Vector<eig_dyn_vec>;

  using eig_dyn_mat	= Eigen::Matrix<scalar_t, -1, -1>;
  using decoder_jac_t	= rompp::core::MultiVector<eig_dyn_mat>;
  using decoder_t	= rompp::rom::LinearDecoder<decoder_jac_t>;

  //-------------------------------
  // app object
  constexpr int numCell = 20;
  Eigen::Vector3d mu(5.0, 0.02, 0.02);
  fom_t appobj( mu, numCell);
  appobj.setup();
  auto t0 = static_cast<scalar_t>(0);
  scalar_t dt = 0.01;

  // read from file the jacobian of the decoder
  constexpr int romSize = 11;
  // store modes computed before from file
  decoder_jac_t phi =
    rompp::apps::test::eigen::readBasis("basis.txt", romSize, numCell);
  const int numBasis = phi.numVectors();
  assert( numBasis == romSize );

  // this is my reference state
  auto & y0n = appobj.getInitialState();
  decoder_t decoderObj(phi);

  // define ROM state
  lspg_state_t yROM(romSize);
  // initialize to zero (this has to be done)
  yROM.putScalar(0.0);

  // define LSPG type
  using lspg_problem_types = rompp::rom::DefaultLSPGTypeGenerator<
    fom_t, rompp::ode::ImplicitEnum::Euler, decoder_t, lspg_state_t>;
  rompp::rom::LSPGStepperObjectGenerator<lspg_problem_types> lspgGener(
      appobj, y0n, decoderObj, yROM, t0);

  using rom_stepper_t = typename lspg_problem_types::rom_stepper_t;

  // GaussNewton solver
  // hessian comes up in GN solver, it is (J phi)^T (J phi)
  // rom is solved using eigen, hessian is wrapper of eigen matrix
  using eig_dyn_mat	 = Eigen::Matrix<scalar_t, -1, -1>;
  using hessian_t	 = rompp::core::Matrix<eig_dyn_mat>;
  using solver_tag	 = rompp::solvers::linear::LSCG;
  using converged_when_t = rompp::solvers::iterative::default_convergence;
  using gnsolver_t	 = rompp::solvers::iterative::GaussNewton<
    scalar_t, solver_tag, rompp::solvers::EigenIterative,
    converged_when_t, rom_stepper_t, hessian_t>;
  gnsolver_t solver(lspgGener.stepperObj_, yROM);
  solver.setTolerance(1e-13);
  solver.setMaxIterations(200);

  // integrate in time
  rompp::ode::integrateNSteps(lspgGener.stepperObj_, yROM, 0.0, dt, 10, solver);

  // compute the fom corresponding to our rom final state
  using fom_state_w_t = typename lspg_problem_types::fom_state_w_t;
  fom_state_w_t yRf(y0n);
  decoderObj.applyMapping(yROM, yRf);
  yRf += lspgGener.y0Fom_;
  std::cout << *yRf.data() << std::endl;

  // this is a reproducing ROM test, so the final reconstructed state
  // has to match the FOM solution obtained with euler, same time-step, for 10 steps
  const auto trueY = rompp::apps::test::Burg1DtrueImpEulerN20t010;
  for (auto i=0; i<yRf.size(); i++)
    assert(std::abs(yRf[i] - trueY[i]) < 1e-10 );

  return 0;
}