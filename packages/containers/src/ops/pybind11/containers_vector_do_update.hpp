/*
//@HEADER
// ************************************************************************
//
// containers_vector_do_update.hpp
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

#ifdef PRESSIO_ENABLE_TPL_PYBIND11
#ifndef CONTAINERS_SRC_OPS_PYBIND11_VECTOR_DO_UPDATE_HPP_
#define CONTAINERS_SRC_OPS_PYBIND11_VECTOR_DO_UPDATE_HPP_

#include "../containers_ops_meta.hpp"
#include "../../vector/containers_vector_meta.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace pressio{ namespace containers{ namespace ops{


//----------------------------------------------------------------------
// computing:  V = a * V + b * V1
//----------------------------------------------------------------------
template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v, scalar_t a,
	       const T & v1, scalar_t b){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = a*v.at(i) + b*v1.at(i);
  }
}


template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v, const T & v1, const scalar_t b){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = b*v1.at(i);
  }
}



//----------------------------------------------------------------------
//  overloads for computing this: V = a * V + b * V1 + c * V2
//----------------------------------------------------------------------
template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v, const scalar_t &a,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c){
  std::cout << " do_update pybind11 two terms " << std::endl;
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }
  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = a*v.at(i) + b*v1.at(i) + c*v2.at(i);
  }
}


template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = b*v1.at(i) + c*v2.at(i);
  }
}



//----------------------------------------------------------------------
//  overloads for computing:
//	V = a * V + b * V1 + c * V2 + d * V3
//----------------------------------------------------------------------
template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v, const scalar_t &a,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c,
	       const T & v3, const scalar_t &d){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size() and vsz != v3.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = a*v.at(i) + b*v1.at(i) + c*v2.at(i) + d*v3.at(i);
  }
}


template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c,
	       const T & v3, const scalar_t &d){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size() and vsz != v3.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = b*v1.at(i) + c*v2.at(i) + d*v3.at(i);
  }
}



//----------------------------------------------------------------------
//  overloads for computing:
//	V = a * V + b * V1 + c * V2 + d * V3 + e * V4
//----------------------------------------------------------------------
template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v, const scalar_t &a,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c,
	       const T & v3, const scalar_t &d,
	       const T & v4, const scalar_t &e){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size()
      and vsz != v3.size() and vsz != v4.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = a*v.at(i) + b*v1.at(i) + c*v2.at(i) + d*v3.at(i) + e*v4.at(i);
  }
}


template<
  typename T,
  typename scalar_t,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<T>::value
    > * = nullptr
  >
void do_update(T & v,
	       const T & v1, const scalar_t &b,
	       const T & v2, const scalar_t &c,
	       const T & v3, const scalar_t &d,
	       const T & v4, const scalar_t &e){
  // make sure this is a vector
  if (v.ndim() > 1){
    throw std::runtime_error("containers::ops::do_update: v.ndims()!=1, while this operation requires a vector");
  }

  const auto vsz = v.size();
  if (vsz != v1.size() and vsz != v2.size()
      and vsz != v3.size() and vsz != v4.size())
    throw std::runtime_error("containers::ops::do_update: Input shapes must match");

  for (decltype(v.size()) i=0; i<vsz; ++i){
    v.mutable_at(i) = b*v1.at(i) + c*v2.at(i) + d*v3.at(i) + e*v4.at(i);
  }
}


}}}//end namespace pressio::containers::ops
#endif
#endif