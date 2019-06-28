
#include <gtest/gtest.h>
#include "ALGEBRA_MULTI_VECTOR"
#include "ALGEBRA_OPS"

using eigdmat_t = Eigen::MatrixXd;
using myMV_t = rompp::algebra::MultiVector<eigdmat_t>;


TEST(algebra_multi_vector_serial_eigen_dynamic_class,
     constructor){

  using MVTrait = rompp::algebra::details::traits<myMV_t>;
  ASSERT_TRUE(MVTrait::wrapped_multi_vector_identifier
  == rompp::algebra::details::WrappedMultiVectorIdentifier::Eigen);

  ASSERT_TRUE(
  rompp::algebra::meta::is_multi_vector_wrapper_eigen<myMV_t>::value);
  
  //construct by passing the size 
  myMV_t A(6,3);

  // pass native eigen data
  eigdmat_t eA(45,32);
  myMV_t AW(eA);
}


TEST(algebra_multi_vector_serial_eigen_dynamic_class,
     constructorAndCheckVals){

  //construct by passing the sizes 
  myMV_t A(6,3);
  ASSERT_FALSE( A.empty() );
  ASSERT_TRUE( A.length() == 6 );
  ASSERT_TRUE( A.numVectors() == 3 );
  for (size_t i=0; i<6; i++)
    for (size_t j=0; j<3; j++)
      EXPECT_DOUBLE_EQ( A(i,j), 0.);
  
  // pass native eigen vector
  eigdmat_t eA(45,12);
  eA(2,2) = 2.2;
  eA(4,11) = 4.4;

  myMV_t B(eA);
  ASSERT_FALSE( B.empty() );
  ASSERT_TRUE( B.length() == 45 );
  ASSERT_TRUE( B.numVectors() == 12 );
  ASSERT_FALSE( B.length() == 4 );
  ASSERT_FALSE( B.numVectors() == 1 );
  EXPECT_DOUBLE_EQ( B(2,2), 2.2);
  EXPECT_DOUBLE_EQ( B(4,11), 4.4);
}


TEST(algebra_multi_vector_serial_eigen_dynamic_class,
     copyConstructor){

  //construct by passing the size 
  myMV_t a(3,2);
  a(0,0)=1.1;
  a(0,1)=1.2;
  a(2,1)=1.3;
  
  myMV_t b(a);
  EXPECT_DOUBLE_EQ( b(0,0), 1.1);
  EXPECT_DOUBLE_EQ( b(0,1), 1.2);
  EXPECT_DOUBLE_EQ( b(2,1), 1.3);
}