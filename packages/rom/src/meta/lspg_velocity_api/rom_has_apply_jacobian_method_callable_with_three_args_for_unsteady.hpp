/*
//@HEADER
// ************************************************************************
//
// rom_has_apply_jacobian_method_callable_with_three_args_for_unsteady.hpp
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

#ifndef ROM_HAS_APPLY_JACOBIAN_METHOD_CALLABLE_WITH_THREE_ARGS_FOR_UNSTEADY_HPP_
#define ROM_HAS_APPLY_JACOBIAN_METHOD_CALLABLE_WITH_THREE_ARGS_FOR_UNSTEADY_HPP_

namespace pressio{ namespace rom{ namespace meta {

template <
  typename T,
  typename state_t,
  typename sc_t,
  typename dense_mat_t,
  typename = void
  >
struct has_apply_jacobian_method_callable_with_three_args_for_unsteady
  : std::false_type{};

template <
  typename T,
  typename state_t,
  typename sc_t,
  typename dense_mat_t
  >
struct has_apply_jacobian_method_callable_with_three_args_for_unsteady<
  T, state_t, sc_t, dense_mat_t,
  ::pressio::mpl::enable_if_t<
    !std::is_void<dense_mat_t>::value and
    mpl::is_same<
      dense_mat_t,
      decltype(
	       std::declval<T const>().applyJacobian(
						     std::declval<state_t const &>(),
						     std::declval<dense_mat_t const &>(),
						     std::declval<sc_t const &>()
						     )
	       )
      >::value
    >
  > : std::true_type{};

}}} // namespace pressio::rom::meta
#endif
