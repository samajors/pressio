
#ifndef CONTAINERS_NATIVE_VECTOR_STATIC_ASSERTS_HPP_
#define CONTAINERS_NATIVE_VECTOR_STATIC_ASSERTS_HPP_

#include "./meta/containers_native_blaze_vector_meta.hpp"
#include "./meta/containers_native_eigen_vector_meta.hpp"
#include "./meta/containers_native_epetra_vector_meta.hpp"
#include "./meta/containers_native_tpetra_vector_meta.hpp"
#include "./meta/containers_native_teuchos_vector_meta.hpp"
#include "./meta/containers_native_kokkos_vector_meta.hpp"
#include "./meta/containers_native_tpetra_block_vector_meta.hpp"


namespace pressio{ namespace containers{

#define STATIC_ASSERT_IS_VECTOR_EIGEN(TYPE) \
  static_assert( containers::meta::is_vector_eigen<TYPE>::value, \
		 "THIS_IS_NOT_A_VECTOR_FROM_EIGEN")
#define STATIC_ASSERT_IS_NOT_VECTOR_EIGEN(TYPE) \
  static_assert( !containers::meta::is_vector_eigen<TYPE>::value, \
		 "THIS_IS_A_VECTOR_FROM_EIGEN")

#define STATIC_ASSERT_IS_VECTOR_STDLIB(TYPE) \
  static_assert( containers::meta::is_vector_stdlib<TYPE>::value, \
		 "THIS_IS_NOT_A_STDLIB_VECTOR")
#define STATIC_ASSERT_IS_NOT_VECTOR_STDLIB(TYPE) \
  static_assert( !containers::meta::is_vector_stdlib<TYPE>::value, \
		 "THIS_IS_A_STDLIB_VECTOR")

#ifdef HAVE_TRILINOS
#define STATIC_ASSERT_IS_VECTOR_EPETRA(TYPE) \
  static_assert( containers::meta::is_vector_epetra<TYPE>::value, \
		 "THIS_IS_NOT_A_VECTOR_EPETRA")
#define STATIC_ASSERT_IS_NOT_VECTOR_EPETRA(TYPE) \
  static_assert( !containers::meta::is_vector_epetra<TYPE>::value, \
		 "THIS_IS_A_VECTOR_EPETRA")

#define STATIC_ASSERT_IS_VECTOR_TPETRA(TYPE) \
  static_assert( containers::meta::is_vector_tpetra<TYPE>::value, \
		 "THIS_IS_NOT_A_VECTOR_TPETRA")
#define STATIC_ASSERT_IS_NOT_VECTOR_TPETRA(TYPE) \
  static_assert( !containers::meta::is_vector_tpetra<TYPE>::value, \
		 "THIS_IS_A_VECTOR_TPETRA")

#define STATIC_ASSERT_IS_VECTOR_TPETRA_BLOCK(TYPE) \
  static_assert( containers::meta::is_vector_tpetra_block<TYPE>::value, \
		 "THIS_IS_NOT_A_VECTOR_TPETRA_BLOCK")
#define STATIC_ASSERT_IS_NOT_VECTOR_TPETRA_BLOCK(TYPE) \
  static_assert( !containers::meta::is_vector_tpetra_block<TYPE>::value, \
		 "THIS_IS_A_VECTOR_TPETRA_BLOCK")
#endif


#ifdef HAVE_KOKKOS  
#define STATIC_ASSERT_IS_VECTOR_KOKKOS(TYPE) \
  static_assert( containers::meta::is_vector_kokkos<TYPE>::value, \
		 "THIS_IS_NOT_A_VECTOR_KOKKOS")
#define STATIC_ASSERT_IS_NOT_VECTOR_KOKKOS(TYPE)\
  static_assert( !containers::meta::is_vector_kokkos<TYPE>::value, \
		 "THIS_IS_A_VECTOR_KOKKOS")
#endif


#ifdef HAVE_BLAZE
#define STATIC_ASSERT_IS_VECTOR_BLAZE(TYPE)				\
  static_assert( containers::meta::is_static_vector_blaze<TYPE>::value ||	\
                 containers::meta::is_dynamic_vector_blaze<TYPE>::value,	\
		 "THIS_IS_NOT_A_VECTOR_BLAZE")
#define STATIC_ASSERT_IS_NOT_VECTOR_BLAZE(TYPE)				\
  static_assert( !containers::meta::is_static_vector_blaze<TYPE>::value &&	\
                 !containers::meta::is_dynamic_vector_blaze<TYPE>::value,	\
		 "THIS_IS_A_VECTOR_BLAZE")
#endif

#ifdef HAVE_ARMADILLO
#define STATIC_ASSERT_IS_VECTOR_ARMADILLO(TYPE)			\
  static_assert( containers::meta::is_vector_armadillo<TYPE>::value,	\
		 "THIS_IS_NOT_A_VECTOR_ARMADILLO")
#define STATIC_ASSERT_IS_NOT_VECTOR_ARMADILLO(TYPE)		\
  static_assert( !containers::meta::is_vector_armadillo<TYPE>::value,	\
		 "THIS_IS_A_VECTOR_ARMADILLO")
#endif

}}//end namespace pressio::containers
#endif
