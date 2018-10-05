
#ifndef ODE_POLICIES_BASE_EXPLICIT_RESIDUAL_POLICY_BASE_HPP_
#define ODE_POLICIES_BASE_EXPLICIT_RESIDUAL_POLICY_BASE_HPP_

#include "../../ode_forward_declarations.hpp"

namespace rompp{ namespace ode{ namespace policy{
    
template <typename derived_t>
class ExplicitResidualPolicyBase
  : private core::details::CrtpBase<
  ExplicitResidualPolicyBase<derived_t>>{
public:

  //--------------------------------------------------
  // model object is passed
  template <typename state_type,
	    typename space_residual_type,
	    typename model_type,
	    typename scalar_type>
  void operator()(const state_type & y,
		  space_residual_type & R,
		  const model_type & model,
		  scalar_type t) const{
    this->underlying()(y, R, model, t);
  }
  //--------------------------------------------------

  // no model object is passed, it is assumed
  // that the model info is contained within policy
  template <typename state_type,
	    typename space_residual_type,
	    typename scalar_type>
  void operator()(const state_type & y,
		  space_residual_type & R,
		  scalar_type t) const{
    this->underlying()(y, R, t);
  }
  //--------------------------------------------------
  
private:
  friend derived_t;
  friend core::details::CrtpBase<
    ExplicitResidualPolicyBase<derived_t>>;
  ExplicitResidualPolicyBase() = default;
  ~ExplicitResidualPolicyBase() = default;
  
};//end class
  
}//end namespace polices
}//end namespace ode  
}//end namespace rompp
#endif 