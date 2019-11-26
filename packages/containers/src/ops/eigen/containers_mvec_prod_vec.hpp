/*
//@HEADER
// ************************************************************************
//
// containers_mvec_prod_vec.hpp
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
#ifndef CONTAINERS_SRC_OPS_EIGEN_MULTI_VECTOR_PROD_VECTOR_HPP_
#define CONTAINERS_SRC_OPS_EIGEN_MULTI_VECTOR_PROD_VECTOR_HPP_

#include "../containers_ops_meta.hpp"
#include "../../multi_vector/containers_multi_vector_meta.hpp"

namespace pressio{ namespace containers{ namespace ops{

/* multi_vector prod vector */

/* -------------------------------------------------------------------
 * specialize for eigen mv operating on a eigen vector
 *-------------------------------------------------------------------*/
template <
  typename mvec_type,
  typename vec_type,
  ::pressio::mpl::enable_if_t<
   containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
   containers::meta::is_vector_wrapper_eigen<vec_type>::value and
   containers::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value
    > * = nullptr
  >
void product(const mvec_type & mvA, const vec_type & vecB, vec_type & C){

  assert( C.size() == mvA.length() );
  C.setZero();
  const auto numVecs = mvA.numVectors();
  assert(numVecs == vecB.size());

  // compute
  (*C.data()) = (*mvA.data()) * (*vecB.data());
  // for (size_t i=0; i<(size_t)Alength; i++){
  //   for (size_t j=0; j<(size_t)numVecs; j++){
  //     C[i] += mvA(i,j) * vecB[j];
  //   }
  // }
}//end function

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
vec_type product(const mvec_type & mvA, const vec_type & vecB){
  vec_type c(mvA.length());
  product(mvA, vecB, c);
  return c;
}


/* -------------------------------------------------------------------
 * specialize for eigen mv operating on an expression
 *-------------------------------------------------------------------*/
template <
  typename mvec_type,
  typename expr_type,
  typename vec_type,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    containers::meta::is_vector_wrapper_eigen<vec_type>::value and
    containers::meta::wrapper_pair_have_same_scalar<mvec_type, vec_type>::value and
    expr_type::is_view_col_vector_expr
    > * = nullptr
  >
void product(const mvec_type & mvA, const expr_type & b, vec_type & C)
{
  assert( C.size() == mvA.length() );
  const auto numVecs = mvA.numVectors();
  assert(numVecs == b.size());

  const auto & wrapper = b.getCRefToObject();
  const auto & colIndex = b.getIndex();
  (*C.data()) = (*mvA.data()) * (wrapper.data()->col(colIndex) );

}//end function


template <
  typename mvec_type,
  typename expr_type,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_eigen<mvec_type>::value and
    expr_type::is_view_col_vector_expr
    > * = nullptr
  >
auto product(const mvec_type & mvA, const expr_type & b)
  // return a wrapper of a dynamic col vector
  -> ::pressio::containers::Vector<
    Eigen::Matrix< typename containers::details::traits<mvec_type>::scalar_t, -1, 1>
    >
{
  using scalar_t  = typename containers::details::traits<mvec_type>::scalar_t;
  using eig_vec_t = Eigen::Matrix<scalar_t, -1, 1>;
  using return_t  = ::pressio::containers::Vector<eig_vec_t>;

  return_t c(mvA.length());
  product(mvA, b, c);
  return c;
}


}}}//end namespace pressio::containers::ops
#endif
