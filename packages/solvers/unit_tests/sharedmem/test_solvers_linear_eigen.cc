
#include <gtest/gtest.h>

#include "experimental/solvers_linear_factory.hpp"
#include "CORE_MATRIX"
// #include "CORE_VECTOR"
// #include "matrix/concrete/core_matrix_dense_serial_eigen.hpp"
// #include "matrix/concrete/core_matrix_sparse_serial_eigen.hpp"
// #include "vector/concrete/core_vector_serial_eigen.hpp"

TEST(solvers_linear_iterative_eigen, solversTestLinearIterativeEigenCreateWithMatrixAndSolve)
{
  // Namespaces
  using namespace solvers;

  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = core::Vector<vector_n_t>;

  // Define linear system
  vector_w_t b(2);
  (*b.data()) << 7.0, 1.0;

  matrix_w_t A(2, 2);
  A.data()->insert(0, 0) +=  3.0;
  A.data()->insert(0, 1) += -1.0;
  A.data()->insert(1, 0) +=  2.0;
  A.data()->insert(1, 1) +=  3.0;

  // Solve linear system
  auto solver = LinearSolvers::createIterativeSolver<linear::Bicgstab>(A);
  auto x = solver.solve(b);

  // Expectations
  EXPECT_NEAR( x[0],  2.0, 1e-14 );
  EXPECT_NEAR( x[1], -1.0, 1e-14 );
}


TEST(solvers_linear_iterative_eigen, solversTestLinearIterativeEigenCreateWithoutMatrixAndSolve)
{
  // Namespaces
  using namespace solvers;

  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = core::Vector<vector_n_t>;

  // Define linear system
  vector_w_t b(2);
  (*b.data()) << 7.0, 1.0;

  matrix_w_t A(2, 2);
  A.data()->insert(0, 0) +=  3.0;
  A.data()->insert(0, 1) += -1.0;
  A.data()->insert(1, 0) +=  2.0;
  A.data()->insert(1, 1) +=  3.0;

  // Solve linear system
  auto solver = LinearSolvers::createIterativeSolver<linear::Bicgstab, matrix_w_t>();
  auto x = solver.solve(A, b);
   
  // Expectations
  EXPECT_NEAR( x[0],  2.0, 1e-14 );
  EXPECT_NEAR( x[1], -1.0, 1e-14 );
}


