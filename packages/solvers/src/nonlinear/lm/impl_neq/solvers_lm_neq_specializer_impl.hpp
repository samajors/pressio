/*
//@HEADER
// ************************************************************************
//
// solvers_lm_neq_specialization_picker.hpp
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

#ifndef SOLVERS_LM_NEQ_SPECIALIZATION_PICKER_HPP_
#define SOLVERS_LM_NEQ_SPECIALIZATION_PICKER_HPP_

#include "./res_jac_api/solvers_lm_neq_res_jac_api_impl.hpp"

namespace pressio{ namespace solvers{ namespace nonlinear{ namespace impl{

/*
template <typename T1, typename T2>
struct ObserverTypesSupported{
  static constexpr bool value = false;
};

template <>
struct ObserverTypesSupported<void, void>{
  static constexpr bool value = true;
  using type = void;
};

template <typename T>
struct ObserverTypesSupported<void, T>{
  static constexpr bool value = true;
  using type = T;
};

template <typename T>
struct ObserverTypesSupported<T, void>{
  static constexpr bool value = true;
  using type = T;
};

template <typename T>
struct ObserverTypesSupported<T, T>{
  static constexpr bool value = true;
  using type = T;
};

*/

template <
  typename scalar_t,
  typename gradient_t,
  typename residual_t,
  typename jacobian_t,
  typename hessian_t,
  typename ...Args
  >
struct LMNEqCustomOpsDetectionHelper
{

  using gradient_native_t = typename ::pressio::containers::details::traits<gradient_t>::wrapped_t;
  using residual_native_t = typename ::pressio::containers::details::traits<residual_t>::wrapped_t;
  using jacobian_native_t = typename ::pressio::containers::details::traits<jacobian_t>::wrapped_t;

  static_assert(::pressio::containers::meta::is_vector_wrapper<gradient_t>::value, "gigi");

  /* detect if Args contain valid ops type with static methods to compute norm of residual */
  using icNorm	  = ::pressio::mpl::variadic::find_if_ternary_pred_t<
    residual_native_t, scalar_t, ::pressio::solvers::meta::has_all_needed_methods_norms, Args...>;
  using norm_op_t = ::pressio::mpl::variadic::at_or_t<void, icNorm::value, Args...>;

  /* detect if Args contain valid ops type with static methods to compute hessian */
  using icHessMV = ::pressio::mpl::variadic::find_if_quaternary_pred_t<
    jacobian_native_t, hessian_t, scalar_t,
    ::pressio::solvers::meta::has_all_needed_methods_for_hessian, Args...>;
  using hess_op_t = ::pressio::mpl::variadic::at_or_t<void, icHessMV::value, Args...>;

  /* detect if Args contain valid ops type with static methods to compute gradient J^T R */
  using icGrad = ::pressio::mpl::variadic::find_if_quinary_pred_t<
    jacobian_native_t, residual_native_t, gradient_t, scalar_t,
    ::pressio::solvers::meta::has_all_needed_methods_for_gradient, Args...>;
  using grad_op_t = ::pressio::mpl::variadic::at_or_t<void, icGrad::value, Args...>;

  //find if there is a single type that contains all methods for all ops
  static constexpr bool found = std::is_same<hess_op_t, norm_op_t>::value and
  				  std::is_same<hess_op_t, grad_op_t>::value and
  				  !std::is_void<hess_op_t>::value;

  // here, I know that there is a single type for all ops, so it does not matter which one to use
  using ops_t = norm_op_t;
  using type = typename std::conditional< found, ops_t, void >::type;

//   static_assert( std::is_void<type>::value or
//     (!std::is_void<type>::value and ::pressio::containers::meta::is_multi_vector_wrapper<jacobian_t>::value),
//      "For GaussNewton normal-eq solver, custom ops are currently supported when the
// jacobian is a multi-vector wrapper. If you are using this for doing ROM, this most likely
// means you wrapped the Jacobian type of your basis with a matrix not a multi-vector.");

};



template <
  bool hasDefaultApi,
  typename system_t,
  typename scalar_t,
  typename linear_solver_t,
  typename lm_schedule_policy_tag,
  typename convergence_t,
  typename ... Args>
struct LMNEQSpecializeApi;


template <
  typename system_t,
  typename scalar_t,
  typename linear_solver_t,
  typename lm_schedule_policy_tag,
  typename convergence_t,
  typename ... Args>
