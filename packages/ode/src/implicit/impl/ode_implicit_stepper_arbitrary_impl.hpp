
#ifndef ODE_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_ARBITRARY_HPP_
#define ODE_IMPLICIT_STEPPERS_IMPLICIT_STEPPER_ARBITRARY_HPP_

namespace pressio{ namespace ode{ namespace implicitmethods{ namespace impl{

template<
  typename scalar_t,
  typename ode_state_type,
  typename ode_residual_type,
  typename ode_jacobian_type,
  typename system_type,
  typename order_setter_t, 
  typename tot_n_setter_t, 
  typename residual_policy_t, 
  typename jacobian_policy_t
  >
class StepperArbitrary
: public ::pressio::ode::implicitmethods::StepperBase< 
  StepperArbitrary<scalar_t, ode_state_type, ode_residual_type, ode_jacobian_type, 
  system_type, order_setter_t, tot_n_setter_t, residual_policy_t, jacobian_policy_t> 
  >
{
public:
  static constexpr bool is_implicit = true;
  static constexpr bool is_explicit = false;

  using tag_name = ::pressio::ode::implicitmethods::Arbitrary;

  // numAuxStates is the number of auxiliary states needed, so all other beside y_n
  static constexpr std::size_t numAuxStates = tot_n_setter_t::value - 1;

  using system_wrapper_t  = ::pressio::ode::impl::OdeSystemWrapper<system_type>;
  using aux_states_t = ::pressio::ode::AuxStatesContainer<false, ode_state_type, numAuxStates>;

  using standard_res_policy_t = ::pressio::ode::implicitmethods::policy::ResidualStandardPolicyForArbitraryStepper<
    ode_state_type, system_type, ode_residual_type>;
  using standard_jac_policy_t = ::pressio::ode::implicitmethods::policy::JacobianStandardPolicyForArbitraryStepper<
    ode_state_type, system_type, ode_jacobian_type>;

public:
  // these need to be here because are detected by solver
  using scalar_type	= scalar_t;
  using state_type	= ode_state_type;
  using residual_type	= ode_residual_type;
  using jacobian_type	= ode_jacobian_type;


public:
  StepperArbitrary() = delete;
  ~StepperArbitrary() = default;
  StepperArbitrary(const StepperArbitrary & other)  = delete;
  StepperArbitrary & operator=(const StepperArbitrary & other)  = delete;
  StepperArbitrary(StepperArbitrary && other)  = delete;
  StepperArbitrary & operator=(StepperArbitrary && other)  = delete;


  StepperArbitrary(const ode_state_type & state,
	  const system_type & systemObj,
	  const residual_policy_t & resPolicyObj,
	  const jacobian_policy_t & jacPolicyObj)
    : sys_{systemObj}, 
      auxStates_{state}, 
      residual_obj_{resPolicyObj},
      jacobian_obj_{jacPolicyObj}
    {}

  // cstr for standard residual and jacob policies
  template <
    typename T1 = residual_policy_t,
    typename T2 = jacobian_policy_t,
    ::pressio::mpl::enable_if_t<
      std::is_default_constructible<T1>::value and
      std::is_default_constructible<T2>::value,
      int > = 0
    >
  StepperArbitrary(const ode_state_type & state,  
                   const system_type & systemObj)
    : sys_{systemObj}, 
      auxStates_{state}
  {}

public:
  ::pressio::ode::types::stepper_order_t order() const
  {
    return order_setter_t::value;
  }

  residual_type createResidual() const{
    return this->residual_obj_.create(sys_.get());
  }

  jacobian_type createJacobian() const{
    return this->jacobian_obj_.create(sys_.get());
  }

  void residual(const state_type & odeState, residual_type & R,
    ::pressio::Norm normKind, scalar_t & normValue) const
  {
    this->residual_obj_.compute(odeState, this->auxStates_, this->sys_.get(),
      this->t_, this->dt_, this->step_, R,
      normKind, normValue);
  }

  void jacobian(const state_type & odeState, jacobian_type & J) const
  {
    this->jacobian_obj_.compute(odeState, this->auxStates_, this->sys_.get(),
      this->t_, this->dt_, this->step_, J);
  }

  template<typename solver_type>
  void doStep(ode_state_type & odeState,
	      const scalar_t & t,
	      const scalar_t & dt,
	      const types::step_t & step,
	      solver_type & solver)
  {
    this->dt_ = dt;
    this->t_ = t;
    this->step_ = step;

    constexpr auto nAux = decltype(this->auxStates_)::size();
    this->updateAuxiliaryStorage<nAux>(odeState);
    solver.solve(*this, odeState);
  }

private:
  // methods to do updating on the storage of previous states
  template<std::size_t nAux, mpl::enable_if_t<nAux==1, int > = 0>
  void updateAuxiliaryStorage(const ode_state_type & odeState){
    // copy y_n into y_n-1
    auto & y_nm1 = this->auxStates_.get(ode::nMinusOne());
    ::pressio::ops::deep_copy(y_nm1, odeState);
  }

  // when we have two aux states,
  template<std::size_t nAux, mpl::enable_if_t<nAux==2, int> = 0>
  void updateAuxiliaryStorage(const ode_state_type & odeState){
    // copy y_n-1 into y_n-2
    auto & y_nm1 = this->auxStates_.get(ode::nMinusOne());
    auto & y_nm2 = this->auxStates_.get(ode::nMinusTwo());
    ::pressio::ops::deep_copy(y_nm2, y_nm1);
    // copy y_n into y_n-1
    ::pressio::ops::deep_copy(y_nm1, odeState);
  }

  // when we have three aux states,
  template<std::size_t nAux, mpl::enable_if_t<nAux==3, int> = 0>
  void updateAuxiliaryStorage(const ode_state_type & odeState){
    auto & y_nm1 = this->auxStates_.get(ode::nMinusOne());
    auto & y_nm2 = this->auxStates_.get(ode::nMinusTwo());
    auto & y_nm3 = this->auxStates_.get(ode::nMinusThree());
    ::pressio::ops::deep_copy(y_nm3, y_nm2);
    ::pressio::ops::deep_copy(y_nm2, y_nm1);
    ::pressio::ops::deep_copy(y_nm1, odeState);
  }

  // when we have four aux states,
  template<std::size_t nAux, mpl::enable_if_t<nAux==4, int> = 0>
  void updateAuxiliaryStorage(const ode_state_type & odeState){
    auto & y_nm1 = this->auxStates_.get(ode::nMinusOne());
    auto & y_nm2 = this->auxStates_.get(ode::nMinusTwo());
    auto & y_nm3 = this->auxStates_.get(ode::nMinusThree());
    auto & y_nm4 = this->auxStates_.get(ode::nMinusFour());
    ::pressio::ops::deep_copy(y_nm4, y_nm3);
    ::pressio::ops::deep_copy(y_nm3, y_nm2);
    ::pressio::ops::deep_copy(y_nm2, y_nm1);
    ::pressio::ops::deep_copy(y_nm1, odeState);
  }

private:
  scalar_t t_  = {};
  scalar_t dt_ = {};
  types::step_t step_  = {};
  system_wrapper_t sys_;
  aux_states_t auxStates_;

  // conditionally set the type of the object knowing how to compute residual
  // if we have a standard policy, then it takes a copy
  // if we have a user-defined policy, we take a const & to it
  typename std::conditional<
    mpl::is_same<standard_res_policy_t, residual_policy_t>::value,
    const residual_policy_t,
    const residual_policy_t &
    >::type residual_obj_;

  // conditionally set the type of the object knowing how to compute jacobian
  // if we have a standard policy, then it takes a copy
  // if we have a user-defined policy, we take a const & to it
  typename std::conditional<
    mpl::is_same<standard_jac_policy_t, jacobian_policy_t>::value,
    const jacobian_policy_t,
    const jacobian_policy_t &
    >::type jacobian_obj_;

};//end class

}}}} 
#endif