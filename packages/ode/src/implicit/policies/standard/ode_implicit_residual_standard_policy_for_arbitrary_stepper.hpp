/*
//@HEADER
// ************************************************************************
//
// ode_implicit_residual_standard_policy_for_arbitrary_stepper.hpp
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

#ifndef ODE_POLICIES_STANDARD_IMPLICIT_RESIDUAL_STANDARD_POLICY_FOR_ARBITRARY_STEPPER_HPP_
#define ODE_POLICIES_STANDARD_IMPLICIT_RESIDUAL_STANDARD_POLICY_FOR_ARBITRARY_STEPPER_HPP_

namespace pressio{ namespace ode{ namespace implicitmethods{ namespace policy{

template<
  typename state_type,
  typename system_type,
  typename residual_type
  >
class ResidualStandardPolicyForArbitraryStepper<
  state_type, system_type, residual_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::ode::meta::is_legitimate_implicit_state_type<state_type>::value and
    ::pressio::ode::meta::is_legitimate_implicit_residual_type<residual_type>::value and
    containers::meta::is_wrapper<state_type>::value and
    containers::meta::is_wrapper<residual_type>::value
    >
  >
{

  using this_t = ResidualStandardPolicyForArbitraryStepper<state_type, system_type, residual_type>;

public:
  ResidualStandardPolicyForArbitraryStepper() = default;
  ~ResidualStandardPolicyForArbitraryStepper() = default;

public:

  residual_type operator()(const state_type & odeCurrentState,
  			   const system_type & model) const{

    residual_type R(model.createTimeDiscreteResidualObject(*odeCurrentState.data()));
    return R;
  }

  //-------------------------------
  // specialize for n == 1
  //-------------------------------
  template <typename prev_states_type, typename scalar_type>
  mpl::enable_if_t< prev_states_type::size()==1>
  operator()(const state_type & odeCurrentState,
		  const prev_states_type & prevStates,
		  const system_type & model,
		  const scalar_type & t,
		  const scalar_type & dt,
		  const types::step_t & step,
		  residual_type & R) const{

    const auto & ynm1 = prevStates.get(ode::nMinusOne());

    model.template timeDiscreteResidual(step, t, dt,
					*R.data(),
					*odeCurrentState.data(),
					*ynm1.data());
  }

  //-------------------------------
  // specialize for n == 2
  //-------------------------------
  template <typename prev_states_type, typename scalar_type>
  mpl::enable_if_t< prev_states_type::size()==2>
  operator()(const state_type & odeCurrentState,
		  const prev_states_type & prevStates,
		  const system_type & model,
		  const scalar_type & t,
		  const scalar_type & dt,
		  const types::step_t & step,
		  residual_type & R) const{

    const auto & ynm1 = prevStates.get(ode::nMinusOne());
    const auto & ynm2 = prevStates.get(ode::nMinusTwo());

    model.template timeDiscreteResidual(step, t, dt,
					*R.data(),
					*odeCurrentState.data(),
					*ynm1.data(),
					*ynm2.data());
  }

  //-------------------------------
  // specialize for n == 3
  //-------------------------------
  template <typename prev_states_type, typename scalar_type>
  mpl::enable_if_t< prev_states_type::size()==3>
  operator()(const state_type & odeCurrentState,
		  const prev_states_type & prevStates,
		  const system_type & model,
		  const scalar_type & t,
		  const scalar_type & dt,
		  const types::step_t &  step,
		  residual_type & R) const{

    const auto & ynm1 = prevStates.get(ode::nMinusOne());
    const auto & ynm2 = prevStates.get(ode::nMinusTwo());
    const auto & ynm3 = prevStates.get(ode::nMinusThree());

    model.template timeDiscreteResidual(step, t, dt,
					*R.data(),
					*odeCurrentState.data(),
					*ynm1.data(),
					*ynm2.data(),
					*ynm3.data());
  }

};//end class

}}}}//end namespace pressio::ode::implicitmethods::policy
#endif
