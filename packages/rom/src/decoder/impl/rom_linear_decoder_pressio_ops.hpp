/*
//@HEADER
// ************************************************************************
//
// rom_linear_decoder_pressio_ops.hpp
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

#ifndef ROM_LINEAR_DECODER_PRESSIO_OPS_HPP_
#define ROM_LINEAR_DECODER_PRESSIO_OPS_HPP_

namespace pressio{ namespace rom{ namespace impl{

template <
  typename matrix_type,
  typename rom_state_type,
  typename fom_state_type
  >
struct LinearDecoderWithPressioOps
{
  using jacobian_type  = matrix_type;

private:
  using scalar_t	  = typename ::pressio::containers::details::traits<rom_state_type>::scalar_t;
  using jacobian_native_t = typename ::pressio::containers::details::traits<jacobian_type>::wrapped_t;
  using fom_native_t	  = typename ::pressio::containers::details::traits<fom_state_type>::wrapped_t;

  matrix_type phi_ = {};

public:
  LinearDecoderWithPressioOps() = delete;
  LinearDecoderWithPressioOps(const jacobian_type & matIn) : phi_(matIn){}
  LinearDecoderWithPressioOps(const jacobian_native_t & matIn) : phi_(matIn){}

  // applyMapping is templated because operand_t can be rom_state_type but
  // can also be an expression based on rom_state_type (e.g. for WLS)
  template <typename operand_t, typename fom_state_t = fom_state_type>
  void applyMapping(const operand_t & operand, fom_state_t & result) const
  {
    constexpr auto zero = ::pressio::utils::constants::zero<scalar_t>();
    constexpr auto one  = ::pressio::utils::constants::one<scalar_t>();
    ::pressio::ops::product(::pressio::nontranspose(), one, phi_, operand, zero, result);
  }

  const jacobian_type & getReferenceToJacobian() const{
    return phi_;
  }
};//end

}}}//end namespace pressio::rom::impl
#endif
