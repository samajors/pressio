
#ifndef ODE_EXPLICIT_EULER_STEPPER_HPP_
#define ODE_EXPLICIT_EULER_STEPPER_HPP_

#include "./base/ode_explicit_stepper_base.hpp"


namespace ode{

template<typename state_type,
	 typename rhs_type,
	 typename scalar_type,
	 typename state_resizer_fnctor_type,
	 typename model_type,
	 typename residual_policy_type
	 >
class explicitEulerStepper<state_type, rhs_type, scalar_type,
			   state_resizer_fnctor_type, model_type,
			   residual_policy_type, 
			   typename
			   std::enable_if< !std::is_void<state_type>::value &&
				   core::meta::is_default_constructible<state_resizer_fnctor_type>::value
					   >::type
			   >
  : public explicitStepperBase< explicitEulerStepper<state_type,rhs_type,scalar_type,
						     state_resizer_fnctor_type, model_type,
						     residual_policy_type
						     >
				>
{
public :
  using stepper_t = explicitEulerStepper<state_type,rhs_type,scalar_type,
					 state_resizer_fnctor_type, model_type,
					 residual_policy_type>;
  using stepper_base_t = explicitStepperBase<stepper_t>;
  
  //(de)constructors
  template < typename U = residual_policy_type>
  explicitEulerStepper(model_type & model,
  		       U & res_policy_obj, 
  		       typename std::enable_if< !std::is_same<U,
  		       ode::policy::explicitEulerStandardResidual<state_type, rhs_type,
		                                                  model_type, details::time_type>
		       >::value >::type * = 0)
    : stepper_base_t(model, res_policy_obj)
  {}

  template < typename U = residual_policy_type>
  explicitEulerStepper(model_type & model,
  		       typename
  		       std::enable_if< std::is_same<U,
  		       ode::policy::explicitEulerStandardResidual<state_type, rhs_type,
		                                                  model_type, details::time_type>
  		       >::value >::type * = 0)
    : stepper_base_t( model, U() ) 
  {}

  ~explicitEulerStepper()
  {}

  
  // methods
  void doStepImpl(state_type & y_inout,
		  ode::details::time_type t,
		  ode::details::time_type dt )
  {
    this->myResizer_(y_inout, RHS_);

    //eval RHS
    this->residual_policy_obj_.compute(y_inout, RHS_, *(this->model_), t);
    
    // TODO: if possible, change to use native operations of the target data types
    // out = in + dt * rhs    
    for (decltype(y_inout.size()) i=0; i < y_inout.size(); i++){
      y_inout[i] += dt*RHS_[i];
    }
  }

private:
  rhs_type RHS_;
  
  // additional members inherited from the base class:
  //   model_, myResizer_, residual_policy_t
  
}; //end class






// template<typename state_type,
// 	 typename rhs_type,
// 	 typename scalar_type,
// 	 typename state_resizer_fnctor_type,
// 	 typename model_type>
// class explicitEulerStepper<state_type, rhs_type, scalar_type,
// 			   state_resizer_fnctor_type, model_type, void,
// 			   typename 
// 		   std::enable_if< !std::is_void<state_type>::value &&
// 				   core::meta::is_default_constructible<state_resizer_fnctor_type>::value &&
// 				   !std::is_void<residual_policy_type>::value
// 				   >::type
// 		   >
// {

// };
  

}//end namespace
#endif 




  
  // #ifndef DOXYGEN_SKIP
    // #else
    // typedef explicit_stepper_base< euler< ... > , ... > stepper_base_type;
    // #endif
    // typedef typename stepper_base_type::state_type state_type;
    // typedef typename stepper_base_type::value_type value_type;
    // typedef typename stepper_base_type::deriv_type deriv_type;
    // typedef typename stepper_base_type::time_type time_type;
    // typedef typename stepper_base_type::algebra_type algebra_type;
    // typedef typename stepper_base_type::operations_type operations_type;
    // typedef typename stepper_base_type::resizer_type resizer_type;

    // #ifndef DOXYGEN_SKIP
    // typedef typename stepper_base_type::stepper_type stepper_type;
    // typedef typename stepper_base_type::wrapped_state_type wrapped_state_type;
    // typedef typename stepper_base_type::wrapped_deriv_type wrapped_deriv_type;
    // #endif 
