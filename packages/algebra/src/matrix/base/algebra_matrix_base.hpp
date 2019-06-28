
#ifndef ALGEBRA_MATRIX_BASE_MATRIX_BASE_HPP_
#define ALGEBRA_MATRIX_BASE_MATRIX_BASE_HPP_

#include "../algebra_matrix_traits.hpp"

namespace rompp{ namespace algebra{

template<typename derived_type>
class MatrixBase
  : private utils::details::CrtpBase<
  MatrixBase<derived_type>>{

  using traits_t = details::traits<derived_type>;
  using sc_t = typename traits_t::scalar_t;

public:
  void setIdentity(){
    this->underlying().setIdentityImpl();
  }

  bool isLowerTriangular(){
    return this->underlying().isLowerTriangularImpl();
  }

  bool isUpperTriangular(){
    return this->underlying().isUpperTriangularImpl();
  }

  template <typename T,
        ::rompp::mpl::enable_if_t<
        std::is_same<T,sc_t>::value
        > * = nullptr>
  void addToDiagonal(T value) {
    return this->underlying().addToDiagonalImpl(value);
  }

private:
  /* workaround for nvcc issue with templates, see https://devtalk.nvidia.com/default/topic/1037721/nvcc-compilation-error-with-template-parameter-as-a-friend-within-a-namespace/ */
  template<typename DummyType> struct dummy{using type = DummyType;};
  friend typename dummy<derived_type>::type;

  friend utils::details::CrtpBase<MatrixBase<derived_type>>;
  MatrixBase() = default;
  ~MatrixBase() = default;
};//end class


}}//end namespace rompp::algebra
#endif