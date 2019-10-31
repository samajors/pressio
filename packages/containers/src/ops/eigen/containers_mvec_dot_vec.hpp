/*
//@HEADER
// ************************************************************************
//
// containers_mvec_dot_vec.hpp
//                     		  Pressio
//                             Copyright 2019
//    National Technology & Engineering Solutions of Sandia, LLC (NTESS)
//
// Under the terms of Contract DE-NA0003525 with NTESS, the
// U.S. Government retains certain rights in this software.
//
// Pressio is licensed under BSD-3-Clause terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Francesco Rizzi (fnrizzi@sandia.gov)
//
// ************************************************************************
//@HEADER
*/
#ifndef CONTAINERS_SRC_OPS_EIGEN_MULTI_VECTOR_DOT_VECTOR_HPP_
#define CONTAINERS_SRC_OPS_EIGEN_MULTI_VECTOR_DOT_VECTOR_HPP_

#include "../containers_ops_meta.hpp"
#include "../../multi_vector/containers_multi_vector_meta.hpp"
#include "../../vector/containers_vector_meta.hpp"

namespace pressio{ namespace containers{ namespace ops{

/*
 * multi_vector dot vector
 */

// void specializiation for:
// * vec_type is a DYNAMIC eigen vector wrapper
// * result_vec_type is the same as vec_type
template <
  typename mvec_type,
  typename vec_type,
  typename result_vec_type,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    containers::meta::is_vector_wrapper_eigen<vec_type>::value and
    containers::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value and
    containers::meta::is_vector_wrapper_eigen<result_vec_type>::value and
    containers::meta::wrapper_pair_have_same_scalar<vec_type,result_vec_type>::value and
    containers::details::traits<result_vec_type>::is_dynamic
    > * = nullptr
  >
void dot(const mvec_type & mvA,
	 const vec_type & vecB,
	 result_vec_type & result){
  const auto numVecs = mvA.numVectors();
  // I can resize if needed because I know here it is a dynamic vector
  if ( result.size() != numVecs )
    result.resize(numVecs);
  *result.data() = (*mvA.data()).transpose() * (*vecB.data());
}


// non-void specialize for:
// * vec_type is a DYNAMIC eigen vector wrapper
// * result type is the same as vec_type
template <
  typename mvec_type,
  typename vec_type,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    containers::meta::is_vector_wrapper_eigen<vec_type>::value and
    containers::details::traits<vec_type>::is_dynamic and
    containers::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value
    > * = nullptr
  >
vec_type dot(const mvec_type & mvA, const vec_type & vecB)
{
  vec_type c(mvA.data()->cols());
  dot(mvA,vecB,c);
  return c;
}


// void specialize for:
// * vec_type is a generic eigen vector wrapper
// * result_vec_type is a STATIC eigen vector wrapper
template <
  typename mvec_type,
  typename vec_type,
  typename result_vec_type,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    containers::meta::is_vector_wrapper_eigen<vec_type>::value and
    containers::meta::is_vector_wrapper_eigen<result_vec_type>::value and 
    containers::details::traits<result_vec_type>::is_static and
    containers::meta::wrapper_triplet_have_same_scalar<
      mvec_type, vec_type, result_vec_type>::value
    > * = nullptr
  >
void dot(const mvec_type & mvA,
	 const vec_type & vecB,
	 result_vec_type & result)
{
  // we are dealing with static vector type, so this needs to be true
  assert(result.size() == mvA.data()->cols());
  // compute
  *result.data() = (*mvA.data()).transpose() * (*vecB.data());
}

}}}//end namespace pressio::containers::ops
#endif