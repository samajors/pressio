
#include <gtest/gtest.h>
#include "pressio_solvers.hpp"
#include "./problems/solvers_utest_serial_expon_data_fit_n2.hpp"

TEST(solvers_nonlinear_least_squares,
     gn_normal_eq_lscg_exp_data_fit_n2){

  using namespace pressio;

  using problem_t   = solvers::test::ExpDataFitN2;
  using state_t	    = typename problem_t::state_type;
  problem_t problem;
  state_t x(2); x[0] = 2.0; x[1] = 0.25;

  using hessian_t = containers::Matrix<Eigen::MatrixXd>;

  // linear solver type
  using solver_tag	= solvers::linear::iterative::LSCG;
  using linear_solver_t = solvers::iterative::EigenIterative<solver_tag, hessian_t>;
  linear_solver_t linSolver;

  // GaussNewton solver
  using gn_t = solvers::iterative::GaussNewton<linear_solver_t, problem_t, hessian_t>;
  gn_t GNSolver(problem, x, linSolver);
  GNSolver.setTolerance(1e-8);
  GNSolver.solve(problem, x);

  std::cout << std::setprecision(14) << *x.data() << std::endl;
  EXPECT_NEAR( x(0), 2.4173449278229, 1e-7 );
  EXPECT_NEAR( x(1), 0.26464986197941, 1e-7 );
}


TEST(solvers_nonlinear_least_squares,
     gn_normal_eq_pass_sys_type_lscg_exp_data_fit_n2){

  using namespace pressio;

  using problem_t   = solvers::test::ExpDataFitN2;
  using vec_t	  = typename problem_t::state_type;
  using mat_t	  = typename problem_t::jacobian_type;
  using state_t	  = vec_t;
  using hessian_t = mat_t;

  problem_t problem;
  state_t x(2); x[0] = 2.0; x[1] = 0.25;

  // linear solver type
  using solver_tag	= solvers::linear::iterative::LSCG;
  using linear_solver_t = solvers::iterative::EigenIterative<solver_tag, hessian_t>;
  linear_solver_t linSolver;

  // GaussNewton solver
  using converged_when_t = solvers::iterative::default_convergence;
  using gn_t = solvers::iterative::GaussNewton
    <linear_solver_t, converged_when_t, problem_t, hessian_t>;
  gn_t GNSolver(problem, x, linSolver);
  GNSolver.setTolerance(1e-8);
  GNSolver.solve(problem, x);

  std::cout << std::setprecision(14) << *x.data() << std::endl;
  EXPECT_NEAR( x(0), 2.4173449278229, 1e-7 );
  EXPECT_NEAR( x(1), 0.26464986197941, 1e-7 );
}


TEST(solvers_nonlinear_least_squares,
     gn_normal_eq_pass_sys_type_abs_proj_res_lscg_exp_data_fit_n2){

  using namespace pressio;

  using problem_t   = solvers::test::ExpDataFitN2;
  using vec_t	  = typename problem_t::state_type;
  using mat_t	  = typename problem_t::jacobian_type;
  using state_t	  = vec_t;
  using hessian_t = mat_t;

  problem_t problem;
  state_t x(2); x[0] = 2.0; x[1] = 0.25;

  // linear solver type
  using solver_tag	= solvers::linear::iterative::LSCG;
  using linear_solver_t = solvers::iterative::EigenIterative<solver_tag, hessian_t>;
  linear_solver_t linSolver;

  // GaussNewton solver
  using converged_when_t = solvers::iterative::converged_when::absoluteNormGradientBelowTol;
  using gn_t = solvers::iterative::GaussNewton
    <linear_solver_t, converged_when_t, problem_t, hessian_t>;
  gn_t GNSolver(problem, x, linSolver);
  // 1e3 is chosen to test the convergence condition
  GNSolver.setTolerance(1e3);
  GNSolver.solve(problem, x);

  std::cout << std::setprecision(14) << *x.data() << std::endl;
  EXPECT_NEAR( x(0), 2.41536166777134, 1e-11);
  EXPECT_NEAR( x(1), 0.26482938025711, 1e-11);
}


TEST(solvers_nonlinear_least_squares,
     gn_normal_eq_pass_sys_type_rel_proj_res_lscg_exp_data_fit_n2){

  using namespace pressio;

  using problem_t   = solvers::test::ExpDataFitN2;
  using vec_t	  = typename problem_t::state_type;
  using mat_t	  = typename problem_t::jacobian_type;
  using state_t	  = vec_t;
  using hessian_t = mat_t;

  problem_t problem;
  state_t x(2); x[0] = 2.0; x[1] = 0.25;

  // linear solver type
  using solver_tag	= solvers::linear::iterative::LSCG;
  using linear_solver_t = solvers::iterative::EigenIterative<solver_tag, hessian_t>;
  linear_solver_t linSolver;

  // GaussNewton solver
  using converged_when_t = solvers::iterative::converged_when::relativeNormGradientBelowTol;
  using gn_t = solvers::iterative::GaussNewton
    <linear_solver_t, converged_when_t, problem_t, hessian_t>;
  gn_t GNSolver(problem, x, linSolver);
  GNSolver.setTolerance(0.1);
  GNSolver.solve(problem, x);

  std::cout << std::setprecision(14) << *x.data() << std::endl;
  EXPECT_NEAR( x(0), 2.41728158794844, 1e-11);
  EXPECT_NEAR( x(1), 0.26465375115797, 1e-11);
}
