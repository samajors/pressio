/*
//@HEADER
// ************************************************************************
//
// rom_lspg_unsteady_fom_states_storage_capacity_helper.hpp
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

#ifndef ROM_LSPG_UNSTEADY_FOM_STATES_STORAGE_CAPACITY_HELPER_HPP_
#define ROM_LSPG_UNSTEADY_FOM_STATES_STORAGE_CAPACITY_HELPER_HPP_

namespace pressio{ namespace rom{ namespace lspg{ namespace unsteady{ namespace impl{

// base case
template <typename stepper_tag>
struct fomStatesStorageCapacityHelper{
  static constexpr std::size_t value = 1;
};

template <>
struct fomStatesStorageCapacityHelper<::pressio::ode::implicitmethods::Euler>{
  // for euler, I need to store the state at the current step
  // plus the state at the previous step, so 2 total
  static constexpr std::size_t value = 2;
};

template <>
struct fomStatesStorageCapacityHelper<::pressio::ode::implicitmethods::BDF2>{
  // for BDF2, I need to store the state at the current step
  // plus the state at the previous two steps, so 3 total
  static constexpr std::size_t value = 3;
};

}}}}}//end  namespace pressio::rom::lspg::unsteady::impl
#endif
