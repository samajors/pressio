/*
//@HEADER
// ************************************************************************
//
// ode_is_valid_user_defined_ops_for_implicit_euler.hpp
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

#ifndef ODE_IS_VALID_USER_DEFINED_OPS_IMPLICIT_EULER_HPP_
#define ODE_IS_VALID_USER_DEFINED_OPS_IMPLICIT_EULER_HPP_

namespace pressio{ namespace ode{ namespace meta {

template<
  typename T,
  typename scalar_t,
  typename state_t,
  typename residual_t,
  typename jacobian_t,
  typename enable = void
  >
struct is_valid_user_defined_ops_for_implicit_euler : std::false_type{};

template<
  typename T,
  typename scalar_t,
  typename state_t,
  typename residual_t,
  typename jacobian_t
  >
struct is_valid_user_defined_ops_for_implicit_euler<
  T, scalar_t, state_t, residual_t, jacobian_t,
    mpl::enable_if_t<
      ::pressio::ops::meta::has_method_do_update_two_terms<
	typename T::update_op,
	scalar_t,
	typename containers::details::traits<residual_t>::wrapped_t,
	typename containers::details::traits<state_t>::wrapped_t,
	typename containers::details::traits<state_t>::wrapped_t
	>::value
      and
      ::pressio::ops::meta::has_method_scale<
	typename T::update_op,
	typename containers::details::traits<jacobian_t>::wrapped_t,
	scalar_t
	>::value
      and
      ::pressio::ops::meta::has_method_add_to_diagonal<
	typename T::update_op,
	typename containers::details::traits<jacobian_t>::wrapped_t,
	scalar_t
	>::value
      >
  > : std::true_type{};

}}} // namespace pressio::ode::meta
#endif
