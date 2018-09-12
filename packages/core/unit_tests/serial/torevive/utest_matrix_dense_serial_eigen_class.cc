
#include <gtest/gtest.h>
#include "CORE_MATRIX"

TEST(core_matrix_dense_serial_eigen_class, constructor)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;
  STATIC_ASSERT_IS_CORE_MATRIX_WRAPPER(mymat_t);
  using matTrait = core::details::traits<mymat_t>;
   
  mymat_t m1;
  EXPECT_EQ( m1.rows(), 0 );
  EXPECT_EQ( m1.cols(), 0 );

  mymat_t m2(5, 8);
  EXPECT_EQ( m2.rows(), 5 );
  EXPECT_EQ( m2.cols(), 8 );

  native_t eigMat;
  eigMat.resize(56,101);
  mymat_t m3(eigMat);
  EXPECT_EQ( m3.rows(), 56 );
  EXPECT_EQ( m3.cols(), 101 );
}


TEST(core_matrix_dense_serial_eigen_class, queryWrappedData)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  mymat_t m1;
  ::testing::StaticAssertTypeEq<decltype(m1.data()),
				native_t * >();
  const mymat_t m2(45,64);
  ::testing::StaticAssertTypeEq< decltype(m2.data()),
				 const native_t * >();
}


TEST(core_matrix_dense_serial_eigen_class, sizeResize)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  mymat_t m1;
  EXPECT_EQ( m1.rows(), 0 );
  EXPECT_EQ( m1.cols(), 0 );
  m1.resize(11,45);
  EXPECT_NE( m1.rows(), 0 );
  EXPECT_NE( m1.cols(), 0 );
  EXPECT_EQ( m1.rows(), 11 );
  EXPECT_EQ( m1.cols(), 45 );
}


TEST(core_matrix_dense_serial_eigen_class, subscriptOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,3);
  em1 << 34.0, 22.5, 11.5, 75., 3., 6.;  

  mymat_t m1(em1);
  EXPECT_DOUBLE_EQ( m1(0,0), 34.0);
  EXPECT_DOUBLE_EQ( m1(0,1), 22.5);
  EXPECT_DOUBLE_EQ( m1(0,2), 11.5);
  EXPECT_DOUBLE_EQ( m1(1,0), 75.);
  EXPECT_DOUBLE_EQ( m1(1,1), 3.0);
  EXPECT_DOUBLE_EQ( m1(1,2), 6.0);

  // create an eigen-type vector
  mymat_t m3(em1);
  EXPECT_EQ( m3.rows(), 2 );
  EXPECT_EQ( m3.cols(), 3 );
  m3(1,1) = 55.;
  m3(0,2) = -12.;
  EXPECT_DOUBLE_EQ( m3(1,1), 55.);
  EXPECT_DOUBLE_EQ( m3(0,2), -12.);
}


TEST(core_matrix_dense_serial_eigen_class, additionOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,2);
  em1 << 2., 4., 3., 6.;
  mymat_t m1(em1);

  native_t em2;
  em2.resize(2,2);
  em2 << 1., 2., 1., 2.;
  mymat_t m2(em2);

  mymat_t res = m1 + m2;  
  EXPECT_DOUBLE_EQ(res(0,0), 3.);
  EXPECT_DOUBLE_EQ(res(0,1), 6.);
  EXPECT_DOUBLE_EQ(res(1,0), 4.);
  EXPECT_DOUBLE_EQ(res(1,1), 8.);
}

TEST(core_matrix_dense_serial_eigen_class, substractOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,2);
  em1 << 2., 4., 3., 6.;
  mymat_t m1(em1);

  native_t em2;
  em2.resize(2,2);
  em2 << 1., 2., 1., 2.;
  mymat_t m2(em2);

  mymat_t res = m1 - m2;  
  EXPECT_DOUBLE_EQ(res(0,0), 1.);
  EXPECT_DOUBLE_EQ(res(0,1), 2.);
  EXPECT_DOUBLE_EQ(res(1,0), 2.);
  EXPECT_DOUBLE_EQ(res(1,1), 4.);
}


TEST(core_matrix_dense_serial_eigen_class, starOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,2);
  em1 << 2., 4., 3., 6.;
  mymat_t m1(em1);

  native_t em2;
  em2.resize(2,2);
  em2 << 1., 2., 1., 2.;
  mymat_t m2(em2);

  mymat_t res = m1 * m2;
  EXPECT_DOUBLE_EQ(res(0,0), 6.);
  EXPECT_DOUBLE_EQ(res(0,1), 12.);
  EXPECT_DOUBLE_EQ(res(1,0), 9.);
  EXPECT_DOUBLE_EQ(res(1,1), 18.);
}

