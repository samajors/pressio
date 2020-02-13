/*
//@HEADER
// ************************************************************************
//
// rom_lspg_unsteady_type_generator_common_velocity_api.hpp
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

#ifndef ROM_LSPG_UNSTEADY_TYPE_GENERATOR_COMMON_VELOCITY_api_HPP_
#define ROM_LSPG_UNSTEADY_TYPE_GENERATOR_COMMON_VELOCITY_api_HPP_

#include "../impl_shared/rom_lspg_unsteady_aux_stepper_type_helper.hpp"
#include "../impl_shared/rom_lspg_unsteady_fom_states_storage_capacity_helper.hpp"

namespace pressio{ namespace rom{ namespace lspg{ namespace unsteady{ namespace impl{

template <
  bool isCpp,
  typename stepper_tag,
  typename fom_type,
  typename lspg_state_type,
  typename ...Args>
struct LSPGUnsteadyCommonTypesVelocityApi;


//-------------------------------------------------------
// partial specialize for when we are native C++
//-------------------------------------------------------
template <
  typename stepper_tag,
  typename fom_type,
  typename lspg_state_type,
  typename ...Args>
struct LSPGUnsteadyCommonTypesVelocityApi<
  true, stepper_tag, fom_type, lspg_state_type, Args ...
  >
{

  /* template arguments definitely needed
   * - valid decoder
   * - we possibly also have user-defined ops
   */
  // verify that args contains a valid decoder type
  using ic2 = ::pressio::mpl::variadic::find_if_unary_pred_t<
    ::pressio::rom::meta::is_legitimate_decoder_type, Args...>;
  using decoder_t = ::pressio::mpl::variadic::at_or_t<void, ic2::value, Args...>;
  static_assert(!std::is_void<decoder_t>::value and ic2::value < sizeof... (Args),
		"A valid decoder type must be passed to define a LSPG problem");
  using decoder_jac_t = typename decoder_t::jacobian_t;

  // these are native types of the full-order model (fom)
  using fom_t			= fom_type;
  using scalar_t		= typename fom_t::scalar_type;
  using fom_native_state_t	= typename fom_t::state_type;
  using fom_native_velocity_t	= typename fom_t::velocity_type;

  // fom wrapper types
  using fom_state_t		= ::pressio::containers::Vector<fom_native_state_t>;
  using fom_velocity_t		= ::pressio::containers::Vector<fom_native_velocity_t>;

  // rom state type (passed in)
  using lspg_state_t		= lspg_state_type;

  /* for LSPG, the lspg_residual_type = fom_velocity_type
   * and typically, the residual object has same size of velocity object
   * this is because the residual is computed where the velocity is computed.
   * This is important for sample mesh cases.
   * For instance imagine BDF1, where the residual is:
   *        R = -dt f() + y_n - y_n-1
   */
  using lspg_residual_t		= fom_velocity_t;

  /* lspg_matrix_t is type of J*decoder_jac_t (in the most basic case) where
   * * J is the jacobian of the fom rhs
   * * decoder_jac_t is the type of the decoder jacobian
   * In more complex cases, we might have (something)*J*decoder_jac_t,
   * where (something) is product of few matrices.
   * For now, set lspg_matrix_t to be of same type as decoder_jac_t
   * if phi is MV<>, then lspg_matrix_t = containers::MV<>
   * if phi is Matrix<>, then we have containers::Matrix<>
   * not bad assumption since all matrices are left-applied to decoder_jac_t
   */
  using lspg_matrix_t		= decoder_jac_t;

  // fom state reconstructor type
  using fom_state_reconstr_t	= FomStateReconstructor<scalar_t, fom_state_t, decoder_t>;

  // total num of fom states (i.e. stencil size plus the state at current step) needed
  static constexpr auto numStates = fomStatesStorageCapacityHelper<stepper_tag>::value;

  // type of class holding the fom states
  using fom_states_data = ::pressio::rom::FomStatesStaticContainer<fom_state_t, numStates, fom_state_reconstr_t>;

  // if we have a non-trivial user-defined ops, need to find from Args
  using ud_ops_t = void;

};


//-------------------------------------------------------
// partial specialize for pybind11
//-------------------------------------------------------
#ifdef PRESSIO_ENABLE_TPL_PYBIND11
template <
  typename stepper_tag,
  typename fom_type,
  typename lspg_state_type,
  typename decoder_type,
  typename ud_ops_type
  >
struct LSPGUnsteadyCommonTypesVelocityApi<
  false, stepper_tag, fom_type, lspg_state_type, decoder_type, ud_ops_type
  >
{

  // verify that we have a valid decoder type
  static_assert( ::pressio::rom::meta::is_legitimate_decoder_type<decoder_type>::value,
		"A valid decoder type must be passed to define a LSPG problem for Python bindings. \
The type detected does not meet the requirements.");
  using decoder_t		= decoder_type;
  using decoder_jac_t		= typename decoder_type::jacobian_t;

  // the fom_t is a pybind11::object since this comes from the python side
  using fom_t			= fom_type;
  using scalar_t		= typename decoder_type::scalar_t;

  // in this case there is no difference between types because
  // they all are pybind11::array_t so basically wrappers of numpy arrays
  // Since this is used to interface to python, EVERYTHING is done using numpy arrays
  using fom_native_state_t	= lspg_state_type;
  using fom_native_velocity_t	= lspg_state_type;

  using fom_state_t		= lspg_state_type;
  using fom_velocity_t		= lspg_state_type;

  // rom state type (passed in)
  using lspg_state_t		= lspg_state_type;

  /* for LSPG, the lspg_residual_type = fom_velocity_type
   * and typically, the residual object has same size of velocity object
   * this is because the residual is computed where the velocity is computed.
   * This is important for sample mesh cases.
   * For instance imagine BDF1, where the residual is:
   *        R = -dt f() + y_n - y_n-1
   */
  using lspg_residual_t		= fom_velocity_t;

  /* lspg_matrix_t is type of J*decoder_jac_t (in the most basic case) where
   * * J is the jacobian of the fom rhs
   * * decoder_jac_t is the type of the decoder jacobian
   * In more complex cases, we might have (something)*J*decoder_jac_t,
   * where (something) is product of few matrices.
   * For now, set lspg_matrix_t to be of same type as decoder_jac_t
   * if phi is MV<>, then lspg_matrix_t = containers::MV<>
   * if phi is Matrix<>, then we have containers::Matrix<>
   * not bad assumption since all matrices are left-applied to decoder_jac_t
   */
  using lspg_matrix_t		= decoder_jac_t;

  // fom state reconstructor type
  using fom_state_reconstr_t	= FomStateReconstructor<scalar_t, fom_state_t, decoder_t>;

  // total num of fom states (i.e. stencil size plus the state at current step) needed
  static constexpr auto numStates = fomStatesStorageCapacityHelper<stepper_tag>::value;

  // class type holding the fom states
  using fom_states_data = ::pressio::rom::FomStatesStaticContainer<fom_state_t, numStates, fom_state_reconstr_t>;

  // ud_ops_t (if this is non-void, it should be a pybind11::object since this is passed by user from python side)
  using ud_ops_t = ud_ops_type;
  static_assert( std::is_void<ud_ops_t>::value or std::is_same<ud_ops_t, pybind11::object>::value,
		 "The type passes for the user defined ops for LSPG bindings \
to python is not valid. It must be a pybind11::object.");

};
#endif


}}}}}//end  namespace pressio::rom::lspg::unstedy::impl
#endif
