
#include <gtest/gtest.h>
#include "CORE_ALL"
#include "SOLVERS_BASIC"

struct ValidSystemA {
  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = rompp::core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = rompp::core::Vector<vector_n_t>;

  using state_type = vector_w_t;
  using residual_type = state_type;
  using jacobian_type = matrix_w_t;

  void residual(const state_type & x, residual_type & res) {
    res[0] =  x[0]*x[0]*x[0] + x[1] - 1.0;
    res[1] = -x[0] + x[1]*x[1]*x[1] + 1.0;
  }

  residual_type residual(const state_type & x) {
    residual_type  res(2);
    res[0] =  x[0]*x[0]*x[0] + x[1] - 1.0;
    res[1] = -x[0] + x[1]*x[1]*x[1] + 1.0;
    return res;
  }

  void jacobian(const state_type & x, jacobian_type & jac) {
    jac.data()->coeffRef(0, 0) = 3.0*x[0]*x[0];
    jac.data()->coeffRef(0, 1) =  1.0;
    jac.data()->coeffRef(1, 0) = -1.0;
    jac.data()->coeffRef(1, 1) = 3.0*x[1]*x[1];
  }

  jacobian_type jacobian(const state_type & x) {
    jacobian_type jac(2, 2);
    jac.data()->coeffRef(0, 0) = 3.0*x[0]*x[0];
    jac.data()->coeffRef(0, 1) =  1.0;
    jac.data()->coeffRef(1, 0) = -1.0;
    jac.data()->coeffRef(1, 1) = 3.0*x[1]*x[1];
    return jac;
  }
};

TEST(system_traits, systemTraitsValidSystemATest)
{
  using namespace rompp;
  using namespace solvers::details;

  typedef system_traits<ValidSystemA> system_t;

  auto is_system = system_t::is_system;

  auto has_residual_callable_with_one_arg = system_t::has_residual_callable_with_one_arg;
  auto has_residual_callable_with_two_args = system_t::has_residual_callable_with_two_args;

  auto has_jacobian_callable_with_one_arg = system_t::has_jacobian_callable_with_one_arg;
  auto has_jacobian_callable_with_two_args = system_t::has_jacobian_callable_with_two_args;

  EXPECT_EQ(is_system, true);
  EXPECT_EQ(has_residual_callable_with_one_arg, true);
  EXPECT_EQ(has_residual_callable_with_two_args, true);
  EXPECT_EQ(has_jacobian_callable_with_one_arg, true);
  EXPECT_EQ(has_jacobian_callable_with_two_args, true);
}




struct ValidSystemB {
  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = rompp::core::Matrix<matrix_n_t>;
  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = rompp::core::Vector<vector_n_t>;

  using state_type = vector_w_t;
  using residual_type = state_type;
  using jacobian_type = matrix_w_t;

  void residual(const vector_w_t& x, vector_w_t& res) {
    res[0] =  x[0]*x[0]*x[0] + x[1] - 1.0;
    res[1] = -x[0] + x[1]*x[1]*x[1] + 1.0;
  }

  void jacobian(const vector_w_t& x, matrix_w_t& jac) {
    jac.data()->coeffRef(0, 0) = 3.0*x[0]*x[0];
    jac.data()->coeffRef(0, 1) =  1.0;
    jac.data()->coeffRef(1, 0) = -1.0;
    jac.data()->coeffRef(1, 1) = 3.0*x[1]*x[1];
  }
};

TEST(system_traits, systemTraitsValidSystemBTest)
{
  using namespace rompp;
  using namespace solvers::details;

  typedef system_traits<ValidSystemB> system_t;

  auto is_system = system_t::is_system;

  auto has_residual_callable_with_one_arg = system_t::has_residual_callable_with_one_arg;
  auto has_residual_callable_with_two_args = system_t::has_residual_callable_with_two_args;

  auto has_jacobian_callable_with_one_arg = system_t::has_jacobian_callable_with_one_arg;
  auto has_jacobian_callable_with_two_args = system_t::has_jacobian_callable_with_two_args;

  EXPECT_EQ(is_system, true);
  EXPECT_EQ(has_residual_callable_with_one_arg, false);
  EXPECT_EQ(has_residual_callable_with_two_args, true);
  EXPECT_EQ(has_jacobian_callable_with_one_arg, false);
  EXPECT_EQ(has_jacobian_callable_with_two_args, true);
}




struct InvalidSystemA {
  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = rompp::core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = rompp::core::Vector<vector_n_t>;

  using state_type = vector_w_t;
  using residual_type = state_type;
  using jacobian_type = matrix_w_t;

  void jacobian(const state_type& x, jacobian_type& jac) {
    jac.data()->coeffRef(0, 0) = 3.0*x[0]*x[0];
    jac.data()->coeffRef(0, 1) =  1.0;
    jac.data()->coeffRef(1, 0) = -1.0;
    jac.data()->coeffRef(1, 1) = 3.0*x[1]*x[1];
  }
};

TEST(system_traits, systemTraitsInvalidSystemATest)
{
  using namespace rompp;
  using namespace solvers::details;

  typedef system_traits<InvalidSystemA> system_t;

  auto is_system = system_t::is_system;

  auto has_residual_callable_with_one_arg = system_t::has_residual_callable_with_one_arg;
  auto has_residual_callable_with_two_args = system_t::has_residual_callable_with_two_args;

  auto has_jacobian_callable_with_one_arg = system_t::has_jacobian_callable_with_one_arg;
  auto has_jacobian_callable_with_two_args = system_t::has_jacobian_callable_with_two_args;

  EXPECT_EQ(is_system, false);
  EXPECT_EQ(has_residual_callable_with_one_arg, false);
  EXPECT_EQ(has_residual_callable_with_two_args, false);
  EXPECT_EQ(has_jacobian_callable_with_one_arg, false);
  EXPECT_EQ(has_jacobian_callable_with_two_args, true);
}
