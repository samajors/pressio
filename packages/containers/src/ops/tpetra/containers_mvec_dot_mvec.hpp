/*
//@HEADER
// ************************************************************************
//
// containers_mvec_dot_mvec.hpp
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

#ifdef PRESSIO_ENABLE_TPL_TRILINOS
#ifndef CONTAINERS_SRC_OPS_TPETRA_MULTI_VECTOR_DOT_MULTI_VECTOR_HPP_
#define CONTAINERS_SRC_OPS_TPETRA_MULTI_VECTOR_DOT_MULTI_VECTOR_HPP_

#include "../../multi_vector/containers_multi_vector_meta.hpp"

namespace pressio{ namespace containers{ namespace ops{

/*
 * multi_vector dot multi vector
 */

// result is stored into an Eigen dense matrix wrapper
template <
  typename mvec_t,
  typename result_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value and
    ::pressio::containers::meta::is_dense_matrix_wrapper_eigen<result_t>::value and
    ::pressio::containers::meta::wrapper_pair_have_same_scalar<mvec_t, result_t>::value
    > * = nullptr
  >
void dot(const mvec_t & mvA, const mvec_t & mvB, result_t & C){
  // how many vectors are in mvA and mvB
  const auto numVecsA = mvA.numVectors();
  const auto numVecsB = mvB.numVectors();
  assert( mvA.extent(0) == mvB.extent(0));
  assert(C.extent(0) == numVecsA);
  assert(C.extent(1) == numVecsB);
  // compute dot between every column of A with every col of B
  for (std::size_t i=0; i<(std::size_t)numVecsA; i++){
    // colI is a Teuchos::RCP<Vector<...>>
    const auto colI = mvA.data()->getVector(i);
    for (std::size_t j=0; j< (std::size_t)numVecsB; j++)
    {
      const auto colJ = mvB.data()->getVector(j);
      C(i,j) = colI->dot(*colJ);
    }
  }
}

// result is stored into an Eigen dense DYNAMIC matrix wrapper
template <
  typename mvec_t,
  typename result_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value and
    ::pressio::containers::meta::is_dense_matrix_wrapper_eigen<result_t>::value and
    ::pressio::containers::details::traits<result_t>::is_dynamic and
    ::pressio::containers::meta::wrapper_pair_have_same_scalar<mvec_t, result_t>::value
    > * = nullptr
  >
result_t dot(const mvec_t & mvA, const mvec_t & mvB){

  const auto numVecsA = mvA.numVectors();
  const auto numVecsB = mvB.numVectors();
  result_t C(numVecsA, numVecsB);
  dot(mvA, mvB, C);
  return C;
}


// C += mvA^T mvB
template <
  typename mvec_t,
  typename expr_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value and
    ::pressio::containers::meta::is_expression<expr_t>::value and
    ::pressio::containers::meta::wrapper_pair_have_same_scalar<mvec_t, expr_t>::value and
    ::pressio::containers::meta::is_matrix_wrapper_eigen<
      typename ::pressio::containers::details::traits<expr_t>::data_t
      >::value
    > * = nullptr
  >
void updateWithDot(const mvec_t & mvA, const mvec_t & mvB, expr_t & C){
  // how many vectors are in mvA and mvB
  const auto numVecsA = mvA.numVectors();
  const auto numVecsB = mvB.numVectors();
  assert( mvA.extent(0) == mvB.extent(0));
  assert(C.extent(0) == numVecsA);
  assert(C.extent(1) == numVecsB);
  // compute dot between every column of A with every col of B
  for (std::size_t i=0; i<(std::size_t)numVecsA; i++){
    // colI is a Teuchos::RCP<Vector<...>>
    const auto colI = mvA.data()->getVector(i);
    for (std::size_t j=0; j< (std::size_t)numVecsB; j++)
    {
      const auto colJ = mvB.data()->getVector(j);
      C(i,j) += colI->dot(*colJ);
    }
  }
}



/* ----------------------------------------------
 * result_t = an expression
 ---------------------------------------------- */
template <
  typename mvec_t,
  typename result_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value and
    ::pressio::containers::meta::is_expression<result_t>::value and
    ::pressio::containers::meta::wrapper_pair_have_same_scalar<mvec_t, result_t>::value
    > * = nullptr
  >
void dot(const mvec_t & mvA, const mvec_t & mvB, result_t & C){
  // how many vectors are in mvA and mvB
  const auto numVecsA = mvA.numVectors();
  const auto numVecsB = mvB.numVectors();
  assert( mvA.extent(0) == mvB.extent(0));

  // compute dot between every column of A with every col of B
  for (std::size_t i=0; i<(std::size_t)numVecsA; i++){
    // colI is a Teuchos::RCP<Vector<...>>
    const auto colI = mvA.data()->getVector(i);
    for (std::size_t j=0; j< (std::size_t)numVecsB; j++)
    {
      const auto colJ = mvB.data()->getVector(j);
      C(i,j) = colI->dot(*colJ);
    }
  }
}

}}}//end namespace pressio::containers::ops
#endif
#endif
