/*
//@HEADER
// ************************************************************************
//
// TBD.hpp
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

#ifndef PRESSIO_ROM_EXP_GALERKIN_RESIDUAL_POLICY_RESIDUAL_API_HPP_
#define PRESSIO_ROM_EXP_GALERKIN_RESIDUAL_POLICY_RESIDUAL_API_HPP_

namespace pressio{ namespace rom{ namespace galerkin{ namespace impl{

template <
  typename rom_residual_type,
  typename fom_residual_type,
  typename decoder_type,
  typename fom_states_data_type,
  typename fom_querier_policy
  >
class ResidualPolicyResidualApi
{
  using scalar_t = typename ::pressio::containers::details::traits<rom_residual_type>::scalar_t;

public:
  using residual_t = rom_residual_type;

public:
  ResidualPolicyResidualApi() = delete;
  ~ResidualPolicyResidualApi() = default;

  template< typename app_t>
  ResidualPolicyResidualApi(fom_states_data_type & fomStatesIn,
			    const fom_querier_policy & fomQuerier,
			    const decoder_type & decoder,
			    const app_t & appObj)
    : fomQuerier_(fomQuerier),
      fomStates_(fomStatesIn),
      phi_(decoder.getReferenceToJacobian()),
      fomR_(fomQuerier_.evaluate(fomStates_.getCRefToCurrentFomState(), appObj)){}

public:
  template <typename rom_state_t, typename rom_prev_states_t, typename fom_t>
  void operator()(const rom_state_t & romState,
                  const rom_prev_states_t & romPrevStates,
                  const fom_t & app,
                  const scalar_t & time,
                  const scalar_t & dt,
                  const ::pressio::ode::types::step_t & step,
                  residual_t & romR) const
  {
    this->compute_impl(romState, romPrevStates, app, time, dt, step, romR);
  }

  // this method only called once at the beginning
  template <typename rom_state_t, typename fom_t>
  residual_t operator()(const rom_state_t & romState, const fom_t & app) const
  {
    fomStates_.reconstructCurrentFomState(romState);
    fom_residual_type fomR(fomQuerier_.evaluate(fomStates_.getCRefToCurrentFomState(), app));
    residual_t R(romState.extent(0));
    constexpr auto zero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto one  = ::pressio::utils::constants::one<scalar_t>();
    ::pressio::ops::product(::pressio::transpose(), one, phi_, fomR, zero, R);
    return R;
  }

private:
  template <typename rom_state_t, typename rom_prev_states_t>
  void doFomStatesReconstruction(const rom_state_t & romState,
				 const rom_prev_states_t & romPrevStates,
				 const ::pressio::ode::types::step_t & step) const
  {
    /* the currrent FOM has to be recomputed every time regardless
     * of whether the step changes since we might be inside a non-linear solve
     * where the time step does not change but this residual method
     * is called multiple times.
     */
    fomStates_.reconstructCurrentFomState(romState);

    /* the previous FOM states should only be recomputed when the time step changes
     * we do not need to reconstruct all the FOM states, we just need to reconstruct
     * the state at the previous step (i.e. t-dt) which is stored in romPrevStates[0]
     */
    if (storedStep_ != step){
      fomStates_ << romPrevStates.get(ode::nMinusOne());
      storedStep_ = step;
    }
  }

  // we have here n = 1 prev rom states
  template <typename rom_state_t, typename rom_prev_states_t, typename fom_t, typename scalar_t>
  mpl::enable_if_t< rom_prev_states_t::size()==1 >
  compute_impl(const rom_state_t		        & romState,
		    const rom_prev_states_t		& romPrevStates,
		    const fom_t			        & app,
		    const scalar_t		        & time,
		    const scalar_t			& dt,
		    const ::pressio::ode::types::step_t & step,
		    residual_t			        & romR) const
  {
    doFomStatesReconstruction(romState, romPrevStates, step);
    const auto & yn   = fomStates_.getCRefToCurrentFomState();
    const auto & ynm1 = fomStates_.getCRefToFomStatePrevStep();
    fomQuerier_.evaluate(yn, ynm1, app, time, dt, step, fomR_);

    constexpr auto zero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto one  = ::pressio::utils::constants::one<scalar_t>();
    ::pressio::ops::product(::pressio::transpose(), one, phi_, fomR_, zero, romR);
  }

  // we have here n = 2 prev rom states
  template <typename rom_state_t, typename rom_prev_states_t, typename fom_t, typename scalar_t>
  mpl::enable_if_t< rom_prev_states_t::size()==2 >
  compute_impl(const rom_state_t			& romState,
		    const rom_prev_states_t		& romPrevStates,
		    const fom_t			        & app,
		    const scalar_t		        & time,
		    const scalar_t			& dt,
		    const ::pressio::ode::types::step_t & step,
		    residual_t			        & romR) const
  {
    doFomStatesReconstruction(romState, romPrevStates, step);

    const auto & yn   = fomStates_.getCRefToCurrentFomState();
    const auto & ynm1 = fomStates_.getCRefToFomStatePrevStep();
    const auto & ynm2 = fomStates_.getCRefToFomStatePrevPrevStep();
    fomQuerier_.evaluate(yn, ynm1, ynm2, app, time, dt, step, fomR_);

    constexpr auto zero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto one  = ::pressio::utils::constants::one<scalar_t>();
    ::pressio::ops::product(::pressio::transpose(), one, phi_, fomR_, zero, romR);
  }

private:
  const fom_querier_policy & fomQuerier_;

  // storedStep is used to keep track of which step we are doing.
  // This is used to decide whether we need to update/recompute the previous
  // FOM states or not. Since it does not make sense to recompute previous
  // FOM states if we are not in a new time step.
  mutable ::pressio::ode::types::step_t storedStep_ = {};

  fom_states_data_type & fomStates_;
  const typename decoder_type::jacobian_type & phi_;
  mutable fom_residual_type fomR_;
};

}}}}//end namespace
#endif