struct LMNEQSpecializeApi<
  true, system_t, scalar_t, linear_solver_t, lm_schedule_policy_tag, convergence_t, Args...>
{
  using linear_solver_matrix_t = typename linear_solver_t::matrix_type;

  /* ------------------------------------------------ */
  // check if the sequence contains a valid hessian type
  using ic3 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::solvers::meta::is_legitimate_hessian_for_gn_normeq, Args...>;
  // if no hessian is found, then get it from the linear solver type
  using hessian_t = ::pressio::mpl::variadic::at_or_t<linear_solver_matrix_t, ic3::value, Args...>;
  // in every scenario, the hesssian type must match the matrix type of linear solver
  static_assert( std::is_same<hessian_t, linear_solver_matrix_t>::value,
		 "Hessian type passed to LM must match the matrix type in the linear solver");
  static_assert(!std::is_void<hessian_t>::value,
  		"The hessian type cannot be void");

  /* ------------------------------------------------ */
  // check if sequence contains an observer for the residual after LM is converged
  using ic6 = ::pressio::mpl::variadic::find_if_binary_pred_t<
    typename system_t::residual_type,
    ::pressio::solvers::meta::is_legitimate_residual_observer_when_solver_converged, Args...>;
  using observer_when_conv_t = ::pressio::mpl::variadic::at_or_t<void, ic6::value, Args...>;

  /* ------------------------------------------------ */
  // check if sequence contains an observer for the residual at each LM step
  using ic7 = ::pressio::mpl::variadic::find_if_binary_pred_t<
    typename system_t::residual_type,
    ::pressio::solvers::meta::is_legitimate_residual_observer_each_solver_step, Args...>;
  using observer_each_step_t = ::pressio::mpl::variadic::at_or_t<void, ic7::value, Args...>;

  /* currently we only allow all methods to observe the residual to be
   * in the same observer class. So we need to check here that when both
   *  observers type detected are NOT void, then they must be the same type */
  using obs_supported_t = ObserverTypesSupported<observer_when_conv_t, observer_each_step_t>;
  static_assert( obs_supported_t::value,
  "Currently, can only accept a single residual observer type to do everything. \
  You can still have multiple methods inside that type to cover all possible observations,\
  but cannot pass multiple types with individual observation methods");
  using observer_t = typename obs_supported_t::type;

  /* --------------------------------------------------------- */
  // check if the sequence contains a valid type for custom ops
  /* There are three functionalities needed for LM normal-eq:
   * 1. computing the norm of the residual and gradient
   * 2. computing the hessian J^T * J
   * 3. computing the gradient J^T * R
   *
   * For 2., remember that the system's jacobian can be either a matrix or a mv wrapper
   * and, depending on that, we (might) do things differently when we have to compute hessian.
   * so when we detect custom ops, we need to differentiate based on jacobian's type
   */
  using gradient_t = typename system_t::state_type; // the gradient is same type as state
  using residual_t = typename system_t::residual_type;
  using jacobian_t = typename system_t::jacobian_type;
  using ud_ops_t   = typename GaussNewtonNEqCustomOpsDetectionHelper<
    scalar_t, gradient_t, residual_t, jacobian_t, hessian_t, Args...>::type;

  // the class type
  using type = ::pressio::solvers::nonlinear::impl::LMNormalEqResJacApi<
    system_t, hessian_t, linear_solver_t, scalar_t, lm_schedule_policy_tag,
    convergence_t, observer_t, ud_ops_t>;
};


template <
  typename system_t,
  typename scalar_t,
  typename linear_solver_t,
  typename lm_schedule_policy_tag,
  typename convergence_t,
  typename ... Args>
struct LMNEQSpecializeApi<
  false, system_t, scalar_t, linear_solver_t, lm_schedule_policy_tag, convergence_t, Args...>
{
  using type = ::pressio::solvers::nonlinear::impl::GaussNewtonHessianGradientApi<
    system_t, linear_solver_t, scalar_t, lm_schedule_policy_tag, convergence_t>;
};



template <typename ... Args>
struct LMNormalEquationsSpecializer
{
  /* ------------------------------------------------ */
  // verify the sequence contains a valid system type
  using ic1 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::solvers::meta::is_legitimate_system_for_gauss_newton_normal_eq, Args...>;
  using system_t = ::pressio::mpl::variadic::at_or_t<void, ic1::value, Args...>;
  static_assert(!std::is_void<system_t>::value and ic1::value < sizeof... (Args),
		"A valid system type must be passed to LM templates. \
This compile-time error means that template arguments passed to the GaussNewton solver\
do not contain a type that is admissible for the normal-equation solver.");
  static constexpr bool hasDefaultApi = ::pressio::solvers::meta::system_meets_default_api<system_t>::value;

  /* ------------------------------------------------ */
  // since system is valid, detect the scalar type
  using scalar_t = typename system_t::scalar_type;

  /* ------------------------------------------------ */
  // check for a valid linear solver type
  using ic2 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::solvers::meta::is_legitimate_linear_solver_for_gn_normeq, Args...>;
  using linear_solver_t = ::pressio::mpl::variadic::at_or_t<void, ic2::value, Args...>;
  static_assert(!std::is_void<linear_solver_t>::value and ic2::value < sizeof... (Args),
  		"A valid linear solver type must be passed to LM with normal equations");

  /* ------------------------------------------------ */
  // check if sequence contains a line search tag
  using ic4 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::solvers::meta::is_legitimate_lm_schedule_policy_tag, Args...>;
  using default_policy_tag = ::pressio::solvers::iterative::lm::SchedulePolicy2;
  using lm_schedule_policy_tag = ::pressio::mpl::variadic::at_or_t<default_policy_tag, ic4::value, Args...>;
  static_assert(!std::is_void<lm_schedule_policy_tag>::value,
		"The line search type for LM cannot be void: either omit it so that I use the \
default, or pick one that is valid.");

  /* ------------------------------------------------ */
  // check if sequence contains a valid convergence tag
  using ic5 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::solvers::meta::is_legitimate_convergence_tag, Args...>;
  using default_conv = ::pressio::solvers::iterative::default_convergence;
  using convergence_t = ::pressio::mpl::variadic::at_or_t<default_conv, ic5::value, Args...>;
  static_assert(!std::is_void<convergence_t>::value, "The convergence type for LM cannot be void");

  /* ------------------------------------------------ */
  // the types above are common for all APIs, now pass args to specializers for further inspection
  using type =
    typename LMNEQSpecializeApi<hasDefaultApi, system_t, scalar_t, linear_solver_t, lm_schedule_policy_tag, convergence_t, Args...>::type;
};

}}}}//end namespace pressio::solvers::nonlinear::impl
#endif
