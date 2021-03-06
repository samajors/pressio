/*
//@HEADER
// ************************************************************************
//
// pressio_containers.hpp
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

#ifndef PRESSIO_CONTAINERS_HPP_
#define PRESSIO_CONTAINERS_HPP_

#include "pressio_mpl.hpp"
#include "pressio_utils.hpp"

#include "containers/src/containers_ConfigDefs.hpp"
#include "containers/src/containers_fwd.hpp"
#include "containers/src/containers_wrapped_types_enum.hpp"
#include "containers/src/containers_shared_traits.hpp"

#include "containers/src/meta/containers_meta_type_aliases_detection.hpp"
#include "containers/src/meta/generic/containers_is_teuchos_rcp.hpp"
#include "containers/src/meta/generic/containers_native_pybind_array_meta.hpp"

//------------------
// BASE
//------------------
#include "containers/src/base/containers_vector_sharedmem_base.hpp"
#include "containers/src/base/containers_vector_distributed_base.hpp"
#include "containers/src/base/containers_multi_vector_sharedmem_base.hpp"
#include "containers/src/base/containers_multi_vector_distributed_base.hpp"
#include "containers/src/base/containers_matrix_sharedmem_base.hpp"
#include "containers/src/base/containers_matrix_distributed_base.hpp"
#include "containers/src/base/containers_container_sharedmem_base.hpp"
#include "containers/src/base/containers_container_distributed_base.hpp"
#include "containers/src/base/containers_container_base.hpp"

//------------------
// VECTOR
//------------------
#include "containers/src/vector/meta/containers_native_eigen_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_epetra_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_kokkos_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_teuchos_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_tpetra_block_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_tpetra_vector_meta.hpp"
#include "containers/src/vector/meta/containers_native_arbitrary_vector_meta.hpp"

#include "containers/src/vector/meta/containers_is_dense_vector_wrapper_teuchos.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_arbitrary.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_pybind.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_eigen.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_epetra.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_kokkos.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_tpetra_block.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper_tpetra.hpp"
#include "containers/src/vector/meta/containers_is_vector_wrapper.hpp"

#include "containers/src/vector/containers_vector_traits.hpp"

#include "containers/src/vector/concrete/containers_vector_arbitrary.hpp"
#include "containers/src/vector/concrete/containers_vector_sharedmem_pybind11.hpp"
#include "containers/src/vector/concrete/containers_vector_distributed_epetra.hpp"
#include "containers/src/vector/concrete/containers_vector_distributed_tpetra_block.hpp"
#include "containers/src/vector/concrete/containers_vector_distributed_tpetra.hpp"
#include "containers/src/vector/concrete/containers_vector_sharedmem_eigen_dynamic.hpp"
#include "containers/src/vector/concrete/containers_vector_sharedmem_eigen_static.hpp"
#include "containers/src/vector/concrete/containers_vector_sharedmem_kokkos.hpp"
#include "containers/src/vector/concrete/containers_vector_sharedmem_teuchos_serial_dense.hpp"

//------------------
// MATRIX
//------------------
#include "containers/src/matrix/meta/containers_native_eigen_matrix_meta.hpp"
#include "containers/src/matrix/meta/containers_native_kokkos_matrix_meta.hpp"
#include "containers/src/matrix/meta/containers_native_trilinos_matrix_meta.hpp"
#include "containers/src/matrix/meta/containers_native_arbitrary_matrix_meta.hpp"

#include "containers/src/matrix/meta/containers_is_dense_matrix_wrapper_eigen.hpp"
#include "containers/src/matrix/meta/containers_is_dense_matrix_wrapper_epetra.hpp"
#include "containers/src/matrix/meta/containers_is_dense_matrix_wrapper_kokkos.hpp"
#include "containers/src/matrix/meta/containers_is_dense_matrix_wrapper_teuchos.hpp"
#include "containers/src/matrix/meta/containers_is_sparse_matrix_wrapper_eigen.hpp"
#include "containers/src/matrix/meta/containers_is_sparse_matrix_wrapper_epetra.hpp"
#include "containers/src/matrix/meta/containers_is_sparse_matrix_wrapper_kokkos.hpp"
#include "containers/src/matrix/meta/containers_is_sparse_matrix_wrapper_tpetra.hpp"
#include "containers/src/matrix/meta/containers_is_matrix_wrapper_arbitrary.hpp"
#include "containers/src/matrix/meta/containers_is_matrix_wrapper_eigen.hpp"
#include "containers/src/matrix/meta/containers_is_matrix_wrapper_kokkos.hpp"
#include "containers/src/matrix/meta/containers_is_matrix_wrapper_pybind.hpp"
#include "containers/src/matrix/meta/containers_is_matrix_wrapper.hpp"

#include "containers/src/matrix/containers_matrix_traits.hpp"

#include "containers/src/matrix/concrete/containers_matrix_arbitrary.hpp"
#include "containers/src/matrix/concrete/containers_matrix_sharedmem_pybind11.hpp"
#include "containers/src/matrix/concrete/containers_matrix_dense_distributed_epetra.hpp"
#include "containers/src/matrix/concrete/containers_matrix_dense_sharedmem_eigen_dynamic.hpp"
#include "containers/src/matrix/concrete/containers_matrix_dense_sharedmem_eigen_static.hpp"
#include "containers/src/matrix/concrete/containers_matrix_dense_sharedmem_kokkos.hpp"
#include "containers/src/matrix/concrete/containers_matrix_dense_sharedmem_teuchos_serial.hpp"
#include "containers/src/matrix/concrete/containers_matrix_sparse_sharedmem_eigen.hpp"


//------------------
// MULTI VECTOR
//------------------
#include "containers/src/multi_vector/meta/containers_native_eigen_multi_vector_meta.hpp"
#include "containers/src/multi_vector/meta/containers_native_epetra_multi_vector_meta.hpp"
#include "containers/src/multi_vector/meta/containers_native_kokkos_multi_vector_meta.hpp"
#include "containers/src/multi_vector/meta/containers_native_tpetra_block_multi_vector_meta.hpp"
#include "containers/src/multi_vector/meta/containers_native_tpetra_multi_vector_meta.hpp"
#include "containers/src/multi_vector/meta/containers_native_arbitrary_multi_vector_meta.hpp"

#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_tpetra.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_tpetra_block.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_kokkos.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_epetra.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_eigen.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper_arbitrary.hpp"
#include "containers/src/multi_vector/meta/containers_is_multi_vector_wrapper.hpp"

#include "containers/src/multi_vector/containers_multi_vector_traits.hpp"

#include "containers/src/multi_vector/concrete/containers_multi_vector_arbitrary.hpp"
#include "containers/src/multi_vector/concrete/containers_multi_vector_distributed_epetra.hpp"
#include "containers/src/multi_vector/concrete/containers_multi_vector_distributed_tpetra_block.hpp"
#include "containers/src/multi_vector/concrete/containers_multi_vector_distributed_tpetra.hpp"
#include "containers/src/multi_vector/concrete/containers_multi_vector_sharedmem_eigen_dynamic.hpp"
#include "containers/src/multi_vector/concrete/containers_multi_vector_sharedmem_kokkos.hpp"


//------------------
// expressions
//------------------
#include "containers/src/expressions/span/containers_vector_span_expression.hpp"
#include "containers/src/expressions/span/containers_expressions_traits.hpp"
#include "containers/src/expressions/span/containers_span.hpp"

#include "containers/src/expressions/subspan/containers_matrix_subspan_expression.hpp"
#include "containers/src/expressions/subspan/containers_expressions_traits.hpp"
#include "containers/src/expressions/subspan/containers_subspan.hpp"

#include "containers/src/meta/generic/containers_is_expression.hpp"

//------------------
// others
//------------------
#include "containers/src/meta/generic/containers_is_wrapper.hpp"
#include "containers/src/meta/containers_meta_compatibility.hpp"
#include "containers/src/collection/containers_static_collection.hpp"

#endif
