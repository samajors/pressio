
#include <gtest/gtest.h>
#include "ALGEBRA_MATRIX"

//Eigen::MatrixNt
// N can be any one of 2, 3, 4, or X (meaning Dynamic).
// t = i (int), f (float), d (double),
// cf ( complex<float>), or cd (complex<double>)

template<typename scalar, int rows, int cols>
struct typesEig{
  using sc_t = scalar;
  constexpr static int nr = rows;
  constexpr static int nc = cols;
};

template<typename T>
struct algebra_matrix_dense_serial_eigen_traitsTest
  : public ::testing::Test{
public:
  using native_t = Eigen::Matrix<typename T::sc_t,T::nr, T::nc>;
  STATIC_ASSERT_IS_MATRIX_DENSE_SHAREDMEM_EIGEN(native_t);
  using myvec_t = algebra::Matrix<native_t>;
  using vecTrait = algebra::details::traits<myvec_t>;

  //need to have this void method otherwise the static assertion for type cannot be used
  void check(){
    ::testing::StaticAssertTypeEq<typename
				  vecTrait::scalar_t, typename T::sc_t>();
    ::testing::StaticAssertTypeEq<typename
				  vecTrait::ordinal_t,int>();
    ::testing::StaticAssertTypeEq<typename
				  vecTrait::wrapped_t,native_t>();
    ::testing::StaticAssertTypeEq<typename
				  vecTrait::derived_t,myvec_t>();

    ASSERT_TRUE(vecTrait::is_matrix == 1);
	ASSERT_TRUE(vecTrait::wrapped_matrix_identifier 
	== algebra::details::WrappedMatrixIdentifier::DenseEigen);

    ASSERT_TRUE(vecTrait::is_dense == 1);
    ASSERT_TRUE(vecTrait::is_vector == 0);
    ASSERT_TRUE(vecTrait::is_shared_mem == 1);
    ASSERT_TRUE(vecTrait::is_static == (T::nr!=-1 && T::nc!=-1) );
  }
};

typedef ::testing::Types< typesEig<int,-1,-1>, typesEig<int,2,2>,
			  typesEig<int,-1,4>, typesEig<int,4,-1>,
			  typesEig<int,11,11>, typesEig<int,11,11>,
			  //
			  typesEig<double,-1,-1>, typesEig<double,2,2>,
			  typesEig<double,-1,4>, typesEig<double,4,-1>,
			  typesEig<double,11,11>, typesEig<double,11,11>,
			  //
			  typesEig<float,-1,-1>, typesEig<float,2,2>,
			  typesEig<float,-1,4>, typesEig<float,4,-1>,
			  typesEig<float,11,11>, typesEig<float,11,11>,
			  //
			  typesEig<std::complex<double>,-1,-1>,
			  typesEig<std::complex<double>,2,2>,
			  typesEig<std::complex<double>,-1,4>,
			  typesEig<std::complex<double>,4,-1>,
			  typesEig<std::complex<double>,11,11>,
			  typesEig<std::complex<double>,11,11>,
			  //
			  typesEig<unsigned int,-1,-1>, typesEig<unsigned int,2,2>,
			  typesEig<unsigned int,-1,4>, typesEig<unsigned int,4,-1>,
			  typesEig<unsigned int,11,11>, typesEig<unsigned int,11,11>
			  > MyTypes;
TYPED_TEST_CASE(algebra_matrix_dense_serial_eigen_traitsTest, MyTypes);

TYPED_TEST(algebra_matrix_dense_serial_eigen_traitsTest, traits)
{
  //this runs all types, no need to put anything
  this->check();
}
