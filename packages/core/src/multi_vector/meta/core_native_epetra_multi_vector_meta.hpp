
#ifdef HAVE_TRILINOS
#ifndef CORE_NATIVE_EPETRA_MULTI_VECTOR_META_HPP_
#define CORE_NATIVE_EPETRA_MULTI_VECTOR_META_HPP_

#include "../../meta/core_meta_basic.hpp"
#include "Epetra_MultiVector.h"

namespace rompp{ namespace core{ namespace meta {

template <typename T, typename enable = void>
struct is_multi_vector_epetra : std::false_type {};

template <typename T>
struct is_multi_vector_epetra<T,
  typename
   std::enable_if<
    std::is_same<T,Epetra_MultiVector>::value
   >::type
  > : std::true_type{};

}}}//end namespace rompp::core::meta
#endif
#endif