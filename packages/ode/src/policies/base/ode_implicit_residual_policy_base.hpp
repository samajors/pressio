
#ifndef ODE_IMPLICIT_RESIDUAL_POLICY_BASE_HPP_
#define ODE_IMPLICIT_RESIDUAL_POLICY_BASE_HPP_

#include "ode_ConfigDefs.hpp"

namespace ode{
namespace policy{   
  
template <typename derived_t, int numAuxStates, int numAuxRHS>
class implicitResidualPolicyBase
  : private core::details::crtpBase<
       implicitResidualPolicyBase<derived_t, numAuxStates, numAuxRHS>>
{
public:

  template <typename state_type,
  	    typename residual_type,
  	    typename model_type,
  	    typename time_type,
	    int T = numAuxRHS,
  	    typename std::enable_if<T==0>::type * = nullptr>
  void compute(const state_type & y,
  	      residual_type & R,
  	      const std::array<state_type, numAuxStates> & auxYs,
  	      model_type & model,
  	      time_type t,
  	      time_type dt)
  {
    this->underlying().computeImpl(y, R, auxYs, model, t, dt);
  }
  //---------------------------------------------------------------
  
  template <typename state_type,
	    typename residual_type,
	    typename model_type,
	    typename time_type,
	    int T = numAuxRHS,
	    typename std::enable_if<T!=0>::type * = nullptr>
  void compute(const state_type & y,
  	       residual_type & R,
  	       const std::array<state_type, numAuxStates> & auxYs,
  	       const std::array<residual_type, T> & auxRHSs,
  	       model_type & model,
  	       time_type t,
  	       time_type dt)
  {
    this->underlying().computeImpl(y, R, auxYs, auxRHSs, model, t, dt);
  }
  
private:
  friend derived_t;
  friend core::details::crtpBase<
    implicitResidualPolicyBase<derived_t, numAuxStates, numAuxRHS>>;
  
  implicitResidualPolicyBase() = default;
  ~implicitResidualPolicyBase() = default;
  
};//end class

  
}//end namespace polices
}//end namespace ode  
#endif