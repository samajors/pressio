
#ifndef ROM_LINEAR_DECODER_HPP_
#define ROM_LINEAR_DECODER_HPP_

#include "rom_decoder_base.hpp"
#include "../rom_forward_declarations.hpp"

namespace rompp{ namespace rom{

template <
  typename matrix_type,
  template <typename...> class
  wrapper_operator_t = MultiVectorOperator
  >
struct LinearDecoder
  : public DecoderBase<
  LinearDecoder<matrix_type, wrapper_operator_t>,
  matrix_type>{

  using this_t	    = LinearDecoder<matrix_type, wrapper_operator_t>;
  using base_t	    = DecoderBase<this_t, matrix_type>;
  using matrix_op_t = wrapper_operator_t<matrix_type>;
  using jacobian_t  = matrix_type;

public:
  LinearDecoder() = delete;
  LinearDecoder(jacobian_t & matIn) : phi_(matIn){}
  ~LinearDecoder() = default;

protected:
  template <typename operand_t, typename result_t>
  void applyMappingImpl(const operand_t & operandObj,
			result_t & resultObj) const{
    phi_.apply(operandObj, resultObj);
  }

  const jacobian_t & getJacobianRefImpl() const{
    return phi_.getRefToOperator();
  }

private:
  friend base_t;
  matrix_op_t phi_ = {};

};//end class

}}//end namespace rompp::rom
#endif