/*
//@HEADER
// ************************************************************************
//
//                      extract_template.hpp
//                         TINYMPL
//              Copyright (C) 2017 NTESS, LLC
//
// Under the terms of Contract DE-NA-0003525 with NTESS, LLC,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact somebody@sandia.gov
//
// ************************************************************************
//@HEADER
*/

#ifndef TINYMPL_EXTRACT_TEMPLATE_HPP
#define TINYMPL_EXTRACT_TEMPLATE_HPP

#include <tinympl/variadic/at.hpp>

namespace tinympl {

template <typename T>
struct extract_template;

template <
  template <class...> class templ,
  class... Args
>
struct extract_template<templ<Args...>> {
  template <typename... Ts>
  using rebind = templ<Ts...>;
};

template <typename T>
struct extract_args;

template <
  template <class...> class templ,
  class... Args
>
struct extract_args<templ<Args...>> {
  template <template <typename...> class new_templ>
  using rebind = new_templ<Args...>;
};

template <size_t N, typename T>
struct extract_arg_n;

template <size_t N, template <class...> class templ, typename... Args>
struct extract_arg_n<N, templ<Args...>> {
  using type = tinympl::variadic::at_t<N, Args...>;
};

} // end namespace tinympl

#endif //TINYMPL_EXTRACT_TEMPLATE_HPP