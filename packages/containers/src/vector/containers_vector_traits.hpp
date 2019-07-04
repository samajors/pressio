
#ifndef CONTAINERS_VECTOR_VECTOR_TRAITS_HPP_
#define CONTAINERS_VECTOR_VECTOR_TRAITS_HPP_

#include "../containers_fwd.hpp"
#include "../containers_shared_traits.hpp"
#include "./meta/containers_native_armadillo_vector_meta.hpp"
#include "./meta/containers_native_blaze_vector_meta.hpp"
#include "./meta/containers_native_eigen_vector_meta.hpp"
#ifdef HAVE_TRILINOS
#include "./meta/containers_native_epetra_vector_meta.hpp"
#include "./meta/containers_native_tpetra_vector_meta.hpp"
#include "./meta/containers_native_teuchos_vector_meta.hpp"
#include "./meta/containers_native_kokkos_vector_meta.hpp"
#include "./meta/containers_native_tpetra_block_vector_meta.hpp"
#endif

namespace pressio{ namespace containers{ namespace details{

/********************************
an arbitrary vector is one
for which a user must provide ops
*******************************/
template <typename wrapped_type>
struct traits<
  Vector<
    wrapped_type,
    mpl::enable_if_t<
#ifdef HAVE_ARMADILLO
      !containers::meta::is_vector_armadillo<wrapped_type>::value and
#endif
#ifdef HAVE_BLAZE
      !containers::meta::is_dynamic_vector_blaze<wrapped_type>::value and
#endif
#ifdef HAVE_TRILINOS
      !containers::meta::is_vector_kokkos<wrapped_type>::value and
      !containers::meta::is_vector_epetra<wrapped_type>::value and
      !containers::meta::is_dense_vector_teuchos<wrapped_type>::value and
      !containers::meta::is_vector_tpetra_block<wrapped_type>::value and
      !containers::meta::is_vector_tpetra<wrapped_type>::value and
#endif
      !containers::meta::is_vector_eigen<wrapped_type>::value
      >
    >
  > {

  using wrapped_t = wrapped_type;
  using derived_t = Vector<wrapped_t>;

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::Arbitrary;

  static constexpr WrappedPackageIdentifier
  wrapped_package_identifier = WrappedPackageIdentifier::Arbitrary;

  static constexpr bool is_vector = true;
  static constexpr bool is_matrix = false;
  static constexpr bool is_multi_vector = false;

  // by default, any container is not admissible to expr templates
  // the ones that are, will overwrite this
  static constexpr bool is_admissible_for_expression_templates = false;
};


//*******************************
// Eigen STATIC ROW vector
//*******************************
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     typename
		     std::enable_if<
		       containers::meta::is_static_row_vector_eigen<
			 wrapped_type
			 >::value
		       >::type
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Eigen,
				    true, true>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::EigenRowStatic;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::Scalar;
  using ordinal_t = int;
};


//*******************************
// Eigen STATIC COLUMN vector
//*******************************
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     typename
		     std::enable_if<
		       containers::meta::is_static_column_vector_eigen<
			 wrapped_type
			 >::value
		       >::type
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Eigen,
				    true, true>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::EigenColStatic;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::Scalar;
  using ordinal_t = int;
};


//*******************************
// Eigen DYNAMIC ROW vector
//*******************************
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     typename
		     std::enable_if<
		       containers::meta::is_dynamic_row_vector_eigen<
			 wrapped_type
			 >::value
		       >::type
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Eigen,
				    true, false>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::EigenRowDynamic;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::Scalar;
  using ordinal_t = int;
};


//*******************************
// Eigen DYNAMIC COLUMN vector
//*******************************
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     typename
		     std::enable_if<
		       containers::meta::is_dynamic_column_vector_eigen<
			 wrapped_type
			 >::value
		       >::type
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Eigen,
				    true, false>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::EigenColDynamic;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::Scalar;
  using ordinal_t = int;
};


//*******************************
// Armadillo column vector
//*******************************
#ifdef HAVE_ARMADILLO
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     ::pressio::mpl::enable_if_t<
		       containers::meta::is_column_vector_armadillo<
			 wrapped_type>::value
		       >
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Armadillo,
				    true, false>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier =
    WrappedVectorIdentifier::ArmadilloCol;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::elem_type;
  using ordinal_t = unsigned long;
};
#endif


//*******************************
// Armadillo row vector
//*******************************
#ifdef HAVE_ARMADILLO
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     ::pressio::mpl::enable_if_t<
		       containers::meta::is_row_vector_armadillo<
			 wrapped_type>::value
		       >
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Armadillo,
				    true, false>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier =
    WrappedVectorIdentifier::ArmadilloRow;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::elem_type;
  using ordinal_t = unsigned long;
};
#endif


//*******************************
// Blaze dynamic vector
//*******************************
#ifdef HAVE_BLAZE
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
		     ::pressio::mpl::enable_if_t<
		       containers::meta::is_dynamic_vector_blaze<
			 wrapped_type>::value
		       >
		     >
	      >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
				    WrappedPackageIdentifier::Blaze,
				    true, false>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::BlazeDynamic;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::ElementType;
  using ordinal_t = unsigned long;
};
#endif


//*******************************
// for teuchos serial dense vec
//*******************************
#ifdef HAVE_TRILINOS
template<typename wrapped_type>
struct traits<Vector<wrapped_type,
	  typename
	  std::enable_if<
	    containers::meta::is_dense_vector_teuchos<
	      wrapped_type>::value
	    >::type
	  >
	>
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
			       WrappedPackageIdentifier::Trilinos,
				    true, false>{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::TeuchosSerialDense;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = typename wrapped_type::scalarType;
  using ordinal_t = typename wrapped_type::ordinalType;
};
#endif


