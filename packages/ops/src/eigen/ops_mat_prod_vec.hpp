/*
//@HEADER
// ************************************************************************
//
// ops_mat_prod_vec.hpp
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
#ifndef OPS_SRC_OPS_EIGEN_MAT_PROD_VECTOR_HPP_
#define OPS_SRC_OPS_EIGEN_MAT_PROD_VECTOR_HPP_

namespace pressio{ namespace ops{

/*
 * y = beta * y + alpha*op(A)*x
 *
*/

//-------------------------------
// specialize for op(A) = A
//-------------------------------
template < typename A_type, typename x_type, typename scalar_type, typename y_type>
::pressio::mpl::enable_if_t<
  (containers::meta::is_multi_vector_wrapper_eigen<A_type>::value or
   containers::meta::is_matrix_wrapper_eigen<A_type>::value) and
  containers::meta::is_vector_wrapper_eigen<x_type>::value and
  containers::meta::is_vector_wrapper_eigen<y_type>::value
  >
product(::pressio::nontranspose mode,
	const scalar_type alpha,
	const A_type & A,
	const x_type & x,
	const scalar_type beta,
	y_type & y)
{
  static_assert(containers::meta::are_scalar_compatible<A_type, x_type, y_type>::value,
		"Types are not scalar compatible");

  assert( y.extent(0) == A.extent(0) );
  assert( x.extent(0) == A.extent(1) );
  const auto & AE = *A.data();
  const auto & xE = *x.data();
  auto & yE = *y.data();

  yE = beta * yE + alpha * AE * xE;
}

//-------------------------------
// specialize for op(A) = A^T
//-------------------------------
template < typename A_type, typename x_type, typename scalar_type, typename y_type>
::pressio::mpl::enable_if_t<
  (containers::meta::is_multi_vector_wrapper_eigen<A_type>::value or
   containers::meta::is_matrix_wrapper_eigen<A_type>::value) and
  containers::meta::is_vector_wrapper_eigen<x_type>::value and
  containers::meta::is_vector_wrapper_eigen<y_type>::value
  >
product(::pressio::transpose mode,
	const scalar_type alpha,
	const A_type & A,
	const x_type & x,
	const scalar_type beta,
	y_type & y)
{
  static_assert(containers::meta::are_scalar_compatible<A_type, x_type, y_type>::value,
		"Types are not scalar compatible");

  assert( y.extent(0) == A.extent(1) );
  assert( x.extent(0) == A.extent(0) );

  auto & yE	  = *y.data();
  const auto & xE = *x.data();
  const auto & AE = *A.data();
  yE = beta * yE + alpha * AE.transpose() * xE;
}

}}//end namespace pressio::ops
#endif
