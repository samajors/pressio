
#include <iostream>
#include "CORE_MATRIX"
#include "SOLVERS_NONLINEAR"

struct NonLinearSystem {

  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_type = rompp::core::Matrix<matrix_n_t>;
  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_type = rompp::core::Vector<vector_n_t>;
 
  auto residual(const vector_type& x) {
    vector_type res(2);
    residual(x, res);
    return res;
  }

  auto jacobian(const vector_type& x) {
    matrix_type jac(2, 2);
    jacobian(x, jac);
    return jac;
  }

  void residual(const vector_type& x, vector_type & R){
    R[0] =  x[0]*x[0]*x[0] + x[1] - 1.0;
    R[1] = -x[0] + x[1]*x[1]*x[1] + 1.0;
  }

  void jacobian(const vector_type& x, matrix_type & jac){
    jac.data()->coeffRef(0, 0) = 3.0*x[0]*x[0];
    jac.data()->coeffRef(0, 1) =  1.0;
    jac.data()->coeffRef(1, 0) = -1.0;
    jac.data()->coeffRef(1, 1) = 3.0*x[1]*x[1];
  }
};


int main() {

  // Namespaces
  using namespace rompp;
  using namespace rompp::solvers;

  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = core::Vector<vector_n_t>;

  auto solver = NonLinearSolvers::createIterativeSolver<
    nonlinear::NewtonRaphson, linear::Bicgstab>();

  vector_w_t x0(2);
  x0[0] = 0.4;
  x0[1] = 0.5;

  NonLinearSystem sys;
  auto x = solver.solve(sys, x0);

  std::cout << "The solution of the nonlinear system is: "
	    << std::endl << *x.data() << std::endl;

  return 0;
}
