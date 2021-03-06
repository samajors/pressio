
#include "tpetra_only_fixtures.hpp"
#include <Kokkos_Core.hpp>

using uint_t = unsigned int;

template <typename T>
struct RedFunctor{
  T a_;
  double val_ = {};

  RedFunctor(T a, double val)
    : a_(a), val_(val){}

  KOKKOS_INLINE_FUNCTION
  void operator() (const int i, uint_t & sum) const {
    if ( std::abs( a_(i,0)-val_) < 1e-8 )
      sum++;
  }
};


TEST(tpetraVectors, loopedVectorManipulation){

  /* test following:
   * create a native tpetra vector
   * wrap it in containers::Vector
   * modify data on HOST
   * sync H -> D
   * lunch kokkos kernel to change data on D (multiply by 2 each element)
   * sync D -> H
   * check that returned vector matches what we did
   */

  // No need to init MPI because this test is run within tpetra scope guard

  using def_exe_space = Kokkos::DefaultExecutionSpace;

  // Print the name of Kokkos' default execution space.
  std::cout << "Kokkos default execution space is " <<
	    typeid (def_exe_space).name() << std::endl;

  // some types (use default, whatever that is)
  using tcomm		= Teuchos::Comm<int>;
  // using node_t		= typename Tpetra::Vector<>::node_type;
  using map_t		= Tpetra::Map<>;
  using nat_v_t		= Tpetra::Vector<>;

  using ST = typename nat_v_t::scalar_type;
  // using LO = typename nat_v_t::local_ordinal_type;
  // using GO = typename nat_v_t::global_ordinal_type;

  // create a teuchos comm
  Teuchos::RCP<const tcomm> comm;
  comm = Teuchos::rcp (new Teuchos::MpiComm<int>(MPI_COMM_WORLD));
  const auto numProc = comm->getSize();
  EXPECT_EQ(numProc, static_cast<uint_t>(4) );

  // create native tpetra vector of with N entries
  const uint_t N = 8;
  Teuchos::RCP<const map_t> contigMap;
  contigMap = Teuchos::rcp(new map_t(N, 0, comm));
  nat_v_t x(contigMap);

  // create wrapper vector
  using myvec_t = pressio::containers::Vector<nat_v_t>;
  // using myvec_dev_t = typename pressio::containers::details::traits<myvec_t>::device_t;
  // containers::Vector<> constr does a deep-copy (for now)
  myvec_t myx(x);  
  myx.data()->putScalar(pressio::utils::constants::zero<ST>());

  // modify the host (and then sync)
  myx.data()->modify<Kokkos::HostSpace>();
  auto vd = myx.data()->getLocalView<Kokkos::HostSpace>();

  // the # of entries I own
  auto myN = myx.extentLocal(0);

  for (auto i=0; i<myN; i++){
    vd(i,0) = 1.1;
  }

  // do first check, all entries should be 1.1
  using view_t = decltype(vd);
  using check_ftor_t = RedFunctor<view_t>;
  check_ftor_t FF1(vd, 1.1);
  uint_t sum = {0};
  Kokkos::parallel_reduce("checkData1", myN, FF1, sum);
  EXPECT_EQ( sum, static_cast<uint_t>(N/numProc) );
}
