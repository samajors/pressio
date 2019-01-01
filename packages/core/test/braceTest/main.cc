
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "CORE_VECTOR"
#include "CORE_OPS"
//#include "Eigen/Dense"

template<typename T, int n>
class OdeStorage;

template<typename T>
class OdeStorage<T, 1>{
public:
  OdeStorage(T const & y0) : auxStates_{y0}{}
  ~OdeStorage() = default;
protected:
  std::array<T, 1> auxStates_;
};

int main(int argc, char *argv[]){

  using eigvec_t = Eigen::Matrix<double, 3/*Eigen::Dynamic*/, 1>;
  using myvec_t = rompp::core::Vector<eigvec_t>;

  eigvec_t a;
  OdeStorage<eigvec_t,1> O(a);

  myvec_t a1;
  OdeStorage<myvec_t,1> O1(a1);

  return 0;
}





/*
// #define TEST_FIND_SUBSTR_IN_STR(SUBSTR, STR) \
//   { \
//     const bool foundSubStr = ((STR).find(SUBSTR) != std::string::npos); \
//     std::cout << "Found \"" SUBSTR "\" ? " << foundSubStr << "\n"; \
//     if (!foundSubStr) success=false; \
//   } \
//   (void)(success)


// int main() {

//   bool success = true;
//   std::cout << std::boolalpha;

//   SimpleCxx::HelloWorld helloWorld;
//   std::ostringstream oss;
//   helloWorld.printHelloWorld(oss);
//   std::cout << oss.str();

//   TEST_FIND_SUBSTR_IN_STR("Hello World", oss.str());

// #ifdef HAVE_SIMPLECXX___INT64
//   TEST_FIND_SUBSTR_IN_STR("We have __int64", oss.str());
// #endif

// #ifdef HAVE_SIMPLECXX_DEBUG
//   TEST_FIND_SUBSTR_IN_STR("Debug is enabled", oss.str());
// #else
//   TEST_FIND_SUBSTR_IN_STR("Release is enabled", oss.str());
// #endif

//   TEST_FIND_SUBSTR_IN_STR("Sqr(3) = 9", oss.str());

// #ifdef HAVE_SIMPLECXX_SIMPLETPL
//   TEST_FIND_SUBSTR_IN_STR("Cube(3) = 27", oss.str());
// #endif

//   if (success) {
//     std::cout << "End Result: TEST PASSED\n";
//   }
//   else {
//     std::cout << "End Result: TEST FAILED\n";
//   }
//   return (success ? EXIT_SUCCESS : EXIT_FAILURE);

// }
*/