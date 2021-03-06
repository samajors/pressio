/*
//@HEADER
// ************************************************************************
//
// ode_integrate_n_steps_explicit.hpp
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

#ifndef ODE_INTEGRATORS_INTEGRATE_N_STEPS_EXPLICIT_HPP_
#define ODE_INTEGRATORS_INTEGRATE_N_STEPS_EXPLICIT_HPP_

#include "./impl/ode_call_stepper_policy.hpp"
#include "./impl/ode_n_steps_integrators.hpp"

namespace pressio{ namespace ode{


template<typename stepper_type, typename state_type, typename time_type>
mpl::enable_if_t<
  ::pressio::mpl::publicly_inherits_from<
    stepper_type, ::pressio::ode::explicitmethods::StepperBase<stepper_type>
    >::value
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  and !::pressio::containers::meta::is_array_pybind<state_type>::value
#endif
>
integrateNSteps(explicitmethods::StepperBase<stepper_type> & stepper,
		state_type			    & odeStateInOut,
		const time_type		    start_time,
		const time_type		    dt,
		const ::pressio::ode::types::step_t num_steps)
{

  static_assert(::pressio::ode::meta::is_legitimate_explicit_state_type<state_type>::value,
		"You are trying to call integrateNSteps with an explicit stepper \
but the state type you are using is not admissible for explicit time-stepping. \
See the requirements inside ode_is_legitimate_explicit_state_type.hpp");

  using do_step_policy_t = impl::ExplicitDoStepBasic;
  using advancer_t	 = impl::IntegratorNStepsWithConstDt<do_step_policy_t>;
  advancer_t::execute(num_steps, start_time, dt, odeStateInOut, stepper);
}


#ifdef PRESSIO_ENABLE_TPL_PYBIND11

/* for pybind, we cannot use:
 * explicitmethods::StepperBase<stepper_type> & stepper
 * because the stepper is passed from Python so it does not know which overload to use
 * and we get a type error since it sees the stepper as an python object
 */

template<typename stepper_type, typename state_type, typename time_type>
mpl::enable_if_t<
  ::pressio::mpl::publicly_inherits_from<
    stepper_type, ::pressio::ode::explicitmethods::StepperBase<stepper_type>
    >::value and
  ::pressio::containers::meta::is_array_pybind<state_type>::value
>
integrateNSteps(stepper_type & stepper,
		state_type	 & odeStateInOut,
		const time_type	 start_time,
		const time_type	 dt,
		const ::pressio::ode::types::step_t num_steps)
{
  // here we want to view the odeStateInOut since we want to modify its data,
  // which is numpy array owned by the user inside their Python code.
  // upon exit of this function, the original odeStateInOut is changed since odeStateView only views it.
  ::pressio::containers::Vector<state_type> odeStateView(odeStateInOut, ::pressio::view());

  using do_step_policy_t = impl::ExplicitDoStepBasic;
  using advancer_t	 = impl::IntegratorNStepsWithConstDt<do_step_policy_t>;
  advancer_t::execute(num_steps, start_time, dt, odeStateView, stepper);
}
#endif



template<
  typename stepper_type,
  typename state_type,
  typename time_type,
  typename collector_type
>
mpl::enable_if_t<
  ::pressio::mpl::publicly_inherits_from<
    stepper_type, ::pressio::ode::explicitmethods::StepperBase<stepper_type>
    >::value
>
integrateNSteps(explicitmethods::StepperBase<stepper_type> & stepper,
		state_type				 & odeStateInOut,
		const time_type			 start_time,
		const time_type			 dt,
		const ::pressio::ode::types::step_t num_steps,
		collector_type			 & collector)
{

  static_assert(::pressio::ode::meta::is_legitimate_explicit_state_type<state_type>::value,
		"You are trying to call integrateNSteps with an explicit stepper \
but the state type you are using is not admissible for explicit time-stepping.\
See the requirements inside ode_is_legitimate_explicit_state_type.hpp");


  static_assert(::pressio::ode::meta::is_legitimate_collector<
      collector_type, ::pressio::ode::types::step_t, time_type, state_type
		>::value, "You are trying to call integrateNSteps with an explicit stepper \
and a collector, but the collector type you are using is not admissible. \
It does not meet the API of a valid collector. See requirements in ode_is_legitimate_collector.hpp");

  using do_step_policy_t = impl::ExplicitDoStepBasic;
  using advancer_t	 = impl::IntegratorNStepsWithCollectorAndConstDt<collector_type, do_step_policy_t>;
  advancer_t::execute(num_steps, start_time, dt, odeStateInOut, collector, stepper);
}

}}//end namespace pressio::ode
#endif
