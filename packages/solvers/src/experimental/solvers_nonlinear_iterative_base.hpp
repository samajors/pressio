
#ifndef SOLVERS_LINEAR_ITERATIVE_BASE_HPP_
#define SOLVERS_LINEAR_ITERATIVE_BASE_HPP_

namespace solvers {


/**
 * @brief Base class for nonlinear iterative solver implemented through CRTP
 *
 * @section DESCRIPTION
 *
 * This class defines the public interface for a nonlinear iterative solver. 
 * Objects of the class cannot be created directly. To create a solver,
 * use the factory class NonlinearIterativeSolvers.
 */
template<typename Derived>
class NonlinearIterativeSolverBase {

  public: 


    /**
     * @brief  Reinitialize the new nonlinear solver
     *
     * @param  A Matrix representing the nonlinear system to solve
     */
    template <typename T>
    void reassignSystem(const T& A) {
      this->underlying().reassignSystem(A);
    }


    /**
     * @brief  Solve the nonlinear system
     *
     * @param  B is the RHS vector
     * @return Solution vector
     */
    template <typename T>
    auto solve(const T& b) {
      return this->underlying().solve(b);  
    }


    /**
     * @brief  Solve the linear system
     *
     * @param  B is the RHS vector
     * @param  X is the solution vector
     * @return void
     */
    template <typename T, typename U>
    void solve(const T& b, U& x) {
      this->underlying().solve(b, x);
    }


    int getLinearSolverMaxIterations() {
      return linearSolverMaxIters_;
    }


    void setLinearSolverMaxIterations(int maxIters) {
      linearSolverMaxIters_ = maxIters;
    }


    double getLinearSolverTolerance() {
      return linearSolverTolerance_;
    }


    void setLinearSolverTolerance(double tolerance) {
      linearSolverTolerance_ = tolerance;
    }


  protected:

    NonlinearIterativeSolverBase() : linearSolverMaxIters_(100), linearSolverTolerance_(1.0e-5) {}
    ~NonlinearIterativeSolverBase() = default;

    NonlinearIterativeSolverBase(const NonlinearIterativeSolverBase&) = delete;


  private:

    Derived& underlying() {
      return static_cast<Derived&>(*this);
    }
  

    Derived const& underlying() const {
      return static_cast<Derived const&>(*this);
    }


  private:

    int linearSolverMaxIters_;
    double linearSolverTolerance_;
      
};

} //end namespace solvers

#endif