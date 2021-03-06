
#include "block_tpetra_only_fixtures.hpp"

using fix_t = tpetraBlockVectorGlobSize15BlockSize5Fixture;
using native_t = typename fix_t::vec_t;
using myvec_t = pressio::containers::Vector<native_t>;

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       QueryWrappedData){
  using namespace pressio;

  myvec_t v1( *x_ );
  ::testing::StaticAssertTypeEq<decltype(v1.data()),
  				native_t * >();
  const myvec_t v2( *x_ );
  ::testing::StaticAssertTypeEq< decltype(v2.data()),
  				 const native_t * >();
}

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       ConstructorFromNative){
  using namespace pressio;
  static_assert
    ( containers::meta::is_vector_wrapper_tpetra_block<myvec_t>::value, "");
  using sc_t = typename fix_t::ST;

  x_->putScalar(1.22);
  myvec_t a( *x_ );

  // a is wrapper of a block vector, so get a tpetra vector
  auto a_tp = a.data()->getVectorView();
  // now that we have a regular tpetra vector, we can check data
  Teuchos::ArrayRCP<const sc_t> dd = a_tp.getData();
  for (int i=0; i<a.extentLocal(0); i++){
    EXPECT_DOUBLE_EQ( dd[i], 1.22 );
  }
}

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       CopyConstructor){
  using namespace pressio;
  static_assert
    ( containers::meta::is_vector_wrapper_tpetra_block<myvec_t>::value, "");
  using sc_t = typename fix_t::ST;

  myvec_t B( *x_ );
  B.data()->putScalar(1.22);

  myvec_t a(B);
  // a is wrapper of a block vector, so get a tpetra vector
  auto a_tp = a.data()->getVectorView();
  // now that we have a regular tpetra vector, we can check data
  Teuchos::ArrayRCP<const sc_t> dd = a_tp.getData();
  for (int i=0; i<a.extentLocal(0); i++){
    EXPECT_DOUBLE_EQ( dd[i], 1.22 );
  }
}

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       localSize){
  using namespace pressio;
  myvec_t v1( *x_ );
  EXPECT_EQ( numProc_, 3);
  EXPECT_EQ( v1.extentLocal(0), 5);
}

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       globalSize){
  using namespace pressio;
  myvec_t v1( *x_ );
  EXPECT_EQ( numProc_, 3);
  EXPECT_EQ( v1.extent(0), 15);
}

TEST_F(tpetraBlockVectorGlobSize15BlockSize5Fixture,
       blockSize){
  using namespace pressio;
  myvec_t v1( *x_ );
  EXPECT_EQ( v1.data()->getBlockSize(), 4);
}

