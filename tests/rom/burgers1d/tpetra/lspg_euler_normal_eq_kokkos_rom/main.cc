
#include "pressio_rom.hpp"
#include "pressio_apps.hpp"
#include "utils_tpetra.hpp"

int main(int argc, char *argv[]){
  using fom_t		= pressio::apps::Burgers1dTpetra;
  using scalar_t	= typename fom_t::scalar_type;
  using native_state_t  = typename fom_t::state_type;
  using fom_state_t  = pressio::containers::Vector<native_state_t>;

  using tcomm_t		= Teuchos::MpiComm<int>;
  using rcpcomm_t	= Teuchos::RCP<const tcomm_t>;

  // get the native dense matrix
  using native_dmat_t   = typename fom_t::dense_matrix_type;

  // get the native state type
  using native_state_t  = typename fom_t::state_type;

  // we want the exe space for tpetra
  using exe_space = typename native_state_t::execution_space;

  // the lspg state is a wrapper of Kokkos vector
  using k1dLl_d = Kokkos::View<scalar_t*, exe_space>;
  using lspg_state_t	= pressio::containers::Vector<k1dLl_d>;

  // linear decoder
  using decoder_jac_t	= pressio::containers::MultiVector<native_dmat_t>;
  using decoder_t	= pressio::rom::LinearDecoder<decoder_jac_t, fom_state_t, fom_state_t>;

  std::string checkStr {"PASSED"};

  // scope guard needed for tpetra
  Tpetra::ScopeGuard tpetraScope (&argc, &argv);
  {
    int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    rcpcomm_t Comm = Teuchos::rcp (new tcomm_t(MPI_COMM_WORLD));

    // app object
    constexpr int numCell = 20;
    fom_t appobj( {5.0, 0.02, 0.02}, numCell, Comm);
    auto t0 = static_cast<scalar_t>(0);
    scalar_t dt = 0.01;

    // read from file the jacobian of the decoder
    constexpr int romSize = 11;
    // store modes computed before from file
    decoder_jac_t phi =
      pressio::rom::test::tpetra::readBasis("basis.txt", romSize, numCell,
    					   Comm, appobj.getDataMap());
    const int numBasis = phi.numVectors();
    if( numBasis != romSize ) return 0;
    // create decoder obj
    decoder_t decoderObj(phi);

    // for this problem, my reference state = initial state
    auto & yRef = appobj.getInitialState();

    // define ROM state (defaulted to zero, which is what we want)
    lspg_state_t yROM("romState", romSize);

    // define LSPG type
    using ode_tag = pressio::ode::implicitmethods::Euler;
    using lspg_problem = pressio::rom::LSPGUnsteadyProblem<
      pressio::rom::DefaultLSPGUnsteady, ode_tag, fom_t, lspg_state_t, decoder_t>;
    using lspg_stepper_t = typename lspg_problem::lspg_stepper_t;
    lspg_problem lspgProblem(appobj, yRef, decoderObj, yROM, t0);

    // linear solver for hessian
    // hessian comes up in GN solver, it is (J phi)^T (J phi)
    // use leftlayout so we access blas/lapack easily
    using kll = Kokkos::LayoutLeft;
    using k2dLl_d = Kokkos::View<scalar_t**, kll, exe_space>;
    using hessian_t  = pressio::containers::Matrix<k2dLl_d>;
    using solver_tag   = pressio::solvers::linear::direct::getrs;
    using linear_solver_t = pressio::solvers::direct::KokkosDirect<solver_tag, hessian_t>;
    linear_solver_t linSolverObj;

    // GaussNewton solver
    using gnsolver_t = pressio::solvers::iterative::GaussNewton<lspg_stepper_t, linear_solver_t>;
    gnsolver_t solver(lspgProblem.getStepperRef(), yROM, linSolverObj);
    solver.setTolerance(1e-13);
    solver.setMaxIterations(4);

    // integrate in time
    pressio::ode::integrateNSteps(lspgProblem.getStepperRef(), yROM, 0.0, dt, 10, solver);

    // compute the fom corresponding to our rom final state
    auto yFomFinal = lspgProblem.getFomStateReconstructorCRef()(yROM);
    auto yFF_v = yFomFinal.data()->getData();

    // this is a reproducing ROM test, so the final reconstructed state
    // has to match the FOM solution obtained with euler, same time-step, for 10 steps
    int shift = (rank==0) ? 0 : 10;
    const int myn = yFomFinal.data()->getMap()->getNodeNumElements();
    const auto trueY = pressio::apps::test::Burgers1dImpGoldStatesBDF1::get(numCell, dt, 0.10);
    for (auto i=0; i<myn; i++)
      if (std::abs(yFF_v[i] - trueY[i+shift]) > 1e-10) checkStr = "FAILED";

  }//tpetra scope

  std::cout << checkStr <<  std::endl;
  return 0;
}
