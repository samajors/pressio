#include <gtest/gtest.h>
#include "Epetra_Map.h"
#include "Epetra_MultiVector.h"
#include "Epetra_MpiComm.h"
#include "CONTAINERS_ALL"

TEST(containers_multivector_distributed_epetra,
     Traits){
  using namespace rompp;

  using natV_t = Epetra_MultiVector;
  STATIC_ASSERT_IS_NOT_VECTOR_EIGEN(natV_t);
  STATIC_ASSERT_IS_NOT_VECTOR_EPETRA(natV_t);
  STATIC_ASSERT_IS_MULTIVECTOR_EPETRA(natV_t);

  using myvec_t = containers::MultiVector<natV_t>;
  STATIC_ASSERT_IS_CONTAINERS_MULTI_VECTOR_WRAPPER(myvec_t);
  STATIC_ASSERT_IS_NOT_VECTOR_EIGEN(myvec_t);
  STATIC_ASSERT_IS_NOT_VECTOR_EPETRA(myvec_t);
  STATIC_ASSERT_IS_NOT_MULTIVECTOR_EPETRA(myvec_t);

  using vecTrait = containers::details::traits<myvec_t>;
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::scalar_t,
		   containers::default_types::epetra_scalar_t>();
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::local_ordinal_t,
		   containers::default_types::epetra_lo_t>();
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::global_ordinal_t,
		   containers::default_types::epetra_go_t1>();
  
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::wrapped_t, natV_t>();
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::derived_t, myvec_t>();
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::data_map_t,
		   Epetra_BlockMap>();
  ::testing::StaticAssertTypeEq<typename
		   vecTrait::communicator_t,
		   Epetra_Comm>();
  
  ASSERT_TRUE(vecTrait::is_vector == false);
  ASSERT_TRUE(vecTrait::is_matrix == false);
  ASSERT_TRUE(vecTrait::is_multi_vector == true);
  ASSERT_TRUE(vecTrait::is_shared_mem == false);
  ASSERT_TRUE(vecTrait::wrapped_package_identifier ==
	    containers::details::WrappedPackageIdentifier::Trilinos);
}//end TEST