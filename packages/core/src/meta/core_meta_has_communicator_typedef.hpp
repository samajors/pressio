
#ifndef CORE_META_HAS_COMMUNICATOR_TYPEDEF_HPP_
#define CORE_META_HAS_COMMUNICATOR_TYPEDEF_HPP_

#include <type_traits>


namespace rompp{ namespace core{ namespace meta {

  template <typename T, typename enable = void>
  struct has_communicator_typedef : std::false_type{};

  template <typename T>
  struct has_communicator_typedef<T,
          typename
          std::enable_if<
            !std::is_void<typename
              T::communicator_type
              >::value
            >::type
          > : std::true_type{};

}}}//end namespace rompp::core::meta
#endif