
#include <gtest/gtest.h>
#include "pressio_rom.hpp"

// NOTE: here it does not matter to leave all empty since this
// is just for doing type checking
struct ValidApp{
  using scalar_type   = double;
  using state_type    = std::vector<scalar_type>;
  using residual_type = state_type;
  using jacobian_type = std::vector<std::vector<scalar_type>>;
  using dense_matrix_type = std::vector<std::vector<scalar_type>>;

public:

  template <typename step_t, typename ... Args>
  void timeDiscreteResidual(const step_t & step,
  			    const scalar_type & time,
			    const scalar_type & dt,
  			    residual_type & R,
  			    Args & ... states) const
  {
    // forward to whatever approriate impl method, e. g.
    // timeDiscreteResidualImpl<step_t>( step, time, f, std::forward<Args>(states)... );
  }

  template <typename step_t, typename ... Args>
  void applyTimeDiscreteJacobian(const step_t & step,
				 const scalar_type & time,
				 const scalar_type & dt,
				 const dense_matrix_type & B,
				 int id,
				 dense_matrix_type & A,
				 Args & ... states) const
  {}


  residual_type createTimeDiscreteResidualObject(const state_type & stateIn) const
  {
    residual_type R;
    return R;
  }

  dense_matrix_type createApplyTimeDiscreteJacobianObject(const state_type & stateIn,
							  const dense_matrix_type & B) const
  {
    dense_matrix_type A;
    return A;
  }

};

TEST(rom_lspg_meta, validResidualAPI){
  using namespace pressio;
  using app_t    = ValidApp;
  static_assert( rom::meta::model_meets_residual_api_for_unsteady_lspg<app_t>::value,"");

  static_assert( rom::meta::is_legitimate_model_for_unsteady_lspg<app_t>::value,"");
  // assert that it does not meet velocity api
  static_assert( !rom::meta::model_meets_velocity_api_for_unsteady_lspg<app_t>::value,"");
}
