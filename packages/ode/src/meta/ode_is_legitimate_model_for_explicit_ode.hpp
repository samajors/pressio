
#ifndef ODE_IS_LEGITIMATE_MODEL_FOR_EXPLICIT_ODE_HPP_
#define ODE_IS_LEGITIMATE_MODEL_FOR_EXPLICIT_ODE_HPP_

#include "../../../containers/src/meta/containers_meta_has_scalar_typedef.hpp"
#include "ode_has_state_typedef.hpp"
#include "ode_has_velocity_typedef.hpp"
#include "ode_model_has_all_needed_velocity_methods.hpp"

namespace pressio{ namespace ode{ namespace meta {

template<
  typename model_type,
  typename enable = void
  >
struct is_legitimate_model_for_explicit_ode : std::false_type{};

template<typename model_type>
struct is_legitimate_model_for_explicit_ode<
  model_type,
  mpl::enable_if_t<
    ::pressio::containers::meta::has_scalar_typedef<model_type>::value and
    ::pressio::containers::meta::has_state_typedef<model_type>::value and
    ::pressio::containers::meta::has_velocity_typedef<model_type>::value and
    model_has_needed_velocity_methods<
      model_type,
      typename model_type::state_type,
      typename model_type::velocity_type,
      typename model_type::scalar_type>::value
    >
  > : std::true_type{};

//-------------------------------------------------------

}}} // namespace pressio::ode::meta
#endif
