/*
//@HEADER
// ************************************************************************
//
// solvers_nonlinear_base.hpp
//                     		      Pressio 
// Copyright 2019 National Technology & Engineering Solutions of Sandia,LLC 
//							      (NTESS)
//
// Under the terms of Contract DE-NA0003525 with NTESS, the 
// U.S. Government retains certain rights in this software.
//
// Pressio is licensed under BSD-3-Clause terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions 
// are met:
//
// 1. Redistributions of source code must retain the above copyright 
// notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright 
// notice, this list of conditions and the following disclaimer in the 
// documentation and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its 
// contributors may be used to endorse or promote products derived 
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Francesco Rizzi (fnrizzi@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef SOLVERS_EXPERIMENTAL_NONLINEAR_BASE_HPP
#define SOLVERS_EXPERIMENTAL_NONLINEAR_BASE_HPP

#include <iostream>
#include <memory>
#include <type_traits>

#include "../solvers_ConfigDefs.hpp"
#include "solvers_system_traits.hpp"
#include "solvers_norms_fwd.hpp"
#include "solvers_linear_traits.hpp"
#include "solvers_meta_static_checks.hpp"


namespace pressio{
namespace solvers{


struct NonlinearSolvers; // Fwd declaration

/**
 * @brief Base class for nonlinear solver implemented through CRTP
 *
 * @section DESCRIPTION
 *
 * This class defines the public interface for a nonlinear solver.
 * Objects of the class cannot be created directly. To create a solver,
 * use the factory class NonLinearSolvers.
 */
template <typename Derived>
class NonLinearSolverBase {

  public:

    /**
     * Raise an assertion as the non linear system supplied as input is invalid
     *
     * @param  system non linear system to be solved
     * @param  x0 initial solution guess
     */
    template <
      typename PrecT,
      typename NormT,
      typename SystemT,
      typename VectorT,
      typename std::enable_if<
        !(
          details::system_traits<SystemT>::is_system &&
          solvers::meta::are_vector_matrix_compatible<
            VectorT,
            typename details::system_traits<SystemT>::matrix_type
          >::value
        ),
        int
      >::type* = nullptr
    >
    void solve(const SystemT& sys, const VectorT& x0) {
      std::cerr << "Error: either the nonlinear system or \
the solution hint is invalid." << std::endl;
      assert(0);
    }
    //--------------------------------------------------------------


    /**
     * Solve the non linear system.
     *
     * @param system non linear system to be solved.
     * @param x0 initial solution guess.
     * @return solution vector.
     */
    template <
      typename PrecT,
      typename NormT,
      typename SystemT,
      typename VectorT,
      typename std::enable_if<
        details::system_traits<SystemT>::is_system &&
        solvers::meta::are_vector_matrix_compatible<
          VectorT,
          typename details::system_traits<SystemT>::matrix_type
        >::value,
        int
      >::type* = nullptr
    >
    VectorT solve(const SystemT& sys, const VectorT& x0) {
      return this->underlying().template solve_<PrecT, NormT>(sys, x0);
    }
    //--------------------------------------------------------------


    /**
     * @brief Solve the non linear system.
     *
     * @param system is the non linear system to be solved.
     * @param x0 is the solution hint.
     * @return solution vector.
     *
     * @section DESCRIPTION
     *
     * This version of solve takes a reduced set of meta-parameters
     * and forward the arguments tto the full solve method.
     */
    template <
      typename SystemT,
      typename VectorT
    >
    VectorT solve(const SystemT& sys, const VectorT& x0) {
      return this->template solve<linear::DefaultPreconditioner, 
                                   L2Norm, SystemT, VectorT>(sys, x0);
    }
    //--------------------------------------------------------------


    /**
     * Get the maximum number of iterations of the nonlinear solver.
     */
    containers::default_types::uint getMaxNonLinearIterations() {
      return maxNonLinearIterations_;
    }
    //--------------------------------------------------------------


    /**
     * Get the tolerance of the nonlinear solver.
     */
    double getNonLinearTolerance() {
      return nonLinearTolerance_;
    }
    //--------------------------------------------------------------


    /**
     * Set the maximum number of iterations of the nonlinear solver.
     *
     * @param maxNonLinearIterations maximum number of iterations of the nonlinear solver.
     */
    void setMaxNonLinearIterations(containers::default_types::uint
				   maxNonLinearIterations) {
      maxNonLinearIterations_ = maxNonLinearIterations;
    }
    //--------------------------------------------------------------


    /**
     * Set the tolerance of the nonlinear solver.
     *
     * @param nonLinearTolerance tolerance of the nonlinear solver.
     */
    void setNonLinearTolerance(double nonLinearTolerance) {
      nonLinearTolerance_ = std::abs(nonLinearTolerance);
    }
    //--------------------------------------------------------------

  protected:

    NonLinearSolverBase()
      : maxNonLinearIterations_(100), nonLinearTolerance_(1.0e-5) {}

  private:

    Derived& underlying() {
      return static_cast<Derived&>(*this);
    }

    Derived const& underlying() const {
      return static_cast<Derived const&>(*this);
    }

  private:

    containers::default_types::uint maxNonLinearIterations_;
    double nonLinearTolerance_;

};

} //end namespace solvers
}//end namespace pressio
#endif
