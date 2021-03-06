/*
//@HEADER
// ************************************************************************
//
// containers_vector_sharedmem_teuchos_serial_dense.hpp
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

#ifdef PRESSIO_ENABLE_TPL_TRILINOS
#ifndef CONTAINERS_VECTOR_CONCRETE_VECTOR_SHAREDMEM_TEUCHOS_SERIAL_DENSE_HPP_
#define CONTAINERS_VECTOR_CONCRETE_VECTOR_SHAREDMEM_TEUCHOS_SERIAL_DENSE_HPP_

namespace pressio{ namespace containers{

template <typename wrapped_type>
class Vector<wrapped_type,
	     ::pressio::mpl::enable_if_t<
	       containers::meta::is_dense_vector_teuchos<wrapped_type>::value
	       >
	     >
  : public VectorSharedMemBase< Vector<wrapped_type> >
{

  using this_t = Vector<wrapped_type>;
  using mytraits = typename details::traits<this_t>;
  using sc_t = typename mytraits::scalar_t;
  using ord_t = typename  mytraits::ordinal_t;
  using wrap_t = typename mytraits::wrapped_t;
  using ref_t = typename mytraits::reference_t;
  using const_ref_t = typename mytraits::const_reference_t;

public:
  Vector() = default;
  explicit Vector(ord_t insize) : data_(insize){}
  explicit Vector(const wrap_t & src) : data_(src){}

  // copy constructor implements copy semantics
  Vector(const Vector & other) = default;
  // copy assign implments copy semantics
  Vector & operator=(const Vector & other) = default;

  // move cnstr and assign
  Vector(Vector && other) = default;
  Vector & operator=(Vector && other) = default;

  ~Vector() = default;

public:
  // assignment with value
  this_t & operator=(const sc_t value){
    for (ord_t i = 0; i != this->extent(0); ++i)
      data_[i] = value;
    return *this;
  }

  // compound assignment when type(b) = type(this)
  // this += b
  this_t & operator+=(const this_t & other) {
    assert( other.extent(0) == this->extent(0) );
    this->data_ += *other.data();
    return *this;
  }

  // compound assignment when type(b) = type(this)
  // this -= b
  this_t & operator-=(const this_t & other) {
    assert( other.extent(0) == this->extent(0) );
    this->data_ -= *other.data();
    return *this;
  }

public:
  ref_t operator [] (ord_t i){
    return data_(i);
  };
  const_ref_t operator [] (ord_t i) const{
    return data_(i);
  };

  ref_t operator()(ord_t i){
    return data_[i];
  };
  const_ref_t operator()(ord_t i) const{
    return data_[i];
  };

  wrap_t const * data() const{
    return &data_;
  }

  wrap_t * data(){
    return &data_;
  }

  bool empty() const{
    return this->extent(0)==0 ? true : false;
  }

  ord_t extent(ord_t i) const {
    assert(i==0);
    return data_.length();
  }

private:
  friend VectorSharedMemBase< this_t >;
  wrap_t data_ = {};

};//end class

}}//end namespace pressio::containers
#endif
#endif
