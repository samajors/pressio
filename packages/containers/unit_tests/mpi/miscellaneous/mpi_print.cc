#include <gtest/gtest.h>
#include "CONTAINERS_BASIC"

TEST(containers_basic, mpiPrint){
  auto a1 = "fr";
  int b1 = 2;
  double b2 = 44.4;
  rompp::utils::io::print_stdout(a1,b1,b2);
}