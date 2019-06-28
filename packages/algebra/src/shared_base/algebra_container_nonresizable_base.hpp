
#ifndef ALGEBRA_SHARED_BASE_CONTAINER_NONRESIZABLE_BASE_HPP_
#define ALGEBRA_SHARED_BASE_CONTAINER_NONRESIZABLE_BASE_HPP_

#include "../algebra_ConfigDefs.hpp"

namespace rompp{
namespace algebra{

template<typename derived_type, int ndim>
class ContainerNonResizableBase
  : private utils::details::CrtpBase<
  ContainerNonResizableBase<derived_type,ndim>>{

  using this_t = ContainerNonResizableBase<derived_type, ndim>;

  /* workaround for nvcc issue with templates, see https://devtalk.nvidia.com/default/topic/1037721/nvcc-compilation-error-with-template-parameter-as-a-friend-within-a-namespace/ */
  template<typename DummyType> struct dummy{using type = DummyType;};
  friend typename dummy<derived_type>::type;

  friend utils::details::CrtpBase<this_t>;

  ContainerNonResizableBase() = default;
  ~ContainerNonResizableBase() = default;

};//end class
} // end namespace algebra
}//end namespace rompp
#endif