/*
TEST(solvers_linear_iterative_eigen, solversTestLinearIterativeEigenNoReturnSameType)
{
  // Namespaces
  using namespace solvers;

  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vector_n_t = Eigen::VectorXd;
  using vector_w_t = core::Vector<vector_n_t>;

  // Define linear system
  vector_w_t b(2);
  vector_w_t x(2);
  (*b.data()) << 0.0, 2.0;

  matrix_w_t A(2, 2);
  A.data()->coeffRef(0, 0) =  1;
  A.data()->coeffRef(0, 1) =  1;
  A.data()->coeffRef(1, 0) =  1;
  A.data()->coeffRef(1, 1) = -1;

  // Solve linear system
  auto solver = LinearIterativeSolvers::createSolver<linear::CG>(A);
  solver.solve(b, x);

  // Expectations
  EXPECT_NEAR( x[0],  1.0, 1e-14 );
  EXPECT_NEAR( x[1], -1.0, 1e-14 );
}


TEST(solvers_linear_iterative_factory, solversTestLinearIterativeEigenResetSystemSameType)
{
  // Namespaces
  using namespace solvers;

  // Matrix typedefs
  using matrix_n_t = Eigen::SparseMatrix<double>;
  using matrix_w_t = core::Matrix<matrix_n_t>;

  // Vector typedefs
  using vectord_n_t = Eigen::VectorXd;
  using vectord_w_t = core::Vector<vectord_n_t>;


  // Define linear system
  vectord_w_t b(2);
  (*b.data()) << 0.0, 2.0;

  matrix_w_t A0(2, 2);
  matrix_w_t A1(2, 2);
  A0.data()->coeffRef(0, 0) =  1;
  A0.data()->coeffRef(0, 1) =  1;
  A0.data()->coeffRef(1, 0) =  1;
  A0.data()->coeffRef(1, 1) = -3;

  A1.data()->coeffRef(0, 0) =  1;
  A1.data()->coeffRef(0, 1) =  1;
  A1.data()->coeffRef(1, 0) =  1;
  A1.data()->coeffRef(1, 1) = -1;

  // Solve linear system
  auto solver = LinearIterativeSolvers::createSolver<linear::CG>(A0);
  solver.resetLinearSystem(A1);
  auto x = solver.solve(b);

  // Expectations
  EXPECT_NEAR( x[0],  1.0, 1e-14 );
  EXPECT_NEAR( x[1], -1.0, 1e-14 );
}


#if 0

Old tests: to be removed in the future

#include <gtest/gtest.h>
#include "experimental/solvers_linear_eigen.hpp"
#include "matrix/concrete/core_matrix_dense_serial_eigen.hpp"
#include "matrix/concrete/core_matrix_sparse_serial_eigen.hpp"
#include "vector/concrete/core_vector_serial_eigen.hpp"

TEST(solvers_linear, simpleTest)
{
  // vector
  using native_v_t = Eigen::Matrix<double, 3, 1>;
  core::Vector<native_v_t> b;
  (*b.data()) << 3, 3, 4;

  // matrix
  using native_m_t = Eigen::Matrix<double, 3, 3>;
  core::Matrix<native_m_t> A;
  (*A.data()) <<  1,2,3,  4,5,6,  7,8,10;

  // solution
  core::Vector<native_v_t> x;

  // std::cout << *A.data() << std::endl;
  // std::cout << *b.data() << std::endl;
  using namespace solvers::experimental;
  linearSolver<decltype(A),decltype(b),decltype(x)> ls;
  ls.solve(A,b,x);
  //std::cout << *x.data() << std::endl;
    
  EXPECT_NEAR( x[0], -2.0, 1e-14 );
  EXPECT_NEAR( x[1], 1.0, 1e-14 );
  EXPECT_NEAR( x[2], 1.0, 1e-14 );  
}


TEST(solvers_linear, simpleTestSparse)
{
  int Nmesh = 150;

  using sc_t = double;
  sc_t PI = 3.141592653589793238462643383279502;
  sc_t dx = 1.0/(static_cast<sc_t>(Nmesh-1));
  sc_t dx2 = dx*dx;
  sc_t c = (sc_t)4*PI*PI;
  int N = Nmesh-2;

  sc_t uLeft = sin(2*PI*0.0);
  sc_t uRight = sin(2*PI*0.0);
  
  // vector
  using native_v_t = Eigen::Matrix<sc_t, Eigen::Dynamic, 1>;
  core::Vector<native_v_t> b(N);
  b[0] = -c*sin(2*PI*dx)*dx2 - uLeft;
  for (int i=1; i<=N-2; i++){
    sc_t x = dx + i*dx;
    b[i] = -c*sin(2*PI*x)*dx2;
  }
  b[N-1] = -c*dx2*sin(2*PI*(1.-dx)) - uRight;
  
  // matrix
  using native_m_t = Eigen::SparseMatrix<sc_t,Eigen::RowMajor,int>;
  core::Matrix<native_m_t> A(N,N);
  using veci = std::vector<int>;
  using vecd = std::vector<sc_t>;
  veci indices; vecd vals;
  int i=0;
  indices = veci({i,i+1});
  vals = vecd({-2.,1.});
  A.insertValues(i,(int)vals.size(),vals.data(),indices.data());
  //
  for (i=1; i<=N-2; i++){
    indices = veci({i-1,i,i+1});
    vals = vecd({1.,-2.,1.});
    A.insertValues(i,(int)vals.size(),vals.data(),indices.data());
  }
  //
  i=N-1;
  indices = veci({i-1,i});
  vals = vecd({1.,-2.});
  A.insertValues(i,(int)vals.size(),vals.data(),indices.data());
  //  std::cout << *A.data() << std::endl;

  using namespace solvers::experimental;
  
  // solution
  constexpr sc_t zero = static_cast<sc_t>(0);
  // constexpr sc_t one = static_cast<sc_t>(1);
  core::Vector<native_v_t> x(N);
  for (i=0; i<N; i++)
    x[i] = zero;

  // solve
  linearSolver<decltype(A),decltype(b),decltype(x)> ls;
  ls.solve(A,b,x);

  sc_t error = 0.0;
  for (i=0; i<N; i++){
    sc_t xpt = dx + i*dx;
    sc_t truex = sin(2*PI*xpt);
    sc_t tmp =std::abs(x[i]-truex); 
    error += tmp*tmp;
  }
  error = std::sqrt(error/N);
  //std::cout << error << std::endl;
  EXPECT_LT(error, 0.0005 );
}

#endif
*/