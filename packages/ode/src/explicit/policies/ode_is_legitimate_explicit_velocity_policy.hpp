/*
//@HEADER
// ************************************************************************
//
// ode_is_legitimate_explicit_velocity_policy.hpp
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

#ifndef ODE_EXPLICIT_POLICIES_IS_LEGITIMATE_EXPLICIT_VELOCITY_POL_HPP_
#define ODE_EXPLICIT_POLICIES_IS_LEGITIMATE_EXPLICIT_VELOCITY_POL_HPP_

namespace pressio{ namespace ode{ namespace meta {

template<
  typename T,
  typename scalar_t, typename state_t, typename velocity_t, typename model_t,
  typename enable = void
  >
struct is_legitimate_explicit_velocity_policy
  : std::false_type{};

template<
  typename T,
  typename scalar_t, typename state_t, typename velocity_t, typename model_t
  >
struct is_legitimate_explicit_velocity_policy<
  T, scalar_t, state_t, velocity_t, model_t,
  mpl::enable_if_t<
    std::is_void<
      decltype
      (
       std::declval<T const>().template operator()
       (
	std::declval<state_t const &>(),
	std::declval<velocity_t &>(),
	std::declval<model_t const &>(),
	std::declval<scalar_t const &>()
	)
       )
      >::value
    and
    std::is_same<
      decltype
      (
       std::declval<T const>().template operator()
       (
	std::declval<state_t const &>(),
	std::declval<model_t const &>(),
	std::declval<scalar_t const &>()
	)
       ),
      velocity_t
      >::value
    >
  > : std::true_type{};


}}}//end namespace pressio::containers::meta
#endif
