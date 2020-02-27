/*
//@HEADER
// ************************************************************************
//
// rom_galerkin_explicit_velocity_policy.hpp
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
]//
// Questions? Contact Francesco Rizzi (fnrizzi@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef ROM_DEFAULT_GALERKIN_EXPLICIT_VELOCITY_POLICY_HPP_
#define ROM_DEFAULT_GALERKIN_EXPLICIT_VELOCITY_POLICY_HPP_

namespace pressio{ namespace rom{ namespace galerkin{

template <
  typename fom_states_data_type,
  typename fom_rhs_t,
  typename decoder_t,
  typename ud_ops
  >
class DefaultExplicitVelocityPolicy
  : public ::pressio::ode::explicitmethods::policy::VelocityPolicyBase<
       DefaultExplicitVelocityPolicy<fom_states_data_type,
				     fom_rhs_t,
				     decoder_t, ud_ops>>
{

protected:
  using this_t = DefaultExplicitVelocityPolicy<fom_states_data_type,
					       fom_rhs_t,
					       decoder_t,
					       ud_ops>;
  friend ::pressio::ode::explicitmethods::policy::VelocityPolicyBase<this_t>;

public:
  static constexpr bool isResidualPolicy_ = true;

public:
  DefaultExplicitVelocityPolicy() = delete;
  ~DefaultExplicitVelocityPolicy() = default;

  template <
    typename _fom_rhs_t = fom_rhs_t,
    typename _ud_ops = ud_ops,
    ::pressio::mpl::enable_if_t<
      std::is_void<_ud_ops>::value
      and ::pressio::containers::meta::is_wrapper<_fom_rhs_t>::value
      > * = nullptr
    >
  DefaultExplicitVelocityPolicy(const _fom_rhs_t & fomRhs,
				fom_states_data_type & fomStates,
				const decoder_t & decoder)
    : fomRhs_{fomRhs},
      phi_(decoder.getReferenceToJacobian()),
      fomStates_(fomStates){}


public:
  /* for now, the ROM state and ROM velocity must be of the same type */
  template <typename galerkin_state_t, typename fom_t, typename scalar_t>
  void operator()(const galerkin_state_t  & romState,
		  galerkin_state_t	  & romRhs,
  		  const fom_t		  & app,
		  const scalar_t	  & t) const{
    this->compute_impl(romState, romRhs, app, t);
  }

  template <typename galerkin_state_t, typename fom_t, typename scalar_t>
  galerkin_state_t operator()(const galerkin_state_t  & romState,
			      const fom_t	      & app,
			      const scalar_t	      & t) const
  {
    // TODO: make this better, maybe initialized somewhere else
    // evenif this is called once, this is ugly
    galerkin_state_t result(romState);
    ::pressio::ops::set_zero(result);
    this->compute_impl(romState, result, app, t);
    return result;
  }

private:
  //--------------------------------------------
  // query fom velocity
  //--------------------------------------------
  template<
    typename scalar_t,
    typename fom_t,
    typename fom_state_t,
    typename _fom_rhs_t = fom_rhs_t,
    ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_wrapper<fom_state_t>::value
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
    and ::pressio::mpl::not_same<fom_t, pybind11::object>::value
#endif
    > * = nullptr
  >
  void queryFomVelocityDispatch(const fom_t & app,
				const fom_state_t & fomState,
				const scalar_t & time) const
  {
    app.velocity(*fomState.data(), time, *fomRhs_.data());
  }

#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  template<
    typename scalar_t,
    typename fom_t,
    typename fom_state_t,
    typename _fom_rhs_t = fom_rhs_t,
    ::pressio::mpl::enable_if_t<
      ::pressio::containers::meta::is_vector_wrapper_pybind<fom_state_t>::value and
      ::pressio::mpl::is_same<fom_t, pybind11::object>::value
      > * = nullptr
    >
  void queryFomVelocityDispatch(const fom_t & app,
				const fom_state_t & fomState,
				const scalar_t & time) const
  {
    *fomRhs_.data() = app.attr("velocity")(*fomState.data(), time);
  }
#endif

  // --------------------------------------------
  // compute RHS of ode
  // --------------------------------------------
  template <
    typename scalar_t,
    typename result_t,
    typename _ops_t = ud_ops,
    ::pressio::mpl::enable_if_t<
      ::pressio::containers::meta::is_wrapper<result_t>::value and
      std::is_void<_ops_t>::value
      > * = nullptr
    >
  void applyDecoderJacobianToFomVelDispatch(result_t & resObj) const
  {
    constexpr auto zero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto one  = ::pressio::utils::constants::one<scalar_t>();
    ::pressio::ops::product(::pressio::transpose(), one, phi_, fomRhs_, zero, resObj);
  }

private:
  template <
    typename galerkin_state_t,
    typename fom_t,
    typename scalar_t,
    typename _ud_ops = ud_ops,
    ::pressio::mpl::enable_if_t< std::is_void<_ud_ops>::value > * = nullptr
  >
  void compute_impl(const galerkin_state_t  & romState,
		    galerkin_state_t	    & romRhs,
		    const fom_t		    & app,
		    const scalar_t	    & t) const
  {
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    auto timer = Teuchos::TimeMonitor::getStackedTimer();
    timer->start("galerkin explicit velocity");
#endif

    fomStates_.reconstructCurrentFomState(romState);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("fom eval rhs");
#endif
    const auto & yFom = fomStates_.getCRefToCurrentFomState();
    (*this).template queryFomVelocityDispatch<scalar_t>(app, yFom, t);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("fom eval rhs");
    timer->start("phiT*fomRhs");
#endif

    (*this).template applyDecoderJacobianToFomVelDispatch<scalar_t>(romRhs);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("phiT*fomRhs");
    timer->stop("galerkin explicit velocity");
#endif
  }

protected:
  mutable fom_rhs_t fomRhs_ = {};
  const typename decoder_t::jacobian_t & phi_;
  fom_states_data_type & fomStates_;

#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  typename std::conditional<
    ::pressio::mpl::is_same<ud_ops, pybind11::object>::value,
    ud_ops,
    const ud_ops *
    >::type udOps_ = {};
#else
    const ud_ops * udOps_ = {};
#endif

};//end class

}}}//end namespace pressio::rom::galerkin
#endif
