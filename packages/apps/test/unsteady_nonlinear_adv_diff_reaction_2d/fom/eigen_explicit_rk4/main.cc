
#include "CORE_ALL"
#include "ODE_ALL"
#include "APPS_UNSTEADYNONLINADVDIFFREACTION2D"
#include "../gold_states_explicit.hpp"

constexpr double eps = 1e-12;
std::string checkStr {"PASSED"};

template <typename T>
void checkSol(const T & y,
	      const std::vector<double> & trueS){
  if (trueS.empty()) {
    std::cout << " true solution not found, empty " << std::endl;
    checkStr = "FAILED";
  }
  for (size_t i=0; i<trueS.size(); i++){
    if (std::abs(y[i] - trueS[i]) > eps) checkStr = "FAILED";
  }
}

constexpr bool do_print = true;

struct Observer{
  Observer() = default;

  template <typename T>
  void operator()(size_t step,
  		  double t,
  		  const T & y)
  {
    if (do_print){
      if (step % 5 == 0){
    	std::ofstream file;
    	file.open( "sol_" + std::to_string(step) + ".txt" );
    	for(auto i=0; i < y.size(); i++){
    	  file << std::fixed << std::setprecision(14) << y[i] ;
    	  file << std::endl;
    	}
    	file.close();
      }
    }
  }
};


int main(int argc, char *argv[]){
  using app_t		= rompp::apps::UnsteadyNonLinAdvDiffReac2dEigen;
  using scalar_t	= typename app_t::scalar_type;
  using app_state_t	= typename app_t::state_type;
  using app_residual_t	= typename app_t::residual_type;
  constexpr auto zero = ::rompp::core::constants::zero<scalar_t>();

  constexpr int Nx = 11, Ny = Nx*2-1;
  app_t appobj(Nx, Ny);
  appobj.setup();
  auto y0n = appobj.getInitialState();
  const auto r0n = appobj.residual(y0n, zero);

  if (do_print){
    auto X = appobj.getX(); auto Y = appobj.getY();
    std::ofstream file; file.open( "xy.txt" );
    for(auto i=0; i < X.size(); i++){
      file << std::setprecision(14)
	   << X[i] << " " << Y[i];
      file << std::endl;
    }
    file.close();
  }

  using ode_state_t = rompp::core::Vector<app_state_t>;
  using ode_res_t   = rompp::core::Vector<app_residual_t>;
  ode_state_t y(y0n);
  ode_res_t r(r0n);

  constexpr auto ode_case = rompp::ode::ExplicitEnum::RungeKutta4;
  using stepper_t = rompp::ode::ExplicitStepper<
    ode_case, ode_state_t, app_t, ode_res_t>;
  stepper_t stepperObj(y, appobj, r);

  // integrate in time
  constexpr scalar_t dt = 0.001;
  constexpr scalar_t fint = 0.5;
  constexpr auto Nsteps = static_cast<unsigned int>(fint/dt);
  Observer obs;
  rompp::ode::integrateNSteps(stepperObj, y, 0.0, dt, Nsteps, obs);
  std::cout << std::setprecision(14) << *y.data() << std::endl;
  {
    using namespace rompp::apps::test;
    checkSol(y,
  	     NonLinAdvDiffReac2dExpGoldStates<ode_case>::get(Nx, Ny, dt, fint));
  }

  std::cout << checkStr << std::endl;
  return 0;
}