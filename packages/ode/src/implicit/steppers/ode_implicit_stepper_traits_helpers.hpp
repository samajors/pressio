/*
//@HEADER
// ************************************************************************
//
// ode_implicit_stepper_traits_helpers.hpp
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

#ifndef ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_HELPERS_HPP_
#define ODE_STEPPERS_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_TRAITS_HELPERS_HPP_

#include "../../ode_fwd.hpp"
#include "../policies/meta/ode_find_if_legitimate_implicit_residual_policy.hpp"
#include "../policies/meta/ode_find_if_legitimate_implicit_jacobian_policy.hpp"
#include "../policies/meta/ode_find_if_legitimate_residual_policy_for_implicit_arbitrary_stepper.hpp"
#include "../policies/meta/ode_find_if_legitimate_jacobian_policy_for_implicit_arbitrary_stepper.hpp"
#include "../../meta/ode_is_valid_user_defined_ops_for_implicit_ode.hpp"
#include "../../meta/ode_is_legitimate_model_for_implicit_ode.hpp"
#include "../../meta/ode_is_legitimate_model_for_implicit_ode_arbitrary_stepper.hpp"
#include "../../meta/ode_is_stepper_order_setter.hpp"
#include "../../meta/ode_is_stepper_total_n_states_setter.hpp"

namespace pressio{ namespace ode{ namespace details{ namespace impl{

// these will be definitely improved... when I have time

template <typename T, typename = void>
struct ScalarHelper{
  static constexpr bool value = false;
  using type = void;
};

template <typename T>
struct ScalarHelper<
  T,
  mpl::enable_if_t<
    ::pressio::containers::meta::is_wrapper<T>::value and
    ::pressio::containers::details::traits<T>::wrapped_package_identifier
    != ::pressio::containers::details::WrappedPackageIdentifier::Arbitrary
    >
  >{
  static constexpr bool value = true;
  using type = typename ::pressio::containers::details::traits<T>::scalar_t;
};

template <typename T>
struct ScalarHelper<
  T,
  mpl::enable_if_t<
    ::pressio::containers::meta::is_wrapper<T>::value and
    ::pressio::containers::details::traits<T>::wrapped_package_identifier
    == ::pressio::containers::details::WrappedPackageIdentifier::Arbitrary
    >
  >{
  static constexpr bool value = false;
  using type = void;
};
//-------------------------------------------------------------------


template <
  typename system_t,
  typename state_t,
  typename residual_t,
  typename jacobian_t,
  typename enable = void
  >
struct StdPoliciesPicker;

template <
  typename system_t,
  typename state_t,
  typename residual_t,
  typename jacobian_t
  >
struct StdPoliciesPicker<
  system_t, state_t, residual_t, jacobian_t
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
  , mpl::enable_if_t<
    mpl::not_same<system_t, pybind11::object>::value
    >
#endif
  >
{
  using standard_res_policy_t = policy::ImplicitResidualStandardPolicy<
    state_t, system_t, residual_t>;
  using standard_jac_policy_t = policy::ImplicitJacobianStandardPolicy<
    state_t, system_t, jacobian_t>;
};

#ifdef PRESSIO_ENABLE_TPL_PYBIND11
template <
  typename system_t,
  typename state_t,
  typename residual_t,
  typename jacobian_t
  >
struct StdPoliciesPicker<
  system_t, state_t, residual_t, jacobian_t,
  mpl::enable_if_t<
    mpl::is_same<system_t, pybind11::object>::value
    >
  >
{
  using standard_res_policy_t = policy::ImplicitResidualStandardPolicyPybind11<
    state_t, system_t, residual_t>;
  using standard_jac_policy_t = policy::ImplicitJacobianStandardPolicyPybind11<
    state_t, system_t, jacobian_t>;
};
#endif
//-------------------------------------------------------------------


template <typename T, bool isStdPolicy>
struct CheckModelJacobianMethods;

template <typename T>
struct CheckModelJacobianMethods<T, false> {};

template <typename T>
struct CheckModelJacobianMethods<T, true>{
  static_assert(::pressio::ode::meta::model_has_needed_jacobian_methods<
		T, typename T::state_type,
		typename T::jacobian_type, typename T::scalar_type
		>::value,
		"\nThe model type you passed to the implicit stepper \n \
does not have valid jacobian methods and since you are using \n \
a standard policy, the model needs to implement the jacobian. \n \
See api for reference.");
};

}}}}//end namespace pressio::ode::details::impl
#endif
