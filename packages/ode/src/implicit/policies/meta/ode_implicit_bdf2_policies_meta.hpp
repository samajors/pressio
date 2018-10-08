
#ifndef ODE_POLICIES_META_IMPLICIT_bdf2_POLICIES_META_HPP_
#define ODE_POLICIES_META_IMPLICIT_bdf2_POLICIES_META_HPP_

#include "ode_implicit_policies_meta.hpp"
#include "../standard/ode_implicit_bdf2_residual_standard_policy.hpp"
#include "../standard/ode_implicit_bdf2_jacobian_standard_policy.hpp"

namespace rompp{
namespace ode{
namespace meta {

// //-----------------------------------------------------------------
// // METAF FOR ADMISSIBLE IMPLICIT EULER RESIDUAL
// //-----------------------------------------------------------------
// template<typename policy_t, typename enable = void>
// struct is_legitimate_implicit_bdf2_residual_policy : std::false_type{};
  
// template <typename policy_t>
// struct is_legitimate_implicit_bdf2_residual_policy<
//   policy_t,
//   typename std::enable_if<
//     core::meta::publicly_inherits_from<
//       policy_t,
//       ode::policy::ImplicitResidualPolicyBase<policy_t, 2, 0>
//       >::value 
//     >::type
//   > : std::true_type{};
  
  
// //-----------------------------------------------------------------
// // METAF FOR ADMISSIBLE IMPLICIT EULER JACOBIAN
// //-----------------------------------------------------------------
// template<typename policy_t, typename enable = void>
// struct is_legitimate_implicit_bdf2_jacobian_policy
//   : is_legitimate_implicit_jacobian_policy<policy_t>{};


// //-----------------------------------------------------------------
// // METAF TO CHECK RESIDUAL POLICY IS STANDARD
// //-----------------------------------------------------------------
// template<typename policy_t, typename enable = void>
// struct is_implicit_bdf2_residual_standard_policy
//   : std::false_type{};

  
// template <template <typename...> class policy_t,
// 	  typename... Args>
// struct is_implicit_bdf2_residual_standard_policy<
//   policy_t<Args...>,
//   typename std::enable_if<
//     std::is_same<policy_t<Args...>,
// 		 ode::policy::ImplicitBDF2ResidualStandardPolicy<
// 		 Args...>
// 		 >::value
// 			  >::type
//   > : std::true_type{};

  
// //-----------------------------------------------------------------
// // METAF TO CHECK JACOBIAN POLICY IS STANDARD
// //-----------------------------------------------------------------
// template<typename policy_t, typename enable = void>
// struct is_implicit_bdf2_jacobian_standard_policy
//   : std::false_type{};

  
// template <template <typename...> class policy_t,
//   	  typename... Args>
// struct is_implicit_bdf2_jacobian_standard_policy<
//   policy_t<Args...>,
//   typename std::enable_if<
//     std::is_same<policy_t<Args...>,
// 		 ode::policy::ImplicitBDF2JacobianStandardPolicy<
// 		 Args...>
// 		 >::value
// 			  >::type
//   > : std::true_type{};
// //----------------------------------------------------------------

  
} // namespace meta
} // namespace core
}//end namespace rompp
#endif
