
#ifndef ODE_IMPLICIT_BDF2_RESIDUAL_POLICY_BASE_HPP_
#define ODE_IMPLICIT_BDF2_RESIDUAL_POLICY_BASE_HPP_

#include "ode_ConfigDefs.hpp"

namespace ode{
namespace policy{
  
template <template <typename...> class derived_type,
	  typename state_type,
	  typename residual_type,
	  typename model_type,
	  typename time_type,
	  typename ... Args>
class implicitBDF2ResidualPolicyBase
{
public:
  void compute(const state_type & y,
	       const state_type & ynm1,
	       const state_type & ynm2,
	       residual_type & R,
	       model_type & model,
	       time_type t,
	       time_type dt){
    this->underlying().computeImpl(y, ynm1, ynm2, R, model, t, dt);
  } 

private:
  using derived_t = derived_type<state_type,residual_type,
				 model_type, time_type, Args...>;
  friend derived_t; 
  implicitBDF2ResidualPolicyBase() = default;
  ~implicitBDF2ResidualPolicyBase() = default;
  
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