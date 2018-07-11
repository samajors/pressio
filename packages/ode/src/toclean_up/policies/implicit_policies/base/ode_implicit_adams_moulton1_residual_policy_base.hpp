
#ifndef ODE_IMPLICIT_ADAMS_MOULTON1_RESIDUAL_POLICY_BASE_HPP_
#define ODE_IMPLICIT_ADAMS_MOULTON1_RESIDUAL_POLICY_BASE_HPP_

#include "ode_ConfigDefs.hpp"

namespace ode{
namespace policy{
  
template <template <typename...> class derived_type,
	  typename state_type,
	  typename residual_type,
	  typename model_type,
	  typename time_type,
	  typename ... Args>
class implicitAdamsMoulton1ResidualPolicyBase
{
public:
  void compute(const state_type & y,
	       const state_type & ynm1,
	       const residual_type & Rnm1,
	       residual_type & R,
	       model_type & model,
	       time_type t,
	       time_type dt){
    this->underlying().computeImpl(y, ynm1, Rnm1, R, model, t, dt);
  } 

  void computeModelResidual(const state_type & y,
			    residual_type & R,
			    model_type & model,
			    time_type t){
    this->underlying().computeModelResidualImpl(y, R, model, t);
  } 
  
private:
  using derived_t = derived_type<state_type,residual_type,
				 model_type, time_type, Args...>;
  friend derived_t; 
  implicitAdamsMoulton1ResidualPolicyBase() = default;
  ~implicitAdamsMoulton1ResidualPolicyBase() = default;
  
  derived_t & underlying(){
    return static_cast<derived_t& >( *this );
  }
  derived_t const & underlying() const{
    return static_cast<derived_t const & >( *this );
  }
};

}//end namespace polices
}//end namespace ode  
#endif 