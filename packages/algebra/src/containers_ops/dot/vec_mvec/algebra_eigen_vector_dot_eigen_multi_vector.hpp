
#ifndef ALGEBRA_EIGEN_VECTOR_DOT_EIGEN_MULTI_VECTOR_HPP_
#define ALGEBRA_EIGEN_VECTOR_DOT_EIGEN_MULTI_VECTOR_HPP_

#include "../../algebra_ops_meta.hpp"
#include "../../../vector/algebra_vector_meta.hpp"
#include "../../../multi_vector/algebra_multi_vector_meta.hpp"

namespace rompp{ namespace algebra{ namespace ops{

// Eigen vector dot multivector
// result stored in Eigen DYNAMIC vector passed by reference
template <
  typename vec_type,
  typename mvec_type,
  typename result_vec_type,
  ::rompp::mpl::enable_if_t<
    algebra::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    algebra::meta::is_vector_wrapper_eigen<vec_type>::value and
    algebra::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value and
    algebra::meta::is_vector_wrapper_eigen<result_vec_type>::value and
    algebra::meta::wrapper_pair_have_same_scalar<vec_type,result_vec_type>::value and
    algebra::details::traits<result_vec_type>::is_dynamic
    > * = nullptr
  >
void dot(const vec_type & vec,
	 const mvec_type & mv,
	 result_vec_type & result)
{
  dot(mv, vec, result);
}


// Eigen vector dot multivector
// result stored in Eigen STATIC vector passed by reference
template <
  typename vec_type,
  typename mvec_type,
  typename result_vec_type,
  ::rompp::mpl::enable_if_t<
    algebra::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    algebra::meta::is_vector_wrapper_eigen<vec_type>::value and
    algebra::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value and
    algebra::meta::is_vector_wrapper_eigen<result_vec_type>::value and
    algebra::meta::wrapper_pair_have_same_scalar<vec_type,result_vec_type>::value and
    algebra::details::traits<result_vec_type>::is_static
    > * = nullptr
  >
void dot(const vec_type & vec,
	 const mvec_type & mv,
	 result_vec_type & result){
  dot(mv, vec, result);
}


// Eigen vector dot multivector
// result is built and returned
template <typename vec_type,
	  typename mvec_type,
  ::rompp::mpl::enable_if_t<
    algebra::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    algebra::meta::is_vector_wrapper_eigen<vec_type>::value and
    algebra::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value
    > * = nullptr
  >
auto dot(const vec_type & vec, const mvec_type & mv)
-> algebra::Vector<
  Eigen::Matrix<typename algebra::details::traits<mvec_type>::scalar_t,
                Eigen::Dynamic, 1>
                >{

  return dot(mv, vec);
}

}}} // end namespace rompp::algebra::ops
#endif