//*******************************
// for epetra vector
//*******************************
#ifdef HAVE_TRILINOS
template<typename wrapped_type>
struct traits<Vector<wrapped_type,
	  typename
	  std::enable_if<
	    containers::meta::is_vector_epetra<
	      wrapped_type>::value
	    >::type
	  >
	>
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
			       WrappedPackageIdentifier::Trilinos,
				    false, false>{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::Epetra;

  static constexpr bool is_admissible_for_expression_templates = true;
  using scalar_t = containers::default_types::epetra_scalar_t;
  using local_ordinal_t = containers::default_types::epetra_lo_t;
  using global_ordinal_t = containers::default_types::epetra_go_t1;
  using data_map_t = Epetra_BlockMap;
  using communicator_t = Epetra_Comm;
};
#endif


//*******************************
// for tpetra vector
//*******************************
#ifdef HAVE_TRILINOS
template<typename wrapped_type>
struct traits<Vector<wrapped_type,
	  typename
	  std::enable_if<
	    containers::meta::is_vector_tpetra<
	      wrapped_type>::value
	    >::type
	  >
	>
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
			       WrappedPackageIdentifier::Trilinos,
				    false, false>{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::Tpetra;

  using scalar_t = typename wrapped_type::impl_scalar_type;
  using local_ordinal_t = typename wrapped_type::local_ordinal_type;
  using global_ordinal_t = typename wrapped_type::global_ordinal_type;
  using data_map_t = typename wrapped_type::map_type;

  /* node is a Tpetra concept, defined as:
   * node_type = ::Kokkos::Compat::KokkosDeviceWrapperNode<execution_space>;
   * where memory space is taken from the execution_space
   */
  using node_t = typename wrapped_type::node_type;
  using dual_view_t = typename wrapped_type::dual_view_type;
  // device_type is just an (execution space, memory space) pair.
  // defined as: Kokkos::Device<execution_space, memory_space>
  // so from the device we can get the device execution and memory space
  using device_t = typename wrapped_type::device_type;
  using device_mem_space_t = typename device_t::memory_space;
  using device_exec_space_t = typename device_t::execution_space;
  // store types for host
  using host_mem_space_t = typename Kokkos::HostSpace::memory_space;
  using host_exec_space_t = typename Kokkos::HostSpace::execution_space;

  using dot_t = typename wrapped_type::dot_type;
  using mag_t = typename wrapped_type::mag_type;
  using communicator_t = decltype(std::declval<data_map_t>().getComm());
};
#endif


//*******************************
// Kokkos vector
//*******************************
#ifdef HAVE_TRILINOS
template <typename wrapped_type>
struct traits<Vector<wrapped_type,
	  ::pressio::mpl::enable_if_t<
	    containers::meta::is_vector_kokkos<
	      wrapped_type>::value
	    >
	  >
	>
  : public containers_shared_traits<
  Vector<wrapped_type>,
  wrapped_type,
  true, false, false,
  WrappedPackageIdentifier::Kokkos,
  true, //true because kokkos is for shared mem
  // static view if the number of runtime determined dimensions == 0
  wrapped_type::traits::rank_dynamic==0>
{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::Kokkos;

  using scalar_t	   = typename wrapped_type::traits::value_type;
  using ordinal_t	   = typename wrapped_type::traits::size_type;
  using execution_space    = typename wrapped_type::traits::execution_space;
  using memory_space	   = typename wrapped_type::traits::memory_space;
  using device_type	   = typename wrapped_type::traits::device_type;
  using memory_traits	   = typename wrapped_type::traits::memory_traits;
  using host_mirror_space  = typename wrapped_type::traits::host_mirror_space;
};
#endif


//*******************************
// for block tpetra vector
//*******************************
#ifdef HAVE_TRILINOS
template<typename wrapped_type>
struct traits<
  Vector<wrapped_type,
	 typename
	 std::enable_if<
	   containers::meta::is_vector_tpetra_block<
	     wrapped_type
	     >::value
	   >::type
	 >
  >
  : public containers_shared_traits<Vector<wrapped_type>,
				    wrapped_type,
				    true, false, false,
			       WrappedPackageIdentifier::Trilinos,
				    false, false>{

  static constexpr WrappedVectorIdentifier
  wrapped_vector_identifier = WrappedVectorIdentifier::TpetraBlock;

  using scalar_t = typename wrapped_type::impl_scalar_type;
  using local_ordinal_t = typename wrapped_type::local_ordinal_type;
  using global_ordinal_t = typename wrapped_type::global_ordinal_type;
  using data_map_t = typename wrapped_type::map_type;

  using mag_t = scalar_t;

  /* node is a Tpetra concept, defined as:
   * node_type = ::Kokkos::Compat::KokkosDeviceWrapperNode<execution_space>;
   * where memory space is taken from the execution_space
   */
  using node_t = typename wrapped_type::node_type;

  // device_type is just an (execution space, memory space) pair.
  // defined as: Kokkos::Device<execution_space, memory_space>
  // so from the device we can get the device execution and memory space
  using device_t = typename wrapped_type::device_type;
  using device_mem_space_t = typename device_t::memory_space;
  using device_exec_space_t = typename device_t::execution_space;
  // store types for host
  using host_mem_space_t = typename Kokkos::HostSpace::memory_space;
  using host_exec_space_t = typename Kokkos::HostSpace::execution_space;

  using communicator_t = decltype(std::declval<data_map_t>().getComm());
};
#endif


}}}//end namespace pressio::containers::details
#endif
