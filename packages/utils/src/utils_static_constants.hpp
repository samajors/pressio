/*
//@HEADER
// ************************************************************************
//
// utils_static_constants.hpp
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

#ifndef UTILS_STATIC_CONSTANTS_HPP_
#define UTILS_STATIC_CONSTANTS_HPP_

namespace pressio{ namespace utils{

struct constants{

  // this is typically used as a template parameter
  // a positive quantity (e.g., a size of a vector)
  // is not known at compile-time, and the value
  // is defined at runtime
  static constexpr int dynamic = -1;

  template <typename scalar_t = double>
  static constexpr scalar_t negOne(){
    return static_cast<scalar_t>(-1);
  }

  template <typename scalar_t = double>
  static constexpr scalar_t zero(){
    return static_cast<scalar_t>(0);
  }

  template <typename scalar_t = double>
  static constexpr scalar_t one(){
    return static_cast<scalar_t>(1);
  }

  template <typename scalar_t = double>
  static constexpr scalar_t two(){
    return static_cast<scalar_t>(2);
  }

  template <typename scalar_t = double>
  static constexpr scalar_t three(){
    return static_cast<scalar_t>(3);
  }

  template <typename scalar_t = double>
  static constexpr scalar_t four(){
    return static_cast<scalar_t>(4);
  }

};

}} // end of namespace pressio::utils
#endif
