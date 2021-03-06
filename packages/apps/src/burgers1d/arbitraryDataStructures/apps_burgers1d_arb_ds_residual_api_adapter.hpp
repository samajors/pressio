/*
//@HEADER
// ************************************************************************
//
// apps_burgers1d_arb_ds_residual_api_adapter.hpp
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

#ifndef PRESSIOAPPS_BURGERS1D_ARB_DS_RESIDUAL_API_ADAPTER_HPP_
#define PRESSIOAPPS_BURGERS1D_ARB_DS_RESIDUAL_API_ADAPTER_HPP_

#include "../../apps_ConfigDefs.hpp"
#include "apps_burgers1d_arb_ds.hpp"

namespace pressio{ namespace apps{

class Burgers1dArbDsResidualApiAdapter
{
public:
  using int_t		  = typename Burgers1dArbDs::int_t;
  using sc_t		  = typename Burgers1dArbDs::scalar_type;
  using scalar_type	  = typename Burgers1dArbDs::scalar_type;
  using state_type	  = typename Burgers1dArbDs::state_type;
  using residual_type	  = typename Burgers1dArbDs::state_type;
  using dense_matrix_type = typename Burgers1dArbDs::dense_matrix_type;
  using jacobian_type	  = typename Burgers1dArbDs::dense_matrix_type;

  Burgers1dArbDsResidualApiAdapter() = delete;

  Burgers1dArbDsResidualApiAdapter(const Burgers1dArbDs & appObj)
    : appObj_{appObj},
      f_{appObj.meshSize()},
      JJ_{appObj.meshSize(), appObj.meshSize()}{}

public:
  template <typename step_t, typename ... Args>
  void timeDiscreteResidual(const step_t & step,
  			    const scalar_type & time,
  			    const scalar_type & dt,
  			    residual_type & R,
  			    Args && ... states) const
  {
    timeDiscreteResidualImpl(step, time, dt, R, std::forward<Args>(states)... );
  }

  template <typename step_t, typename ... Args>
  void applyTimeDiscreteJacobian(const step_t & step,
  				 const scalar_type & time,
  				 const scalar_type & dt,
  				 const dense_matrix_type & B,
  				 int id,
  				 dense_matrix_type & A,
  				 Args && ... states) const
  {
    applyTimeDiscreteJacobianImpl(step, time, dt, B, id, A, std::forward<Args>(states)...);
  }

  residual_type createTimeDiscreteResidualObject(const state_type & stateIn) const
  {
    std::cout << "calling createTimeDiscreteResidualObject" << std::endl;

    residual_type R( appObj_.meshSize() );
    for (int_t i=0; i<R.extent(0); ++i)
      R(i) = ::pressio::utils::constants::zero<scalar_type>();

    return R;
  }

  dense_matrix_type createApplyTimeDiscreteJacobianObject(const state_type & stateIn,
							  const dense_matrix_type & B) const
  {
    std::cout << "calling createApplyTimeDiscreteJacobianObject" << std::endl;

    dense_matrix_type A(appObj_.meshSize(), B.extent(1));
    for (int_t i=0; i<A.extent(0); ++i)
      for (int_t j=0; j<A.extent(1); ++j)
	A(i,j) = ::pressio::utils::constants::zero<scalar_type>();

    return A;
  }


private:
  // case when we only have a single auxiliary state
  template <typename step_t>
  void timeDiscreteResidualImpl(const step_t & step,
				const scalar_type & time,
				const scalar_type & dt,
				residual_type & R,
				const state_type & yn,
				const state_type & ynm1) const
  {
    appObj_.velocity(yn, time, f_);
    for (int_t i=0; i<f_.extent(0); ++i)
      R(i) = yn(i) - ynm1(i) - dt * f_(i);
  }

  // case when we only have a single auxiliary state
  template <typename step_t, typename state_t>
  void applyTimeDiscreteJacobianImpl(const step_t & step,
				     const scalar_type & time,
				     const scalar_type & dt,
				     const dense_matrix_type & B,
				     int id,
				     dense_matrix_type & A,
				     const state_t & yn,
				     const state_t & ynm1) const
  {
    // compute Jacobian
    appObj_.jacobian(yn, time, JJ_);

    // compute time discrete Jacobian
    for (int_t i=0; i<JJ_.extent(0); ++i){
      for (int_t j=0; j<JJ_.extent(1); ++j){
	JJ_(i,j) *= -dt;
	if (i==j)
	  JJ_(i,j) += ::pressio::utils::constants::one<scalar_type>();
      }
    }

    // compute A = JJ * B
    for (int_t i=0; i<A.extent(0); ++i){
      for (int_t j=0; j<A.extent(1); ++j){
	A(i,j) = ::pressio::utils::constants::zero<scalar_type>();
	for (int_t k=0; k<JJ_.extent(1); ++k){
	  A(i,j) += JJ_(i,k) * B(k,j);
	}
      }
    }
  }

private:
  const Burgers1dArbDs & appObj_;
  mutable state_type f_;
  mutable jacobian_type JJ_;

};//end class

}} //namespace pressio::apps
#endif
