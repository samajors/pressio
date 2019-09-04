/*
//@HEADER
// ************************************************************************
//
// containers_norm2_vector.hpp
//                     		      Pressio 
// Copyright 2019 National Technology & Engineering Solutions of Sandia,LLC 
//							      (NTESS)
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

#ifndef CONTAINERS_CONTAINER_OPS_NORMS_NORM2_VECTOR_HPP_
#define CONTAINERS_CONTAINER_OPS_NORMS_NORM2_VECTOR_HPP_

#include "../containers_ops_meta.hpp"
#include "../../vector/containers_vector_meta.hpp"
#ifdef HAVE_KOKKOS
#include "KokkosBlas1_nrm2.hpp"
#endif

namespace pressio{ namespace containers{ namespace ops{

#ifdef HAVE_TRILINOS
//  epetra vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_vector_wrapper_epetra<vec_type>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::scalar_t
{
  using sc_t = typename details::traits<vec_type>::scalar_t;
  sc_t result = 0.0;
  a.data()->Norm2(&result);
  return result;
}

//  tpetra vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_vector_wrapper_tpetra<vec_type>::value &&
    std::is_same<typename details::traits<vec_type>::scalar_t,
		 typename details::traits<vec_type>::mag_t>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::mag_t
{
  return a.data()->norm2();
}

//  block tpetra vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_vector_wrapper_tpetra_block<vec_type>::value &&
    std::is_same<typename details::traits<vec_type>::scalar_t,
		 typename details::traits<vec_type>::mag_t>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::mag_t
{
  /* workaround the non-constness of getVectorView,
   * which is supposed to be const but it is not */
  using mv_t = Tpetra::Experimental::BlockVector<>;
  return const_cast<mv_t*>(a.data())->getVectorView().norm2();
}

//  teuchos serial dense vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_dense_vector_wrapper_teuchos<vec_type>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::scalar_t
{
  using sc_t = typename details::traits<vec_type>::scalar_t;
  sc_t result = 0.0;
  for (decltype(a.size()) i=0; i<a.size(); i++)
    result += a[i]*a[i];
  return std::sqrt(result);
}
#endif


#ifdef HAVE_KOKKOS
//  kokkos vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_vector_wrapper_kokkos<vec_type>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::scalar_t
{
  return KokkosBlas::nrm2(*a.data());
}
#endif
//--------------------------------------------------------


//  eigen vector wrapper
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_vector_wrapper_eigen<vec_type>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename details::traits<vec_type>::scalar_t
{
  using sc_t = typename details::traits<vec_type>::scalar_t;
  sc_t result = 0.0;
  for (decltype(a.size()) i=0; i<a.size(); i++)
    result += a[i]*a[i];
  return std::sqrt(result);
}
//--------------------------------------------------------

#ifdef HAVE_PYBIND11
// pybind11::array
template <typename vec_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<vec_type>::value
    > * = nullptr
  >
auto norm2(const vec_type & a)
  -> typename vec_type::value_type
{
  using sc_t = typename vec_type::value_type;
  sc_t result = 0.0;

  // make sure this is a vector
  if (a.ndim() > 1){
    throw std::runtime_error("a.ndims()!=1, this norm op is for pybind11 vectors");
  }

  for (decltype(a.size()) i=0; i<a.size(); i++)
    result += a.at(i)*a.at(i);
  return std::sqrt(result);
}
#endif
//--------------------------------------------------------

}}}//end namespace pressio::containers::ops
#endif
