/*
//@HEADER
// ************************************************************************
//
// solvers_gauss_newton_normal_eq_conservative_impl.hpp
//                     		  Pressio
//                             Copyright 2019
//    National Technology & Engineering Solutions of Sandia, LLC (NTESS)
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

#ifndef SOLVERS_GAUSS_NEWTON_NORMAL_EQ_CONSERVATIVE_IMPL_HPP
#define SOLVERS_GAUSS_NEWTON_NORMAL_EQ_CONSERVATIVE_IMPL_HPP

#include "../helpers/solvers_converged_criterior_policy.hpp"
#include "../helpers/solvers_norm_dispatcher.hpp"
#include "../helpers/solvers_get_matrix_size_helper.hpp"

namespace pressio{ namespace solvers{ namespace iterative{ namespace impl{

template <
  typename system_t,
  typename iteration_t,
  typename scalar_t,
  typename lin_solver_t,
  typename line_search_t,
  typename converged_when_tag,
  typename cbar_t,
  typename mat_t,
  typename norm_dispatcher_t,
  ::pressio::mpl::enable_if_t<
    containers::meta::is_vector_wrapper_eigen<typename system_t::state_type>::value and
    containers::meta::is_vector_wrapper<typename system_t::residual_type>::value
    > * =nullptr
  >
void gauss_newtom_neq_conserv_solve(const system_t & sys,
				    typename system_t::state_type & y,
				    typename system_t::state_type & ytrial,
				    typename system_t::residual_type & resid,
				    typename system_t::jacobian_type & jacob,
				    iteration_t maxNonLIt,
				    scalar_t tolerance,
				    typename system_t::state_type & dy,
				    lin_solver_t & linSolver,
				    const cbar_t & cbarT,
				    mat_t & jTj,
				    mat_t & jTcbarT,
				    mat_t & cbarJ,
				    mat_t & zero,
				    typename system_t::residual_type & cbarTlambda,
				    typename system_t::state_type & jTr2,
				    typename system_t::state_type & cbarR,
				    mat_t & A,
				    typename system_t::state_type & b,
				    typename system_t::state_type & lambda,
				    typename system_t::state_type & y2,
				    std::string & convCondDescr,
				    const ::pressio::solvers::Norm & normType,
				    const norm_dispatcher_t & normDispatcher)
{

  // functor for checking convergence
  using is_conv_helper_t = IsConvergedHelper<converged_when_tag>;

  //-------------------------------------------------------

  // alpha for taking steps
  scalar_t alpha = static_cast<scalar_t>(1);
  // storing residual norm
  scalar_t normRes = {};
  scalar_t normRes0 = {};
  // storing projected residual norm
  scalar_t normJTRes = {};
  scalar_t normJTRes0 = {};

  convCondDescr = std::string(is_conv_helper_t::description_);

#ifdef PRESSIO_ENABLE_DEBUG_PRINT
  // get precision
  auto ss = std::cout.precision();
  // set to 14 for prints
  std::cout.precision(14);

  auto reset = utils::io::reset();
  auto fmt1 = utils::io::cyan() + utils::io::underline();
  ::pressio::utils::io::print_stdout(fmt1, "GN normal eqns conserv:",
				     "criterion:",
				     convCondDescr, reset, "\n");
#endif

  auto norm_dy = static_cast<scalar_t>(0);
  auto normLambda = static_cast<scalar_t>(0);
  auto normCbarR = static_cast<scalar_t>(0);

  constexpr auto zeroConst  = ::pressio::utils::constants::zero<scalar_t>();
  constexpr auto one = ::pressio::utils::constants::one<scalar_t>();

  typename system_t::state_type dy_y(y.extent(0));
  typename system_t::state_type dy_lambda(lambda.extent(0));

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
  auto timer = Teuchos::TimeMonitor::getStackedTimer();
  timer->start("Gauss Newton Conserv");
#endif

  iteration_t iStep = 0;
  while (++iStep <= maxNonLIt)
  {

#ifdef PRESSIO_ENABLE_DEBUG_PRINT
    ::pressio::utils::io::print_stdout("\n");
    auto fmt = utils::io::underline();
    ::pressio::utils::io::print_stdout(fmt, "step", iStep,
				    utils::io::reset(), "\n");
#endif

    // residual norm for current state
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("norm resid");
#endif
    normDispatcher.evaluate(resid, normRes, normType);
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("norm resid");
#endif

    // store initial residual norm
    if (iStep==1) normRes0 = normRes;

    // assemble LHS
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("lhs");
#endif

    // dot_self(jacob, jTj);
    ::pressio::ops::product(::pressio::transpose(), ::pressio::nontranspose(),
			    one, jacob, zeroConst, jTj);

    // ::pressio::ops::dot(jacob, cbarT, jTcbarT);
    ::pressio::ops::product(::pressio::transpose(), ::pressio::nontranspose(),
			    one, jacob, cbarT, zeroConst, jTcbarT);

    // ::pressio::ops::dot(cbarT, jacob, cbarJ);
    ::pressio::ops::product(::pressio::transpose(), ::pressio::nontranspose(),
			    one, cbarT, jacob, zeroConst, cbarJ);

    A.data()->block(0, 0, jTj.extent(0), jTj.extent(1)) = *jTj.data();
    A.data()->block(0, jTj.extent(1), jTcbarT.extent(0), jTcbarT.extent(1)) = *jTcbarT.data();
    A.data()->block(jTj.extent(0), 0, cbarJ.extent(0), cbarJ.extent(1)) = *cbarJ.data();
    A.data()->block(jTj.extent(0), jTj.extent(1), zero.extent(0), zero.extent(1)) = *zero.data();

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("lhs");
#endif

    // compute RHS
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("rhs");
#endif

    // ::pressio::ops::dot(cbarT, resid, cbarR);
    ::pressio::ops::product(::pressio::transpose(),
			    one, cbarT, resid, zeroConst, cbarR);

    normDispatcher.evaluate(cbarR, normCbarR, normType);

    // ::pressio::ops::product(cbarT, lambda, cbarTlambda);
    ::pressio::ops::product(::pressio::nontranspose(),
			    one, cbarT, lambda, zeroConst, cbarTlambda);

    resid.data()->update(1.0, *cbarTlambda.data(), 1.0);
    // ::pressio::ops::dot(jacob, resid, jTr2);
    ::pressio::ops::product(::pressio::transpose(),
			    one, jacob, resid, zeroConst, jTr2);

    constexpr auto negOne = ::pressio::utils::constants::negOne<scalar_t>();
    ::pressio::ops::scale(jTr2, negOne);
    ::pressio::ops::scale(cbarR, negOne);
    b.data()->block(0, 0, jTr2.extent(0), 1) = *jTr2.data();
    b.data()->block(jTr2.extent(0), 0, cbarR.extent(0), 1) = *cbarR.data();

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("rhs");
#endif

    // projected residual norm for current state
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("norm JTR");
#endif
    normDispatcher.evaluate(jTr2, normJTRes, normType);
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("norm JTR");
#endif

    // store initial residual norm
    if (iStep==1) normJTRes0 = normJTRes;

    // solve normal equations
#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->start("solve normeq");
#endif

    linSolver.solve(A, b, dy);

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
    timer->stop("solve normeq");
#endif

    *dy_y.data() = dy.data()->block(0, 0, y.extent(0), 1);
    *dy_lambda.data() = dy.data()->block(y.extent(0), 0, lambda.extent(0), 1);

    // norm of the correction
    normDispatcher.evaluate(dy_y, norm_dy, normType);
    normDispatcher.evaluate(dy_lambda, normLambda, normType);

#ifdef PRESSIO_ENABLE_DEBUG_PRINT
    ::pressio::utils::io::print_stdout(std::scientific,
				       "||R|| =", normRes,
				       "||R||(r) =", normRes/normRes0,
				       "||J^T R|| =", normJTRes,
				       "||J^T R||(r) =", normJTRes/normJTRes0,
				       "||cbar_R|| = ", normCbarR,
				       "||dy|| =", norm_dy,
				       "||dlambda|| =", normLambda,
				       utils::io::reset(), "\n");
#endif

    // exit with error if NaNs detected in solution update dy
    if (std::isnan(norm_dy))
    {
      throw std::runtime_error(
        "Nonlinear solver: Gauss Newton Conserv: NaNs detected in solution update dy");
    }
    //y2 = y2 + alpha * dy;
    ::pressio::ops::do_update(y2, one, dy, alpha);


    // solution update
    *y.data() = y2.data()->block(0, 0, y.extent(0), 1);
    *lambda.data() = y2.data()->block(y.extent(0), 0, lambda.extent(0), 1);

    // check convergence (whatever method user decided)
    const auto flag = is_conv_helper_t::evaluate(y, dy,
						 norm_dy, normRes, normRes0,
						 normJTRes, normJTRes0,
						 iStep, maxNonLIt, tolerance);
    if (flag) break;

    sys.residual(y, resid);
    sys.jacobian(y, jacob);

  }//loop

#if defined PRESSIO_ENABLE_DEBUG_PRINT
  std::cout.precision(ss);
  ::pressio::utils::io::print_stdout(std::fixed);
#endif

#ifdef PRESSIO_ENABLE_TEUCHOS_TIMERS
  timer->stop("Gauss Newton Conserv");
#endif

}

}}}} //end namespace pressio::solvers::iterative::impl
#endif
