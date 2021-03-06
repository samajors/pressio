/*
//@HEADER
// ************************************************************************
//
// ops_mvec_prod_vec.hpp
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
#ifndef OPS_SRC_OPS_TPETRA_MULTI_VECTOR_PROD_VECTOR_HPP_
#define OPS_SRC_OPS_TPETRA_MULTI_VECTOR_PROD_VECTOR_HPP_

#include "Tpetra_idot.hpp"
#include <KokkosBlas1_axpby.hpp>
#include "KokkosBlas2_gemv.hpp"

namespace pressio{ namespace ops{


/*
 * multi_vector prod vector
 *
 * y = beta * y + alpha*op(A)*x
 *
*/

// begin namespace pressio::ops::impl
namespace impl{

template <typename A_type, typename x_type, typename y_type, typename scalar_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<y_type>::value
  >
_product_tpetra_mv_sharedmem_vec(const scalar_type alpha,
				 const A_type & A,
				 const x_type & x,
				 const scalar_type beta,
				 y_type & y)
{
  assert(size_t(A.numVectors()) == size_t(x.extent(0)));

  using kokkos_view_t = Kokkos::View<const scalar_type*, Kokkos::HostSpace,
				     Kokkos::MemoryTraits<Kokkos::Unmanaged> >;
  kokkos_view_t xview(x.data()->data(), x.extent(0));

  const auto ALocalView_h = A.data()->getLocalViewHost();
  const auto yLocalView_h = y.data()->getLocalViewHost();
  const char ctA = 'N';
  // Tpetra::Vector is implemented as a special case of MultiVector //
  // so getLocalView returns a rank-2 view so in order to get
  // view with rank==1 I need to explicitly get the subview of that
  const auto yLocalView_drank1 = Kokkos::subview(yLocalView_h, Kokkos::ALL(), 0);
  KokkosBlas::gemv(&ctA, alpha, ALocalView_h, xview, beta, yLocalView_drank1);
}


// when the operand is a kokkos wrapper we use kokkos functionalities directly
template <typename A_type, typename x_type, typename y_type, typename scalar_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<y_type>::value
  >
_product_tpetra_mv_sharedmem_vec_kokkos(const scalar_type alpha,
					     const A_type & A,
					     const x_type & x,
					     const scalar_type beta,
					     y_type & y)
{
  // make sure the tpetra mv has same exe space of the kokkos vector wrapper
  using tpetra_mv_dev_t = typename ::pressio::containers::details::traits<A_type>::device_t;
  using kokkos_v_dev_t  = typename ::pressio::containers::details::traits<x_type>::device_type;
  static_assert( std::is_same<tpetra_mv_dev_t, kokkos_v_dev_t>::value,
		 "product: tpetra MV and kokkos wrapper need to have same device type" );

  assert( (std::size_t)A.numVectors() == x.data()->extent(0) );
  const char ctA = 'N';
  const auto ALocalView_d = A.data()->getLocalViewDevice();

  // Tpetra::Vector is implemented as a special case of MultiVector //
  // so getLocalView returns a rank-2 view so in order to get
  // view with rank==1 I need to explicitly get the subview of that
  const auto yLocalView_drank2 = y.data()->getLocalViewDevice();
  const auto yLocalView_drank1 = Kokkos::subview(yLocalView_drank2, Kokkos::ALL(), 0);
  KokkosBlas::gemv(&ctA, alpha, ALocalView_d, *x.data(), beta, yLocalView_drank1);
}
}//end namespace pressio::ops::impl


/* -------------------------------------------------------------------
 * x is a sharedmem vector wrapper
 *-------------------------------------------------------------------*/
template < typename A_type, typename x_type, typename scalar_type, typename y_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<y_type>::value and
  !containers::meta::is_vector_wrapper_kokkos<x_type>::value
  >
product(::pressio::nontranspose mode,
	const scalar_type alpha,
	const A_type & A,
	const ::pressio::containers::VectorSharedMemBase<x_type> & x,
	const scalar_type beta,
	y_type & y)
{
  static_assert(containers::meta::are_scalar_compatible<A_type, x_type, y_type>::value,
    "Types are not scalar compatible");

  ::pressio::ops::impl::_product_tpetra_mv_sharedmem_vec(alpha, A, x, beta, y);
}

template < typename A_type, typename x_type, typename scalar_type, typename y_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<y_type>::value and
  containers::meta::is_vector_wrapper_kokkos<x_type>::value
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

  ::pressio::ops::impl::_product_tpetra_mv_sharedmem_vec_kokkos(alpha, A, x, beta, y);
}

/* -------------------------------------------------------------------
 * x is a distributed Tpetra vector wrapper
 *-------------------------------------------------------------------*/

// y = wrapper of Kokkos vector
template <typename A_type, typename x_type, typename y_type, typename scalar_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<x_type>::value and
  containers::meta::is_vector_wrapper_kokkos<y_type>::value
  >
product(::pressio::transpose mode,
	const scalar_type alpha,
	const A_type & A,
	const x_type & x,
	const scalar_type beta,
	y_type & y)
{
  static_assert(containers::meta::are_scalar_compatible<A_type, x_type, y_type>::value,
		"Tpetra MV dot V: operands do not have matching scalar type");

  static_assert(std::is_same<
		typename containers::details::traits<A_type>::device_t,
		typename containers::details::traits<x_type>::device_t>::value,
		"Tpetra MV dot V: operands do not have the same device type");

  static_assert(std::is_same<
		typename containers::details::traits<x_type>::device_t,
		typename containers::details::traits<y_type>::device_t>::value,
		"Tpetra MV dot V: V and result do not have the same device type");

  using kokkos_v_t = typename ::pressio::containers::details::traits<y_type>::wrapped_t;
  using v_t = ::pressio::containers::Vector<kokkos_v_t>;
  v_t ATx(y.extent(0));
  auto request = Tpetra::idot(*ATx.data(), *A.data(), *x.data());
  request->wait();
  KokkosBlas::axpby(alpha, *ATx.data(), beta, *y.data());
}

// y = sharedmem vec not kokkos
template <typename A_type, typename x_type, typename y_type, typename scalar_type>
::pressio::mpl::enable_if_t<
  containers::meta::is_multi_vector_wrapper_tpetra<A_type>::value and
  containers::meta::is_vector_wrapper_tpetra<x_type>::value and
  !containers::meta::is_vector_wrapper_kokkos<y_type>::value
  >
product(::pressio::transpose mode,
	const scalar_type alpha,
	const A_type & A,
	const x_type & x,
	const scalar_type beta,
	::pressio::containers::VectorSharedMemBase<y_type> & y)
{
  // dot product of each vector in A with vecB
  /* Apparently, trilinos does not support this...
     the native dot() method of multivectors is only for
     dot product of two multivectors with same # of columns.
     So we have to extract each column vector
     from A and do dot product one a time*/

  static_assert(containers::meta::are_scalar_compatible<A_type, x_type, y_type>::value,
		"Types are not scalar compatible");

  const auto numVecs = A.extent(1);
  for (std::size_t i=0; i<(std::size_t)numVecs; i++){
    // colI is a Teuchos::RCP<Vector<...>>
    const auto colI = A.data()->getVector(i);
    y[i] = beta * y[i] + alpha * colI->dot(*x.data());
  }
}

}}//end namespace pressio::ops
#endif
#endif
