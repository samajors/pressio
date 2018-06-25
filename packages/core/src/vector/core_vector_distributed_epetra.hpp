
#ifndef CORE_VECTOR_EPETRA_HPP_
#define CORE_VECTOR_EPETRA_HPP_

#include "./base/core_vector_generic_base.hpp"
#include "./base/core_vector_distributed_base.hpp"
#include "./base/core_vector_math_base.hpp"
#include "Epetra_Vector.h"

namespace core{
  
template <typename wrapped_type>
class vector<wrapped_type,
	     typename
	     std::enable_if<
	       std::is_same<wrapped_type,
			    Epetra_Vector
			    >::value
	       >::type
	     >
  : public vectorGenericBase< vector<wrapped_type> >,
    public vectorDistributedBase< vector<wrapped_type> >,
    public vectorMathBase< vector<wrapped_type> >
{
private:
  using derived_t = vector<wrapped_type>;
  using sc_t = typename details::traits<derived_t>::scalar_t;
  using LO_t = typename details::traits<derived_t>::local_ordinal_t;
  using GO_t = typename details::traits<derived_t>::global_ordinal_t;
  using der_t = typename details::traits<derived_t>::derived_t;
  using wrap_t = typename details::traits<derived_t>::wrapped_t;
  using map_t = typename details::traits<derived_t>::data_map_t;
  using mpicomm_t = typename details::traits<derived_t>::communicator_t;

public:
  vector() = delete;
  vector(const map_t & mapobj) : data_(mapobj){};
  vector(const wrap_t & vecobj) : data_(vecobj){};
  ~vector() = default;

public:
  sc_t & operator [] (LO_t i){
    assert(i < this->localSize());
    return data_[i];
  };
  sc_t const & operator [] (LO_t i) const{
    assert(i < this->localSize());
    return data_[i];
  };  

  der_t operator+(const der_t & other) {
    der_t res( other.getDataMap() );
    for (LO_t i=0; i<this->localSize(); i++)
      res[i] = (*this)[i] + other[i];
    return res;
  }

  der_t operator-(const der_t & other) {
    der_t res( other.getDataMap() );
    for (LO_t i=0; i<this->localSize(); i++)
      res[i] = (*this)[i] - other[i];
    return res;
  }

  der_t operator*(const der_t & other) {
    der_t res( other.getDataMap() );
    for (LO_t i=0; i<this->localSize(); i++)
      res[i] = (*this)[i] * other[i];
    return res;
  }

  der_t & operator+=(const der_t & other) {
    this->data_.Update(1.0, *other.data(), 1.0 );
    return *this;
  }

  der_t & operator-=(const der_t & other) {
    this->data_.Update(-1.0, *other.data(), 1.0 );
    return *this;
  }
    
private:  
  wrap_t const * dataImpl() const{
    return &data_;
  };
  wrap_t * dataImpl(){
    return data_;
  };

  size_t localSizeImpl() const {
    return data_.MyLength();
  };

  size_t globalSizeImpl() const {
    return data_.GlobalLength();
  };
  
  map_t const & getDataMapImpl() const{
    return data_.Map();
  }

  void putScalarImpl(sc_t value) {
    this->data_.PutScalar(value);
  }    
  
private:
  friend vectorGenericBase< derived_t >;
  friend vectorDistributedBase< derived_t >;
  friend vectorMathBase< derived_t >;
  
private:
  wrap_t data_;

};//end class    

}//end namespace core
#endif