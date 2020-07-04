

#ifndef ODE_ode_admissible_system_implicit_ode_regular_stepper_HPP_
#define ODE_ode_admissible_system_implicit_ode_regular_stepper_HPP_

namespace pressio{ namespace ode{ namespace meta {

template<typename T, typename enable = void>
struct admissible_system_implicit_ode_regular_stepper : std::false_type{};

template<typename T>
struct admissible_system_implicit_ode_regular_stepper<
  T,
  mpl::enable_if_t<
    ::pressio::containers::meta::has_scalar_typedef<T>::value and
    ::pressio::ode::meta::has_state_typedef<T>::value and
    ::pressio::ode::meta::has_velocity_typedef<T>::value and
    ::pressio::ode::meta::has_jacobian_typedef<T>::value and
    /////// residual methods
    has_const_create_velocity_method_return_result<
      T, typename T::velocity_type >::value and
    has_const_velocity_method_accept_state_time_result_return_void<
      T, 
      typename T::state_type, 
      typename T::scalar_type, 
      typename T::velocity_type
      >::value and
    /////// jacobian methods 
    has_const_create_jacobian_method_return_result<
      T, typename T::jacobian_type >::value and
    has_const_jacobian_method_accept_state_time_result_return_void<
      T, 
      typename T::state_type, 
      typename T::scalar_type, 
      typename T::jacobian_type
      >::value    
    >
  > : std::true_type{};


#ifdef PRESSIO_ENABLE_TPL_PYBIND11
template<typename T>
struct admissible_system_implicit_ode_regular_stepper<
  T,
  mpl::enable_if_t<
    mpl::is_same<T, pybind11::object>::value
    >
  > : std::true_type{};
#endif

}}} // namespace pressio::ode::meta
#endif