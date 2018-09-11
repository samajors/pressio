
#include "CORE_ALL"
#include "ODE_ALL"
#include "SOLVERS_LINEAR"
#include "SOLVERS_NONLINEAR"

#include "apps_burgers1d_epetra.hpp"
//#include "ode_observer.hpp"
//#include "../apps_helper_ode.hpp"


// template<typename T>
// void printSol(std::string mess, const T & y){
//   std::cout << mess << std::endl;
//   for (int i=0; i<y.size(); ++i)
//     std::cout << std::setprecision(14) << y[i]  << " ";
//   std::cout << std::endl;
// }


int main(int argc, char *argv[])
{
  //-------------------------------
  // define native types
  using app_state_t = apps::Burgers1dEpetra::state_type;
  using app_space_residual_t = apps::Burgers1dEpetra::space_residual_type;
  using app_jac_t = apps::Burgers1dEpetra::jacobian_type;
  using scalar_t = apps::Burgers1dEpetra::scalar_type;
  using target_app_t = apps::Burgers1dEpetra;
  
  //-------------------------------
  // MPI init
  MPI_Init(&argc,&argv);
  int rank; // My process ID
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Epetra_MpiComm Comm(MPI_COMM_WORLD);

  assert( Comm.NumProc() == 4);
    
  //-------------------------------
  // create app object
  std::vector<double> mu({5.0, 0.02, 0.02});
  target_app_t appObj(mu, 20, &Comm);
  appObj.setup();
  auto & y0n = appObj.getInitialState();
  auto & r0n = appObj.getInitialResidual();
  auto & j0n = appObj.getInitialJacobian();

  //-------------------------------
  // types for ode
  using ode_state_t = core::Vector<app_state_t>;
  using ode_res_t = ode_state_t;
  using ode_jac_t = core::Matrix<app_jac_t>;

  ode_state_t y(y0n);
  ode_res_t r(r0n);

  // stepper needs to know about types for doing time integration
  using stepper_t = ode::ExplicitEulerStepper<
    ode_state_t, ode_res_t, target_app_t>;
  stepper_t stepperObj(appObj, y, r);
  
  // integrate in time 
  scalar_t fint = 35;
  scalar_t dt = 0.01;
  ode::integrateNSteps(stepperObj, y, 0.0, dt, static_cast<unsigned int>(fint/dt));
  
  y.data()->Print(std::cout << std::setprecision(14));

  std::vector<scalar_t> trueS = { 5.0209814000128,   5.044067908724,  \
				  5.0694601439541,  5.0973757621592,\
				  5.1280505161248,  5.1617393082963,\
				  5.1987172243105,  5.2392805237326,\
				  5.2837475435221,  5.3324594086071, \
				  5.3857802812742,  5.4440964817745,\
				  5.5078129073313,  5.5773432783592,\
				  5.6530870659136,  5.7353794504736,\
				  5.8243903774842,  5.9199350492773,\
				  6.0211454752168,  6.1259551255163};

  const double eps = 1e-12;
  int shift = rank*4;
  if (rank==0){
    assert(std::abs(y[0] - trueS[0]) < eps);
    assert(std::abs(y[1] - trueS[1]) < eps);
    assert(std::abs(y[2] - trueS[2]) < eps);
    assert(std::abs(y[3] - trueS[3]) < eps);
    assert(std::abs(y[4] - trueS[4]) < eps);
  }

  if (rank==1){
    assert(std::abs(y[0] - trueS[5]) < eps);
    assert(std::abs(y[1] - trueS[6]) < eps);
    assert(std::abs(y[2] - trueS[7]) < eps);
    assert(std::abs(y[3] - trueS[8]) < eps);
    assert(std::abs(y[4] - trueS[9]) < eps);
  }

  if (rank==2){
    assert(std::abs(y[0] - trueS[10]) < eps);
    assert(std::abs(y[1] - trueS[11]) < eps);
    assert(std::abs(y[2] - trueS[12]) < eps);
    assert(std::abs(y[3] - trueS[13]) < eps);
    assert(std::abs(y[4] - trueS[14]) < eps);
  }

  if (rank==3){
    assert(std::abs(y[0] - trueS[15]) < eps);
    assert(std::abs(y[1] - trueS[16]) < eps);
    assert(std::abs(y[2] - trueS[17]) < eps);
    assert(std::abs(y[3] - trueS[18]) < eps);
    assert(std::abs(y[4] - trueS[19]) < eps);
  }
  
    
  MPI_Finalize();
  return 0;
}



  // // stepper
  // using stepper_t = ode::ImplicitEulerStepper<
  //   state_t, res_t, jac_t, target_app_t>;
  // // using stepper_t = ode::ExplicitRungeKutta4Stepper<
  // //   state_t, space_res_t, scalar_t, target_app_t>;
  // stepper_t stepperObj(appObj, y, r);

  // // Create linear solver using a valid solver type
  // auto solver = solvers::NonLinearSolvers::createSolver<solvers::nonlinear::NewtonRaphson>();
    
  // // integrate in time 
  // snapshot_collector collObj;
  // scalar_t final_t = 35;
  // scalar_t dt = 0.01;
  // ode::integrateNSteps(stepperObj, y, 0.0, dt, static_cast<unsigned int>(dt/dt), solver);
  // //y.data()->Print(std::cout << std::setprecision(14));
  // // // //printSol("Exp Euler", y0);


  // {
  //   //-------------------------------
  //   // wrapper types 
  //   using state_t = core::Vector<native_state_t>;
  //   using jac_t = core::Matrix<native_jac_t>;
  //   using space_res_t = core::Vector<native_space_residual_t>;

  //   state_t y(y0n);
  //   space_res_t r(r0n);

  //   // stepper
  //   using stepper_t = ode::ExplicitEulerStepper<
  //     state_t, space_res_t, target_app_t>;
  //   // using stepper_t = ode::ExplicitRungeKutta4Stepper<
  //   //   state_t, space_res_t, scalar_t, target_app_t>;
  //   stepper_t stepperObj(appObj, y, r);

  //   // integrate in time 
  //   snapshot_collector collObj;
  //   scalar_t final_t = 35;
  //   scalar_t dt = 0.01;
  //   ode::integrateNSteps(stepperObj, y, 0.0, dt, static_cast<unsigned int>(final_t/dt), collObj);
  //   y.data()->Print(std::cout << std::setprecision(14));
  //   // // //printSol("Exp Euler", y0);
  // }
