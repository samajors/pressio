/*
//@HEADER
// ************************************************************************
//
// ode_implicit_stepper_traits_arbitrary.hpp
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

#ifndef ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_ARBITRARY_HPP_
#define ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_ARBITRARY_HPP_

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
    ImplicitEnum::Arbitrary,
    state_type, residual_type,
    jacobian_type, system_type, Args...>
  > {

  using stepper_t =   ImplicitStepper< ImplicitEnum::Arbitrary,
				       state_type, residual_type,
				       jacobian_type, system_type, Args...>;
  using this_t = traits<stepper_t>;

  static constexpr ode::ImplicitEnum enum_id = ode::ImplicitEnum::Arbitrary;
  static constexpr bool is_implicit = true;
  static constexpr bool is_explicit = false;

  using state_t		  = state_type;
  using residual_t	  = residual_type;
  using jacobian_t	  = jacobian_type;
  using system_t	  = system_type;
  using aux_stepper_t	  = void;

  //----------------------------------------------------------------
  // do some checks on the system type
  //----------------------------------------------------------------
  static_assert(::pressio::containers::meta::has_scalar_typedef<system_t>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have a valid public scalar_type typedef. Define it inside your class as: \n \
using scalar_type = ...; ");

  static_assert(::pressio::ode::meta::has_state_typedef<system_t>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have a valid public state_type typedef. Define it inside your class as: \n \
using state_type = ...; ");

  static_assert(::pressio::ode::meta::has_residual_typedef<system_t>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have a valid public residual_type typedef. Define it inside your class as: \n \
using residual_type = ...; ");

  static_assert(::pressio::ode::meta::has_jacobian_typedef<system_t>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have a valid public jacobian_type typedef. Define it inside your class as: \n \
using jacobian_type = ...; ");

  static_assert(::pressio::ode::meta::has_needed_time_discrete_residual_methods<
		system_t, types::step_t,
		typename system_t::scalar_type,
		typename system_t::state_type, typename system_t::residual_type
		>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have valid velocity methods, see api for reference.");

  static_assert(::pressio::ode::meta::has_needed_time_discrete_jacobian_methods<
		system_t, types::step_t,
		typename system_t::scalar_type,
		typename system_t::state_type, typename system_t::jacobian_type
		>::value,
		"\nThe model type you passed to the Arbitrary implicit stepper \n \
does not have valid jacobian methods, see api for reference.");


  //-------------------------------
  // find the order setter in Args
  //-------------------------------
  using ic0 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::ode::meta::impl::IsStepperOrderSetter, Args...>;
  using order_setter = ::pressio::mpl::variadic::at_or_t<void, ic0::value, Args...>;
  static_assert( !std::is_void<order_setter>::value,
  		 "For the Arbitrary implicit stepper, you need to set the order at\
compile time by passing to the ImplicitStepper a template argument as follows: \
::pressio::ode::types::StepperOrder<your_order_value>.");
  // store
  static constexpr types::stepper_order_t order_value = order_setter::value;


  //-----------------------------------------------------------
  // find the total number of states needed
  //-----------------------------------------------------------
  using ic1 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::ode::meta::impl::IsStepperTotalNumStatesSetter, Args...>;
  using tot_n_setter = ::pressio::mpl::variadic::at_or_t<void, ic1::value, Args...>;
  static_assert( !std::is_void<tot_n_setter>::value,
  		 "For the Arbitrary implicit stepper, you need to set the \
total number of states needed for the stepper at compile time by passing \
to the ImplicitStepper a template argument as follows: \
::pressio::ode::types::StepperTotalNumberOfStates<your_order_value>. \
Note that this is the total number of states needed including previous ones, \
basically the size of the stpper stencil.");
  // numAuxStates is the number of auxiliary states needed, so all other beside y_n
  static constexpr types::stepper_n_states_t numAuxStates = tot_n_setter::value - 1;


  //-----------------------------------------------------------
  // standard policies (only used if not passed a user-defined policy)
  //-----------------------------------------------------------
  using standard_res_policy_t
  = policy::ImplicitResidualStandardPolicyForArbitraryStepper<state_t, system_t, residual_t>;
  using standard_jac_policy_t
  = policy::ImplicitJacobianStandardPolicyForArbitraryStepper<state_t, system_t, jacobian_t>;


  //-----------------------------------------------------------
  // check if scalar is provided in Args
  //-----------------------------------------------------------
  using ic2 = ::pressio::mpl::variadic::find_if_unary_pred_t<std::is_floating_point, Args...>;
  using scalar_from_args = ::pressio::mpl::variadic::at_or_t<void, ic2::value, Args...>;
  // check if state is a containers wrapper, and if so get its scalar_type
  using scalar_type_from_traits = typename impl::ScalarHelper<state_type>::type;
  // decide which to pick
  using scalar_t = typename std::conditional<std::is_void<scalar_from_args>::value,
					     scalar_type_from_traits,
					     scalar_from_args>::type;

  static_assert( std::is_floating_point<scalar_t>::value,
  		 "I cannot determine the scalar_type because it is not found \
in the templates args and the state_type used is not a containers' wrapper. \
If you are using custom data structures that are not wrappeed in containers types, \
pass a valid scalar type as a template argument to ImplicitStepper.");


  //-----------------------------------------------------------
  // check Args for a user-defined admissible residual policy
  //-----------------------------------------------------------
  using ic3 = ::pressio::ode::meta::find_if_legitimate_residual_policy_for_implicit_arbitrary_stepper_t<
    numAuxStates, state_t, residual_t, system_t, scalar_t, Args...>;
  using residual_policy_t = ::pressio::mpl::variadic::at_or_t<standard_res_policy_t,
							      ic3::value, Args...>;

  // check Args for a user-defined admissible jacobian policy
  using ic4 = ::pressio::ode::meta::find_if_legitimate_jacobian_policy_for_implicit_arbitrary_stepper_t<
    numAuxStates, state_t, jacobian_t, system_t, scalar_t, Args...>;
  using jacobian_policy_t = ::pressio::mpl::variadic::at_or_t<standard_jac_policy_t,
							      ic4::value, Args...>;

  // make sure we have non-void policies
  static_assert( !std::is_void<residual_policy_t>::value and
		 !std::is_void<jacobian_policy_t>::value,
  		 "If you want to use an arbitrary implicit stepper, \
i.e. when ode::ImplicitEnum::Arbitrary, you must provide user-defined \
residual and jacobian policies which are in charge of computing the \
time discrete residual and its jacobian. ");


//   // we need to make sure that the residual and jac policies are not standard policies
//   using policy_picker = impl::StdPoliciesPicker<system_t, state_t, residual_t, jacobian_t>;
//   using standard_res_policy_t = typename policy_picker::standard_res_policy_t;
//   using standard_jac_policy_t = typename policy_picker::standard_jac_policy_t;
//   static_assert( mpl::not_same<residual_policy_t, residual_policy_t>::value and
// 		 mpl::not_same<jacobian_policy_t, jacobian_policy_t>::value ,
//   		 "You cannot use standard policies to an arbitrary implicit stepper \
// i.e. when ode::ImplicitEnum::Arbitrary, you must provide user-defined \
// residual and jacobian policies which are in charge of computing the \
// time discrete residual and its jacobian. ");

};

}}}//end namespace pressio::ode::details
#endif
