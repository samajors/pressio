
#ifndef ALGEBRA_MATRIX_BASE_MATRIX_DISTRIBUTED_BASE_HPP_
#define ALGEBRA_MATRIX_BASE_MATRIX_DISTRIBUTED_BASE_HPP_

#include "../algebra_matrix_traits.hpp"

namespace rompp{
namespace algebra{

template<typename derived_type>
class MatrixDistributedBase
  : private utils::details::CrtpBase<
  MatrixDistributedBase<derived_type>>{

  static_assert( details::traits<derived_type>::is_shared_mem==0,
   "OOPS: non-distributed matrix inheriting from distributed base!");

  using traits_t = details::traits<derived_type>;
  using LO_t = typename traits_t::local_ordinal_t;
  using GO_t = typename traits_t::global_ordinal_t;

public:
  LO_t localRows() const{
    return this->underlying().localRowsImpl();}

  LO_t localCols() const{
    return this->underlying().localColsImpl();}

  GO_t globalRows() const{
    return this->underlying().globalRowsImpl();}

  GO_t globalCols() const{
    return this->underlying().globalColsImpl();}

private:
  /* workaround for nvcc issue with templates, see https://devtalk.nvidia.com/default/topic/1037721/nvcc-compilation-error-with-template-parameter-as-a-friend-within-a-namespace/ */
  template<typename DummyType> struct dummy{using type = DummyType;};
  friend typename dummy<derived_type>::type;

  friend utils::details::CrtpBase<MatrixDistributedBase<derived_type>>;
  MatrixDistributedBase() = default;
  ~MatrixDistributedBase() = default;

};//end class
} // end namespace algebra
}//end namespace rompp
#endif