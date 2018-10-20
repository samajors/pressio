
#include <gtest/gtest.h>
#include "CORE_ALL"

using eigdmat_t = Eigen::MatrixXd;
using myMV_t = rompp::core::MultiVector<eigdmat_t>;

TEST(core_multi_vector_serial_eigen_dynamic_class,
     dot_WithEigenVector){

  //construct multivector
  myMV_t A(6,3);
  A(0,0) = 1.; A(0,1) = 2.; A(0,2) = 3.;
  A(1,0) = 3.; A(1,1) = 2.; A(1,2) = 1.;
  A(2,0) = 0.; A(2,1) = 0.; A(2,2) = 1.;
  A(3,0) = 0.; A(3,1) = 1.; A(3,2) = 0.;
  A(4,0) = 1.; A(4,1) = 0.; A(4,2) = 0.;
  A(5,0) = 0.; A(5,1) = 1.; A(5,2) = 1.;
  
  // eigen vector
  rompp::core::Vector<Eigen::VectorXd> b(6);
  b(0) = 1.; b(1) = 1.; b(2) = 1.;
  b(3) = 1.; b(4) = 2.; b(5) = 1.;
  
  auto c1 = rompp::core::ops::dot(A,b);
  std::cout << *c1.data() << std::endl;
  ASSERT_EQ(c1.size(), 3);
  EXPECT_DOUBLE_EQ( c1(0), 6.);
  EXPECT_DOUBLE_EQ( c1(1), 6.);
  EXPECT_DOUBLE_EQ( c1(2), 6.);
  
  rompp::core::Vector<Eigen::VectorXd> c(6);
  rompp::core::ops::dot(A,b,c);
  for (auto i=0; i<c1.size(); i++)
    EXPECT_DOUBLE_EQ( c(i), c(i));
  
}
