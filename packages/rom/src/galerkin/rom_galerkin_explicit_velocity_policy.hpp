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

#include "../rom_fwd.hpp"
#include "../../../ode/src/explicit/policies/ode_explicit_velocity_policy_base.hpp"
#include "../rom_static_container_fom_states.hpp"

namespace pressio{ namespace rom{

template <
  typename fom_states_data_type,
  typename fom_rhs_t,
  typename decoder_t,
  typename ud_ops
  >
class DefaultGalerkinExplicitVelocityPolicy
  : public ode::policy::ExplicitVelocityPolicyBase<
       DefaultGalerkinExplicitVelocityPolicy<fom_states_data_type,
					     fom_rhs_t,
					     decoder_t, ud_ops>>
{

protected:
  using this_t = DefaultGalerkinExplicitVelocityPolicy<fom_states_data_type,
						       fom_rhs_t,
						       decoder_t,
						       ud_ops>;
  friend ode::policy::ExplicitVelocityPolicyBase<this_t>;

public:
  static constexpr bool isResidualPolicy_ = true;

public:
  DefaultGalerkinExplicitVelocityPolicy() = delete;
  ~DefaultGalerkinExplicitVelocityPolicy() = default;

  /* for constructing this we need to deal with a few cases
   * 1. regular c++ with void ops
   * 2. python bindings with void ops
   * 3. python bindings with non-void ops
   */

  // 1. enable for regular c++ and void ops
  template <
    typename _fom_rhs_t = fom_rhs_t,
    typename _ud_ops = ud_ops,
    mpl::enable_if_t<
      std::is_void<_ud_ops>::value
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
      and !::pressio::containers::meta::is_array_pybind11<_fom_rhs_t>::value
#endif
      > * = nullptr
    >
  DefaultGalerkinExplicitVelocityPolicy(const _fom_rhs_t & fomRhs,
					fom_states_data_type & fomStates,
					const decoder_t & decoder)
    : R_{fomRhs},
      decoder_(decoder),
      fomStates_(fomStates){}


#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  // 2. python bindings with void ops
  template <
    typename _fom_rhs_t = fom_rhs_t,
    typename _ud_ops = ud_ops,
    mpl::enable_if_t<
      std::is_void<_ud_ops>::value
      and ::pressio::containers::meta::is_array_pybind11<_fom_rhs_t>::value
      > * = nullptr
    >
  DefaultGalerkinExplicitVelocityPolicy(const _fom_rhs_t & fomRhs,
					fom_states_data_type & fomStates,
					const decoder_t & decoder)
    : R_{fomRhs},
      decoder_(decoder),
      fomStates_(fomStates){}

  // 3. python bindings with non-void ops
  template <
    typename _fom_rhs_t = fom_rhs_t,
    typename _ud_ops = ud_ops,
    mpl::enable_if_t<
      std::is_same<_ud_ops, pybind11::object>::value
      and ::pressio::containers::meta::is_array_pybind11<_fom_rhs_t>::value
      > * = nullptr
    >
  DefaultGalerkinExplicitVelocityPolicy(const _fom_rhs_t & fomRhs,
  					fom_states_data_type & fomStates,
  					const decoder_t & decoder,
  					const _ud_ops & udOps)
    : R_{fomRhs},
      decoder_(decoder),
      fomStates_(fomStates),
      udOps_{udOps}{}
#endif


public:
  /* for now, the ROM state and ROM velocity must be of the same type */
  template <typename galerkin_state_t, typename fom_t, typename scalar_t>
  void operator()(const galerkin_state_t  & romY,
		  galerkin_state_t	  & romR,
  		  const fom_t		  & app,
		  scalar_t		  t) const{
    this->compute_impl(romY, romR, app, t);
  }

  template <typename galerkin_state_t, typename fom_t, typename scalar_t>
  galerkin_state_t operator()(const galerkin_state_t  & romY,
			      const fom_t	      & app,
			      scalar_t		 t) const
  {
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
    // TODO: make this better, maybe initialized somewhere else
    galerkin_state_t result( const_cast<galerkin_state_t &>(romY).request() );
#else
    galerkin_state_t result(romY);
#endif
    ::pressio::containers::ops::set_zero(result);
    this->compute_impl(romY, result, app, t);
    return result;
  }


private:
  template <
  typename galerkin_state_t,
  typename fom_t,
  typename scalar_t,
  typename _ud_ops = ud_ops,
  mpl::enable_if_t<
    std::is_void<_ud_ops>::value and
    ::pressio::containers::meta::is_vector_wrapper<galerkin_state_t>::value
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
    and !::pressio::containers::meta::is_array_pybind11<galerkin_state_t>::value
#endif
    > * = nullptr
  >
  void compute_impl(const galerkin_state_t  & romY,
		    galerkin_state_t	    & romR,
		    const fom_t		    & app,
		    scalar_t		    t) const
  {
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    auto timer = Teuchos::TimeMonitor::getStackedTimer();
    timer->start("galerkin explicit velocity");
#endif

    fomStates_.reconstructCurrentFomState(romY);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("fom eval rhs");
#endif
    const auto & yFom = fomStates_.getCRefToCurrentFomState();
    app.velocity(*yFom.data(), t, *R_.data());

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("fom eval rhs");
    timer->start("phiT*fomRhs");
#endif

    const auto & phi = decoder_.getReferenceToJacobian();
    containers::ops::dot(phi, R_, romR);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("phiT*fomRhs");
    timer->stop("galerkin explicit velocity");
#endif
  }


#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  template <
  typename galerkin_state_t,
  typename fom_t,
  typename scalar_t,
  typename _ud_ops = ud_ops,
  mpl::enable_if_t<
    ::pressio::containers::meta::is_array_pybind11<galerkin_state_t>::value
    > * = nullptr
  >
  void compute_impl(const galerkin_state_t  & romY,
		    galerkin_state_t	    & romR,
		    const fom_t		    & app,
		    scalar_t		    t) const
  {
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    auto timer = Teuchos::TimeMonitor::getStackedTimer();
    timer->start("galerkin explicit velocity");
#endif

    fomStates_.reconstructCurrentFomState(romY);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("fom eval rhs");
#endif
    const auto & yFom = fomStates_.getCRefToCurrentFomState();
    app.attr("velocity")(yFom, t, R_);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("fom eval rhs");
    timer->start("phiT*fomRhs");
#endif
    const auto & phi = decoder_.getReferenceToJacobian();
    (*this).template applyDecoderJacobianToFomVel<scalar_t>(phi, R_, romR);

    // constexpr bool transposePhi = true;
    // udOps_.attr("multiply")(phi, transposePhi, R_, false, romR);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("phiT*fomRhs");
    timer->stop("galerkin explicit velocity");
#endif
  }


  /* if ops_void, and phi has col-major order, use blas*/
  template <
    typename scalar_t,
    typename operand_t,
    typename result_t,
    typename _ops_t = ud_ops,
    typename _decoder_t = decoder_t,
    mpl::enable_if_t<
      ::pressio::containers::meta::is_fstyle_array_pybind11<typename _decoder_t::jacobian_t>::value and
      std::is_void<_ops_t>::value
      > * = nullptr
    >
  void applyDecoderJacobianToFomVel(const typename _decoder_t::jacobian_t & phi,
				    const operand_t & operandObj,
				    result_t & resObj) const{
    constexpr auto dzero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto done  = ::pressio::utils::constants::one<scalar_t>();
    constexpr auto izero = ::pressio::utils::constants::zero<int>();
    constexpr auto ione  = ::pressio::utils::constants::one<int>();
    constexpr auto transA = ione;
    // overwrite y passed in to dgemv
    constexpr auto owy = ione;
    pybind11::object spy = pybind11::module::import("scipy.linalg.blas");
    spy.attr("dgemv")(done, phi, operandObj, dzero, resObj, izero, ione, izero, ione, transA, owy);
  }

  /* if ops_t == void, phi has row-major order, use numpy*/
  template <
    typename scalar_t,
    typename operand_t,
    typename result_t,
    typename _ops_t = ud_ops,
    typename _decoder_t = decoder_t,
    mpl::enable_if_t<
      ::pressio::containers::meta::is_cstyle_array_pybind11<typename _decoder_t::jacobian_t>::value and
      std::is_void<_ops_t>::value
      > * = nullptr
    >
  void applyDecoderJacobianToFomVel(const typename _decoder_t::jacobian_t & phi,
  				    const operand_t & operandObj,
  				    result_t & resObj) const
  {
    // this is typically a matrix vec product. So  use matmul
    const auto phiT = numpy_.attr("transpose")(phi);
    resObj = numpy_.attr("dot")(phiT, operandObj);
  }

  /* if ops_t == pybind11::object*/
  template <
    typename scalar_t,
    typename operand_t,
    typename result_t,
    typename _ops_t = ud_ops,
    typename _decoder_t = decoder_t,
    mpl::enable_if_t<
      ::pressio::containers::meta::is_cstyle_array_pybind11<typename _decoder_t::jacobian_t>::value and
      std::is_same<_ops_t, pybind11::object>::value
      > * = nullptr
    >
  void applyDecoderJacobianToFomVel(const typename _decoder_t::jacobian_t & phi,
  				    const operand_t & operandObj,
  				    result_t & resObj) const
  {
    constexpr bool transposePhi = true;
    udOps_.attr("multiply")(phi, transposePhi, operandObj, false, resObj);
  }
#endif


protected:
  pybind11::object numpy_ = pybind11::module::import("numpy");
  mutable fom_rhs_t R_ = {};
  const decoder_t & decoder_;
  fom_states_data_type & fomStates_;

#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  typename std::conditional<
    mpl::is_same<ud_ops, pybind11::object>::value,
    ud_ops,
    const ud_ops *
    >::type udOps_ = {};
#else
    const ud_ops * udOps_ = {};
#endif


};//end class

}}//end namespace pressio::rom
#endif
