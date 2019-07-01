
#ifdef HAVE_TRILINOS
#ifndef CONTAINERS_TPETRA_MULTI_VECTOR_DOT_TPETRA_MULTI_VECTOR_HPP_
#define CONTAINERS_TPETRA_MULTI_VECTOR_DOT_TPETRA_MULTI_VECTOR_HPP_

#include "../../containers_ops_meta.hpp"
#include "../../../multi_vector/containers_multi_vector_meta.hpp"

namespace rompp{ namespace containers{ namespace ops{

// Tpetra multivector dot tpetra multi vector

template <typename mvec_t,
  ::rompp::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value
    > * = nullptr
  >
void dot(const mvec_t & mvA, const mvec_t & mvB,
	 containers::Matrix<
	 Eigen::Matrix<typename containers::details::traits<mvec_t>::scalar_t,
	 Eigen::Dynamic, Eigen::Dynamic>
	 > & C)
{
  // how many vectors are in mvA and mvB
  auto numVecsA = mvA.globalNumVectors();
  auto numVecsB = mvB.globalNumVectors();
  assert( mvA.globalLength() == mvB.globalLength());
  assert(C.rows() == numVecsA);
  assert(C.cols() == numVecsB);
  // compute dot between every column of A with every col of B
  for (auto i=0; i<numVecsA; i++){
    // colI is a Teuchos::RCP<Vector<...>>
    auto colI = mvA.data()->getVector(i);
    for (auto j=0; j<numVecsB; j++){
      auto colJ = mvB.data()->getVector(j);
      C(i,j) = colI->dot(*colJ);
    }
  }
}


template <typename mvec_t,
  ::rompp::mpl::enable_if_t<
    containers::meta::is_multi_vector_wrapper_tpetra<mvec_t>::value
    > * = nullptr
  >
auto dot(const mvec_t & mvA, const mvec_t & mvB)
  -> containers::Matrix<
  Eigen::Matrix<typename containers::details::traits<mvec_t>::scalar_t,
  Eigen::Dynamic, Eigen::Dynamic>>{

  using sc_t = typename containers::details::traits<mvec_t>::scalar_t;
  using eig_mat = Eigen::Matrix< sc_t, Eigen::Dynamic, Eigen::Dynamic>;
  using res_t = containers::Matrix<eig_mat>;

  auto numVecsA = mvA.globalNumVectors();
  auto numVecsB = mvB.globalNumVectors();
  res_t C(numVecsA, numVecsB);
  dot(mvA, mvB, C);

  // // how many vectors are in mvA and mvB
  // auto numVecsA = mvA.globalNumVectors();
  // auto numVecsB = mvB.globalNumVectors();
  // assert( mvA.globalLength() == mvB.globalLength());
  // auto const & mvAdata = *mvA.data();
  // auto const & mvBdata = *mvB.data();

  // // result
  // res_t C(numVecsA, numVecsB);
  // // compute dot between every column of A with every col of B
  // for (auto i=0; i<numVecsA; i++){
  //   for (auto j=0; j<numVecsB; j++){
  //     mvAdata(i)->Dot( *(mvBdata(j)), &C(i,j) );
  //   }
  // }
  return C;
}
//--------------------------------------------------------


}}} // end namespace rompp::containers::ops
#endif
#endif