/*
//@HEADER
// ************************************************************************
//
// rom_model_meets_velocity_api_for_wls.hpp
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

#ifndef ROM_MODEL_MEETS_VELOCITY_API_FOR_WLS_HPP_
#define ROM_MODEL_MEETS_VELOCITY_API_FOR_WLS_HPP_

namespace pressio{ namespace rom{ namespace meta {

template<typename T, typename enable = void>
struct model_meets_velocity_api_for_wls : std::false_type{};

template<typename T>
struct model_meets_velocity_api_for_wls<
  T,
  mpl::enable_if_t<
    ::pressio::containers::meta::has_scalar_typedef<T>::value and
    ::pressio::ode::meta::has_state_typedef<T>::value and
    ::pressio::ode::meta::has_velocity_typedef<T>::value and
    ::pressio::ode::meta::has_jacobian_typedef<T>::value and
    ::pressio::rom::meta::has_dense_matrix_typedef<T>::value and
    ::pressio::rom::meta::model_has_needed_velocity_methods<
      T,
      typename T::state_type,
      typename T::velocity_type,
      typename T::scalar_type
      >::value and
    ::pressio::rom::meta::model_has_needed_apply_jacobian_methods_for_unsteady<
      T,
      typename T::state_type,
      typename T::scalar_type,
      typename T::dense_matrix_type
      >::value
    >
  > : std::true_type{};


}}} // namespace pressio::rom::meta
#endif
