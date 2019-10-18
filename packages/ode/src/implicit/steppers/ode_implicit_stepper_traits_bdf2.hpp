/*
//@HEADER
// ************************************************************************
//
// ode_implicit_stepper_traits_bdf2.hpp
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

#ifndef ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_BDF2_HPP_
#define ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_BDF2_HPP_

#include "ode_implicit_stepper_traits_helpers.hpp"

namespace pressio{ namespace ode{ namespace details{

template<
  typename state_type,
  typename residual_type,
  typename jacobian_type,
  typename system_type,
  typename ...Args
  >
struct traits<
  ImplicitStepper<
    ImplicitEnum::BDF2,
    state_type, residual_type,
    jacobian_type, system_type,
    Args...>
  > {

  using stepper_t =   ImplicitStepper< ImplicitEnum::BDF2,
				       state_type, residual_type,
				       jacobian_type, system_type,
				       Args...>;
  using this_t = traits<stepper_t>;

  static constexpr ode::ImplicitEnum enum_id = ode::ImplicitEnum::BDF2;
  static constexpr bool is_implicit = true;
  static constexpr bool is_explicit = false;

  using state_t	   = state_type;
  using residual_t = residual_type;
  using jacobian_t = jacobian_type;
  using system_t   = system_type;

  //----------------------------------------------------------------
  // do some checks on the system type
  //----------------------------------------------------------------
  static_assert(::pressio::containers::meta::has_scalar_typedef<system_t>::value,
		"\nThe model type you passed to the BDF2 implicit stepper \n \
does not have a valid public scalar_type typedef. Define it inside your class as: \n \
using scalar_type = ...; ");

  static_assert(::pressio::ode::meta::has_state_typedef<system_t>::value,
		"\nThe model type you passed to the BDF2 implicit stepper \n \
does not have a valid public state_type typedef. Define it inside your class as: \n \
using state_type = ...; ");

  static_assert(::pressio::ode::meta::has_velocity_typedef<system_t>::value,
		"\nThe model type you passed to the BDF2 implicit stepper \n \
does not have a valid public velocity_type typedef. Define it inside your class as: \n \
using velocity_type = ...; ");

  static_assert(::pressio::ode::meta::has_jacobian_typedef<system_t>::value,
		"\nThe model type you passed to the BDF2 implicit stepper \n \
does not have a valid public jacobian_type typedef. Define it inside your class as: \n \
using jacobian_type = ...; ");

  static_assert(::pressio::ode::meta::model_has_needed_velocity_methods<
		system_t, typename system_t::state_type, typename system_t::velocity_type, typename system_t::scalar_type
		>::value,
		"\nThe model type you passed to the BDF2 implicit stepper \n \
does not have valid velocity methods, see api for reference.");


  static constexpr types::stepper_order_t order_value = 2;
  static constexpr types::stepper_n_states_t numAuxStates = 2;

  // check if scalar is provided in Args
  using ic0 = ::pressio::mpl::variadic::find_if_unary_pred_t<std::is_floating_point, Args...>;
  using scalar_from_args = ::pressio::mpl::variadic::at_or_t<void, ic0::value, Args...>;
  // check if state is a containers wrapper, and if so get its scalar_type
  using scalar_type_from_traits = typename impl::ScalarHelper<state_type>::type;
  // decide which to pick
  using scalar_t = typename std::conditional<
    std::is_void<scalar_from_args>::value,
    scalar_type_from_traits, scalar_from_args>::type;

  static_assert( std::is_floating_point<scalar_t>::value,
  		 "I cannot determine the scalar_type because it is not found \
in the templates args and the state_type used is not a containers wrapper. \
If you are using custom data structures that do not have wrappers in \
the containers, pass scalar as a template.");

  // for BDF2 the user has to pass an auxiliary stepper
  using ic1 = ::pressio::mpl::variadic::find_if_binary_pred_t<
    stepper_t, ::pressio::ode::meta::is_legitimate_auxiliary_stepper, Args...>;
  using aux_stepper_t = ::pressio::mpl::variadic::at_or_t<void, ic1::value, Args...>;

  // standard policies (only used if user-defined policies not passed)
  using policy_picker = impl::StdPoliciesPicker<system_t, state_t, residual_t, jacobian_t>;
  using standard_res_policy_t = typename policy_picker::standard_res_policy_t;
  using standard_jac_policy_t = typename policy_picker::standard_jac_policy_t;

  // check Args if a user-defined admissible residual policy is passed
  using ic2 = ::pressio::ode::meta::find_if_legitimate_implicit_residual_policy_t<
    this_t::enum_id, this_t::numAuxStates, state_t, residual_t, system_t, scalar_t,
    Args...>;
  using residual_policy_t = ::pressio::mpl::variadic::at_or_t
    <standard_res_policy_t, ic2::value, Args...>;

  // check Args if a user-defined admissible jacobian policy is passed
  using ic3 = ::pressio::ode::meta::find_if_legitimate_implicit_jacobian_policy_t<
    this_t::enum_id, state_t, jacobian_t, system_t, scalar_t,
    Args...>;
  using jacobian_policy_t = ::pressio::mpl::variadic::at_or_t
    <standard_jac_policy_t, ic3::value, Args...>;

  // we only need to check that model has jacobian methods when the policy is
  // the standard one, since for non-standard the user might be doing things differently
  using check1 = impl::CheckModelJacobianMethods<
    system_t, std::is_same<jacobian_policy_t, standard_jac_policy_t>::value>;
};

}}}//end namespace pressio::ode::details
#endif
