
#include "pressio_ode.hpp"
#include "pressio_apps.hpp"

constexpr double eps = 1e-12;
std::string checkStr {"PASSED"};

template <typename T>
void checkSol(const T & y, const std::vector<double> & trueS){
  for (size_t i=0; i< trueS.size(); i++)
    if (std::abs(y[i] - trueS[i]) > eps) checkStr = "FAILED";
}

int main(int argc, char *argv[]){
  using app_t		= pressio::apps::Burgers1dEigen;
  using scalar_t	= typename app_t::scalar_type;
  using app_state_t	= typename app_t::state_type;
  using app_rhs_t	= typename app_t::velocity_type;
  using app_jacob_t	= typename app_t::jacobian_type;

  //-------------------------------
  // create app object
  Eigen::Vector3d mu(5.0, 0.02, 0.02);
  constexpr int Ncell = 20;
  app_t appObj(mu, Ncell);
  auto & y0n = appObj.getInitialState();
  auto r0n = appObj.velocity(y0n, static_cast<scalar_t>(0));

  // types for ode
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  using ode_res_t   = pressio::containers::Vector<app_rhs_t>;
  using ode_jac_t   = pressio::containers::Matrix<app_jacob_t>;

  ode_state_t y(y0n);
  using ode_tag = pressio::ode::implicitmethods::Euler;
  using stepper_t = pressio::ode::ImplicitStepper<
    ode_tag, ode_state_t, ode_res_t, ode_jac_t, app_t>;
  stepper_t stepperObj(y, appObj);

  // define solver
  using lin_solver_t = pressio::solvers::iterative::EigenIterative<
    pressio::solvers::linear::iterative::Bicgstab, ode_jac_t>;
  lin_solver_t linSolverObj;
  using nonlin_solver_t = pressio::solvers::NewtonRaphson<stepper_t, lin_solver_t, scalar_t>; 
  nonlin_solver_t solverO(stepperObj, y, linSolverObj);

  // integrate in time
  scalar_t fint = 35;
  scalar_t dt = 0.01;
  auto Nsteps = static_cast<::pressio::ode::types::step_t>(fint/dt);
  pressio::ode::integrateNSteps(stepperObj, y, 0.0, dt, Nsteps, solverO);
  std::cout << std::setprecision(14) << *y.data();
  {
    using namespace pressio::apps::test;
    checkSol(y, Burgers1dImpGoldStatesBDF1::get(Ncell, dt, fint));
  }

  std::cout << checkStr << std::endl;
  return 0;
}
