/*
//@HEADER
// ************************************************************************
//
// containers_container_distributed_mpi_base.hpp
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

#ifdef HAVE_MPI
#ifndef CONTAINERS_SHARED_BASE_CONTAINER_DISTRIBUTED_MPI_BASE_HPP_
#define CONTAINERS_SHARED_BASE_CONTAINER_DISTRIBUTED_MPI_BASE_HPP_

#include "../containers_ConfigDefs.hpp"
#include "../meta/containers_meta_basic.hpp"

namespace pressio{ namespace containers{

template<typename derived_type, typename comm_t>
class ContainerDistributedMpiBase
  : private utils::details::CrtpBase<
  ContainerDistributedMpiBase<derived_type,comm_t> >{

  using this_t = ContainerDistributedMpiBase<derived_type,comm_t>;

public:

  template <typename T = comm_t,
	    typename std::enable_if<
      #ifdef HAVE_TRILINOS
	      !meta::is_teuchos_rcp<T>::value and 
      #endif
        !::pressio::mpl::is_std_shared_ptr<T>::value 
	      >::type * = nullptr
	    >
  T const & commCRef() const{
    return this->underlying().commCRefImpl();
  }

  template <typename T= comm_t,
  	    typename std::enable_if<
      #ifdef HAVE_TRILINOS
        meta::is_teuchos_rcp<T>::value or 
      #endif
        ::pressio::mpl::is_std_shared_ptr<T>::value 
  	      >::type * = nullptr>
  T comm() const{
    return this->underlying().commImpl();
  }

private:
  /* workaround for nvcc issue with templates, see https://devtalk.nvidia.com/default/topic/1037721/nvcc-compilation-error-with-template-parameter-as-a-friend-within-a-namespace/ */
  template<typename DummyType> struct dummy{using type = DummyType;};
  friend typename dummy<derived_type>::type;

  friend utils::details::CrtpBase<this_t>;

  ContainerDistributedMpiBase() = default;
  ~ContainerDistributedMpiBase() = default;

};//end class

}}//end namespace pressio::containers
#endif
#endif
