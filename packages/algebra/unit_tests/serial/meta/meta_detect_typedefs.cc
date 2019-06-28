
#include <gtest/gtest.h>
#include "ALGEBRA_BASIC"

TEST(algebra_meta_detect_typedefs, scalarTypedefDetect)
{
  using namespace rompp;

  class A{
  public:
    using scalar_type = double;
    scalar_type x;
  };
  static_assert( ::rompp::algebra::meta::has_scalar_typedef<A>::value, "");

  class B{
  public:
    using scalar_t = double;
    scalar_t x;
  };
  static_assert( ::rompp::algebra::meta::has_scalar_typedef<B>::value == false, "");

  struct C{
    using scalar_type = double;
    scalar_type x;
  };
  static_assert( ::rompp::algebra::meta::has_scalar_typedef<C>::value, "");
}


TEST(algebra_meta_detect_typedefs, ordinalTypedefDetect)
{
  using namespace rompp;
  class A{
  public:
    using ordinal_type = int;
    ordinal_type x;
  };
  EXPECT_EQ( algebra::meta::has_ordinal_typedef<A>::value, true);

  class B{
  public:
    using ordinal_t = int;
    ordinal_t x;
  };
  EXPECT_EQ( algebra::meta::has_ordinal_typedef<B>::value, false);

  struct C{
    using ordinal_type = int;
    ordinal_type x;
  };
  EXPECT_EQ( algebra::meta::has_ordinal_typedef<C>::value, true);  
}


TEST(algebra_meta_detect_typedefs, localglobalOrdinalTypedefDetect)
{
  using namespace rompp;

  class A{
  public:
    using local_ordinal_type = int;
    local_ordinal_type x;
    using global_ordinal_type = int;
    global_ordinal_type x2;
  };
  EXPECT_EQ( algebra::meta::has_local_ordinal_typedef<A>::value, true);
  EXPECT_EQ( algebra::meta::has_global_ordinal_typedef<A>::value, true);

  class B{
  public:
    using local_ordinal_t = int;
    local_ordinal_t x;
    using global_ordinal_t = int;
    global_ordinal_t x2;
  };
  EXPECT_EQ( algebra::meta::has_local_ordinal_typedef<B>::value, false);
  EXPECT_EQ( algebra::meta::has_global_ordinal_typedef<B>::value, false);

  struct C{
    using local_ordinal_type = int;
    local_ordinal_type x;
    using global_ordinal_type = int;
    global_ordinal_type x2;
  };
  EXPECT_EQ( algebra::meta::has_local_ordinal_typedef<C>::value, true);  
  EXPECT_EQ( algebra::meta::has_global_ordinal_typedef<C>::value, true);
}


TEST(algebra_meta_detect_typedefs, mapCommTypedefDetect)
{
  using namespace rompp;

  class A{
  public:
    using data_map_type = int;
    data_map_type x;
    using communicator_type = int;
    communicator_type x2;
  };
  EXPECT_EQ( algebra::meta::has_data_map_typedef<A>::value, true);
  EXPECT_EQ( algebra::meta::has_communicator_typedef<A>::value, true);

  class B{
  public:
    using data_map_t = int;
    data_map_t x;
    using communicator_t = int;
    communicator_t x2;
  };
  EXPECT_EQ( algebra::meta::has_data_map_typedef<B>::value, false);
  EXPECT_EQ( algebra::meta::has_communicator_typedef<B>::value, false);

  struct C{
    using data_map_type = int;
    data_map_type x;
    using communicator_type = int;
    communicator_type x2;
  };
  EXPECT_EQ( algebra::meta::has_data_map_typedef<C>::value, true);  
  EXPECT_EQ( algebra::meta::has_communicator_typedef<C>::value, true);
}

