/*
//@HEADER
// ************************************************************************
//
// ode_call_collector_dispatcher.hpp
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

#ifndef ODE_CALL_COLLECTOR_DISPATCHER_HPP_
#define ODE_CALL_COLLECTOR_DISPATCHER_HPP_

namespace pressio{ namespace ode{ namespace impl{

//this is within the impl namespace, so should not be used outside
template <
  typename collector_type, typename int_type, typename time_type, typename state_type,
  typename enable = void
  >
struct CallCollectorDispatch;


// this is within the impl namespace, so should not be used outside
// specialize for when collector accepts a native type
template <
  typename collector_type, typename int_type, typename time_type, typename state_type
  >
struct CallCollectorDispatch<
  collector_type, int_type, time_type, state_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_wrapper<state_type>::value and
    ::pressio::ode::meta::collector_accepts_native_container<
      collector_type, int_type, time_type, state_type
      >::value
    >
  >
{
  static void execute(collector_type	& collector,
		      const int_type	& step,
		      const time_type	& time,
		      const state_type	& yIn){

    collector(step, time, *yIn.data());
  }
};


//this is within the impl namespace, so should not be used outside
// specialize for when collector accepts a pressio container directly
template <
  typename collector_type, typename int_type, typename time_type, typename state_type
  >
struct CallCollectorDispatch<
  collector_type, int_type, time_type, state_type,
  ::pressio::mpl::enable_if_t<
    ::pressio::containers::meta::is_wrapper<state_type>::value and
    ::pressio::ode::meta::collector_accepts_pressio_container<
      collector_type, int_type, time_type, state_type
      >::value
    >
  >
{
  static void execute(collector_type	& collector,
		      const int_type	& step,
		      const time_type	& time,
		      const state_type	& yIn){

    collector(step, time, yIn);
  }
};

}}}//end namespace pressio::ode::impl
#endif
