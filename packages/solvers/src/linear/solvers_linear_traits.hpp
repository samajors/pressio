/*
//@HEADER
// ************************************************************************
//
// solvers_linear_traits.hpp
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

#ifndef SOLVERS_EXPERIMENTAL_LINEAR_TRAITS_HPP
#define SOLVERS_EXPERIMENTAL_LINEAR_TRAITS_HPP

#include <Eigen/Core>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Householder>
#include <Eigen/QR>
#include <Eigen/Sparse>
#include <Eigen/SparseQR>
#include <Eigen/OrderingMethods>

namespace pressio{ namespace solvers{ namespace linear { namespace details {

// Solvers traits
template <typename T>
struct traits {
  static constexpr bool direct        = false;
  static constexpr bool iterative     = false;
  static constexpr bool eigen_enabled = false;
  static constexpr bool kokkos_enabled = false;
};

template <>
struct traits<::pressio::solvers::linear::iterative::CG> {

  template <
    typename MatrixT,
    typename PrecT = Eigen::DiagonalPreconditioner<typename MatrixT::Scalar>
  >
  using eigen_solver_type = Eigen::ConjugateGradient<MatrixT, Eigen::Lower, PrecT>;

  static constexpr bool direct        = false;
  static constexpr bool iterative     = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::iterative::Bicgstab> {

  template <
    typename MatrixT,
    typename PrecT = Eigen::DiagonalPreconditioner<typename MatrixT::Scalar>
  >
  using eigen_solver_type = Eigen::BiCGSTAB<MatrixT, PrecT>;

  static constexpr bool direct        = false;
  static constexpr bool iterative     = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::iterative::LSCG> {

  template <
    typename MatrixT,
    typename PrecT = Eigen::DiagonalPreconditioner<typename MatrixT::Scalar>
  >
  using eigen_solver_type = Eigen::LeastSquaresConjugateGradient<MatrixT, PrecT>;

  static constexpr bool direct        = false;
  static constexpr bool iterative     = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::direct::ColPivHouseholderQR> {

  /* if matrix is dense, use Eigen::ColPivHouseholderQR.
   * if the native matrix is sparse, then use Eigen::SparseQR.
   * to use SparseQR, the matrix has to be:
   * (a) sparse
   * (b) column-major
   * (c) compressed mode to use COLAMDOrdering
	Note that by default, pressio::container::Matrix wrapper of Eigen::SparseMatrix
	always compresses the matrix.
  */
  template <typename MatrixT>
  using eigen_solver_type =
    typename std::conditional<
      pressio::containers::meta::is_sparse_matrix_eigen<MatrixT>::value &&
      MatrixT::IsRowMajor==0,
      Eigen::SparseQR<MatrixT, Eigen::COLAMDOrdering<typename MatrixT::StorageIndex>>,
      Eigen::ColPivHouseholderQR<MatrixT>
      >::type;

  static constexpr bool iterative = false;
  static constexpr bool direct = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::direct::HouseholderQR> {

  template <typename MatrixT>
  using eigen_solver_type = Eigen::HouseholderQR<MatrixT>;

  static constexpr bool iterative = false;
  static constexpr bool direct = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::direct::PartialPivLU> {

  template <typename MatrixT>
  using eigen_solver_type = Eigen::PartialPivLU<MatrixT>;

  static constexpr bool iterative = false;
  static constexpr bool direct = true;
  static constexpr bool eigen_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::direct::potrsL> {

  template <typename MatrixT>
  using eigen_solver_type = Eigen::LLT<MatrixT, Eigen::Lower>;

  static constexpr bool iterative = false;
  static constexpr bool direct = true;
  static constexpr bool eigen_enabled = true;
#if defined PRESSIO_ENABLE_TPL_TRILINOS or defined PRESSIO_ENABLE_TPL_KOKKOS
  static constexpr bool kokkos_enabled = true;
#endif
};


template <>
struct traits<::pressio::solvers::linear::direct::potrsU> {

  template <typename MatrixT>
  using eigen_solver_type = Eigen::LLT<MatrixT, Eigen::Upper>;

  static constexpr bool direct = true;
  static constexpr bool eigen_enabled = true;
#if defined PRESSIO_ENABLE_TPL_TRILINOS or defined PRESSIO_ENABLE_TPL_KOKKOS
  static constexpr bool kokkos_enabled = true;
#endif
};


#if defined PRESSIO_ENABLE_TPL_TRILINOS or defined PRESSIO_ENABLE_TPL_KOKKOS
template <>
struct traits<::pressio::solvers::linear::direct::getrs> {

  static constexpr bool direct = true;
  // for now, disable eigen, enable it later
  static constexpr bool eigen_enabled = false;
  static constexpr bool kokkos_enabled = true;
};

template <>
struct traits<::pressio::solvers::linear::direct::geqrf> {

  static constexpr bool direct = true;
  // for now, disable eigen, enable it later
  static constexpr bool eigen_enabled = false;
  static constexpr bool kokkos_enabled = true;
};
#endif


// template <>
// struct traits<CompleteOrthogonalDecomposition> {

//   template <typename MatrixT>
//   using eigen_solver_type = Eigen::CompleteOrthogonalDecomposition<MatrixT>;

//   static constexpr bool direct = true;
//   static constexpr bool eigen_enabled = true;
// };


}}}}//end namespace pressio::solvers::linear::details
#endif