TEST(core_matrix_dense_serial_eigen_class, CompoundAssignAddOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,2);
  em1 << 2., 4., 3., 6.;
  mymat_t m1(em1);

  native_t em2;
  em2.resize(2,2);
  em2 << 1., 2., 1., 2.;
  mymat_t m2(em2);

  m1 += m2;
  EXPECT_DOUBLE_EQ(m1(0,0), 3.);
  EXPECT_DOUBLE_EQ(m1(0,1), 6.);
  EXPECT_DOUBLE_EQ(m1(1,0), 4.);
  EXPECT_DOUBLE_EQ(m1(1,1), 8.);
}


TEST(core_matrix_dense_serial_eigen_class, CompoundAssignSubtractOperator)
{
  using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
  using mymat_t = core::Matrix<native_t>;

  native_t em1;
  em1.resize(2,2);
  em1 << 2., 4., 3., 6.;
  mymat_t m1(em1);

  native_t em2;
  em2.resize(2,2);
  em2 << 1., 2., 1., 2.;
  mymat_t m2(em2);

  m1 -= m2;
  EXPECT_DOUBLE_EQ(m1(0,0), 1.);
  EXPECT_DOUBLE_EQ(m1(0,1), 2.);
  EXPECT_DOUBLE_EQ(m1(1,0), 2.);
  EXPECT_DOUBLE_EQ(m1(1,1), 4.);
}



// TEST(core_matrix_dense_serial_eigen_class, transposeDynamic)
// {
//   using native_t = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
//   using mymat_t = core::Matrix<native_t>;

//   native_t em1;
//   em1.resize(2,3);
//   em1 << 34.0, 22.5, 11.5, 75., 3., 6.;  
//   std::cout << em1 << std::endl;

//   mymat_t m1(em1);
//   EXPECT_DOUBLE_EQ( m1(0,0), 34.0);
//   EXPECT_DOUBLE_EQ( m1(0,1), 22.5);
//   EXPECT_DOUBLE_EQ( m1(0,2), 11.5);
//   EXPECT_DOUBLE_EQ( m1(1,0), 75.);
//   EXPECT_DOUBLE_EQ( m1(1,1), 3.0);
//   EXPECT_DOUBLE_EQ( m1(1,2), 6.0);

//   auto tm1 = core::mat_ops::transpose(m1);
//   std::cout << *tm1.data() << std::endl;
//   EXPECT_DOUBLE_EQ( tm1(0,0), 34.0);
//   EXPECT_DOUBLE_EQ( tm1(1,0), 22.5);
//   EXPECT_DOUBLE_EQ( tm1(2,0), 11.5);
//   EXPECT_DOUBLE_EQ( tm1(0,1), 75.);
//   EXPECT_DOUBLE_EQ( tm1(1,1), 3.0);
//   EXPECT_DOUBLE_EQ( tm1(2,1), 6.0);
// }


// TEST(core_matrix_dense_serial_eigen_class, transposeStatic)
// {
//   using native_t = Eigen::Matrix<double, 2, 3>;
//   using mymat_t = core::Matrix<native_t>;

//   native_t em1;
//   em1 << 34.0, 22.5, 11.5, 75., 3., 6.;  
//   //std::cout << em1 << std::endl;

//   mymat_t m1(em1);
//   EXPECT_DOUBLE_EQ( m1(0,0), 34.0);
//   EXPECT_DOUBLE_EQ( m1(0,1), 22.5);
//   EXPECT_DOUBLE_EQ( m1(0,2), 11.5);
//   EXPECT_DOUBLE_EQ( m1(1,0), 75.);
//   EXPECT_DOUBLE_EQ( m1(1,1), 3.0);
//   EXPECT_DOUBLE_EQ( m1(1,2), 6.0);

//   auto tm1 = core::mat_ops::transpose(m1);
//   //std::cout << *tm1.data() << std::endl;
//   EXPECT_DOUBLE_EQ( tm1(0,0), 34.0);
//   EXPECT_DOUBLE_EQ( tm1(1,0), 22.5);
//   EXPECT_DOUBLE_EQ( tm1(2,0), 11.5);
//   EXPECT_DOUBLE_EQ( tm1(0,1), 75.);
//   EXPECT_DOUBLE_EQ( tm1(1,1), 3.0);
//   EXPECT_DOUBLE_EQ( tm1(2,1), 6.0);
// }