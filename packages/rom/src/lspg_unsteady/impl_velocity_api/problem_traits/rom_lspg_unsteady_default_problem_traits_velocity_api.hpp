/*
//@HEADER
// ************************************************************************
//
// rom_lspg_unsteady_default_problem_traits_velocity_api.hpp
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

#ifndef ROM_LSPG_UNSTEADY_DEFAULT_PROBLEM_TRAITS_VELOCITY_API_HPP_
#define ROM_LSPG_UNSTEADY_DEFAULT_PROBLEM_TRAITS_VELOCITY_API_HPP_

#include "../rom_lspg_unsteady_residual_policy_velocity_api.hpp"
#include "../rom_lspg_unsteady_jacobian_policy_velocity_api.hpp"
#include "rom_lspg_unsteady_common_traits_velocity_api.hpp"

namespace pressio{ namespace rom{ namespace lspg{ namespace unsteady{ namespace impl{

template <
  typename stepper_tag,
  typename fom_type,
  typename lspg_state_type,
  typename ... Args
  >
struct DefaultProblemTraitsVelocityApi{

  static_assert( !std::is_same< stepper_tag,  ::pressio::ode::implicitmethods::Arbitrary>::value,
		 "\nTo use unsteady LSPG with the velocity api, \n \
you cannot pass ode::implicitmethods::Arbitrary since that is only \n \
valid when using the residual api. For the velocity api you need \n \
to pass a valid tag from the ode steppers, like implicitmethods::Euler/BDF2");

  /* here, the fom_type must satisfy the velocity api */
  static_assert( ::pressio::rom::meta::model_meets_velocity_api_for_unsteady_lspg<fom_type>::value,
		 "\nUsing DefaultProblemTypeGeneratorVelocityApi \n \
requires a fom adapter class that meets the velocity api. \n \
However, the fom/adapter type you passed does not meet the velocity api. \n \
Verify the fom/adapter class you are using meets the velocity api.");

  // pick the common types holder
  using common_types_t = LSPGUnsteadyCommonTraitsVelocityApi<stepper_tag, fom_type, lspg_state_type, Args...>;

  using fom_t			= typename common_types_t::fom_t;
  using scalar_t		= typename common_types_t::scalar_t;
  using fom_native_state_t	= typename common_types_t::fom_native_state_t;
  using fom_state_t		= typename common_types_t::fom_state_t;
  using fom_velocity_t		= typename common_types_t::fom_velocity_t;
  using lspg_state_t		= typename common_types_t::lspg_state_t;
  using lspg_residual_t		= typename common_types_t::lspg_residual_t;
  using decoder_t		= typename common_types_t::decoder_t;
  using decoder_jac_t		= typename common_types_t::decoder_jac_t;
  using lspg_matrix_t		= typename common_types_t::lspg_matrix_t;
  using fom_state_reconstr_t	= typename common_types_t::fom_state_reconstr_t;
  using fom_states_data		= typename common_types_t::fom_states_data;
  using ud_ops_t		= typename common_types_t::ud_ops_t;

  // policy for evaluating the rhs of the fom object (<false> for unsteady overload)
  using fom_eval_velocity_policy_t	= ::pressio::rom::policy::QueryFomVelocityDefault<false>;

  // policy for left multiplying the fom jacobian with decoder_jac_t
  // possibly involving other stuff like explained above (<false> for unsteady overload)
  using fom_apply_jac_policy_t	= ::pressio::rom::policy::QueryFomApplyJacobianDefault<false>;

  // policy defining how to compute the LSPG time-discrete residual
  using lspg_residual_policy_t	= ::pressio::rom::lspg::unsteady::impl::ResidualPolicyVelocityApi<
    lspg_residual_t, fom_states_data, fom_eval_velocity_policy_t, ud_ops_t>;

  // policy defining how to compute the LSPG time-discrete jacobian
  using lspg_jacobian_policy_t	= ::pressio::rom::lspg::unsteady::impl::JacobianPolicyVelocityApi<
    fom_states_data, lspg_matrix_t, fom_apply_jac_policy_t, decoder_t, ud_ops_t>;

  using aux_stepper_t = typename ::pressio::rom::lspg::unsteady::impl::auxStepperHelper<
    stepper_tag, lspg_state_t, lspg_residual_t, lspg_matrix_t, fom_type,
    lspg_residual_policy_t, lspg_jacobian_policy_t, scalar_t>::type;

  // declare type of stepper object
  using lspg_stepper_t		= ::pressio::ode::implicitmethods::Stepper<
    stepper_tag, lspg_state_t, lspg_residual_t, lspg_matrix_t, fom_type,
    aux_stepper_t, lspg_residual_policy_t, lspg_jacobian_policy_t, scalar_t>;

};//end class

}}}}}//end  namespace pressio::rom::lspg::unstedy::impl
#endif
