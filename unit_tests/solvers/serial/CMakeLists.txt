

add_serial_utest(solvers_linear_dense_eigen 
  solvers_linear_dense_eigen.cc)

add_serial_utest(solvers_linear_sparse_eigen
  solvers_linear_sparse_eigen.cc)

add_serial_utest(solvers_newton_raphson_eigen
  solvers_newton_raphson_eigen.cc)

add_subdirectory(non_linear_least_squares)
