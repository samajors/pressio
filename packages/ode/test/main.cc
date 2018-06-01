
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <type_traits>
#include <cmath>
#include <fstream>
#include <cassert>

#include "ode_euler_stepper.hpp"
#include "ode_rk4_stepper.hpp"
#include "ode_integrate_n_steps.hpp"

#include "vector/core_vector_traits.hpp"
#include "vector/core_vector_epetra.hpp"
#include "vector/core_vector_serial_arbitrary.hpp"
#include "vector/core_vector_std.hpp"
#include "vector/core_vector_eigen.hpp"


using vecD = std::vector<double>;
using ui_t = unsigned int;


class burg1d
{
public:
  using state_type = std::vector<double>;

public:  
  burg1d(vecD params) : mu_(params){}

  void setup(){
    dx_ = (xR_ - xL_)/static_cast<double>(Ncell_);
    xGrid_.resize(Ncell_,0.0);
    for (ui_t i=0; i<Ncell_; ++i){
      xGrid_[i] = dx_*i + dx_*0.5;
    };
    U_.resize(Ncell_, 1.0);
  };

  state_type viewInitCond(){
    return U_;
  };
  
  void operator() ( const state_type & u,
		    state_type & R,
		    const double /* t */ )
  {
    R[0] = (0.5 * ( mu_[0]*mu_[0] - u[0]*u[0] ) )/dx_;
    for (ui_t i=1; i<Ncell_; ++i){
      R[i] = ( 0.5*(u[i-1]*u[i-1] - u[i]*u[i]) )/dx_;
    }
    for (ui_t i=0; i<Ncell_; ++i){
      R[i] += mu_[1]*exp(mu_[2]*xGrid_[i]);
    }    
  }

  void integrate()
  {
    // int nsteps = 2500;
    // vecD rhs(Ncell_,0.0);
    // double dt = tfinal_/static_cast<double>(nsteps);

    // std::ofstream file;
    // file.open( "out.txt" );
    // for (ui_t step=0; step<nsteps; ++step)
    // {
    //   (*this)(U_, rhs, step*dt);
    //   for (ui_t i=0; i<Ncell_; ++i){
    //   	U_[i] += dt*(rhs[i]);
    // 	if (step % 50 == 0 || step==0)
    // 	 file << std::fixed << std::setprecision(10) << U_[i] << " ";
    //   }      
    //   if (step % 50 == 0)
    // 	file << std::endl;
    // }
    // file.close();
  }

private:
  
  const double xL_ = 0.0;
  const double xR_ = 100.0;
  const ui_t Ncell_ = 10;
  vecD mu_;
  const double t0 = 0.0;
  const double tfinal_ = 35.0;
  double dx_;
  vecD xGrid_;
  state_type U_;
};
//==================================================

struct stateResizer{
  // has to be default constructible
  
  void operator()(const vecD  & source,
		  vecD & dest)
  {
    if ( dest.size()!=source.size() )
      dest.resize(source.size());
  };
};


struct snapshot_collector{
  using matrix = std::vector<vecD>;
  using state_t = vecD;
  
  matrix snaps_;
  size_t count_;
  void operator()(size_t step, double t, const state_t & x){
    if (step % 50 ==0 ){
      snaps_.emplace_back(x);
      count_++;
    }
    std::cout << step << " " << t << std::endl;
  }
  size_t getCount(){
    return count_;
  };

  void print()
  {
    std::ofstream file;
    file.open( "out.txt" );
    for (ui_t step=0; step<count_; ++step)
    {
      for (auto & it : snaps_[step])
    	 file << std::fixed << std::setprecision(10) << it << " "; 
      file << std::endl;
    }
    file.close();    
  };
};


// template <typename state_type, typename oapp>
// class GP()
// {
// private:
//   state_type myY_;
//   oapp * appPtr_;
    
// public:
//   GP(state_type & src, ...)
//     : myY(src), ... {}
//   ~GP(){}
  
//   void operator()()
//   {
//     (*oappPtr)()( V' * y )
//   }

//   void run()
//   {
//     ode::eulerStepper<state_t,state_t,double,stateResizer> myStepper;
//     ode::integrateNSteps(myStepper, *this, myY, snColl, 0.0, 0.0035, 10000);    
//   }
// };


int main(int argc, char *argv[])
{
  const vecD mu{5.0, 0.02, 0.02};
  burg1d app(mu);
  app.setup();

  using state_t = burg1d::state_type;
  state_t U = app.viewInitCond();
  snapshot_collector snColl;  
  {
    ode::eulerStepper<state_t,state_t,double,stateResizer> myStepper;
    ode::integrateNSteps(myStepper, app, U, snColl, 0.0, 0.0035, 10000);
    //ode::integrateNSteps(myStepper, app, U, 0.0, 0.0035, 10000);
    // for (auto & it : U)
    //   std::cout << it  << std::endl;
    std::cout << snColl.getCount() << std::endl;
    snColl.print();
  }

  // //----------------------------------------  
  // // lets assume we have snapshot matrix V
  
  // using myvec_t = core::vector<state_t>;
  // myvec_t y(U); // y contains the initial condition
  // y.matmul(V); // something like this

  // GP gpSolver(y, app, ...);
  // gpSolver.run();
  
  // // auto const * data = gigi.view();
  // // for (auto & it : *data)
  // //   std::cout << it  << std::endl;
  
  return 0;
}


  // bool success = true;
  // std::cout << std::boolalpha << success;

  // MPI_Init(&argc,&argv);
  // int rank; // My process ID
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Epetra_MpiComm Comm(MPI_COMM_WORLD);

  // int MyPID = Comm.MyPID();
  // int NumProc = Comm.NumProc();

  // int NumMyElements = 10000;
  // int NumMyElements1 = NumMyElements; // Needed for localmap
  // int NumGlobalElements = NumMyElements*NumProc+EPETRA_MIN(NumProc,3);
  // if (MyPID < 3) NumMyElements++;
  // int IndexBase = 0;
  // int ElementSize = 7;

  // Epetra_LocalMap *LocalMap = new Epetra_LocalMap(NumMyElements1, IndexBase,Comm);
  // Epetra_Vector A(*LocalMap);

  // // epetramock::evector obj;
  // //obj.print();
 
  //   MPI_Finalize();