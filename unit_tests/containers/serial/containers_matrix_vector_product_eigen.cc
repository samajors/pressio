
#include <gtest/gtest.h>
#include "CONTAINERS_ALL"

TEST(containers_matrix_vector_product, eigenVectorDenseMatrix){
  using namespace pressio;

  using natV_t = Eigen::Matrix<double,3,1>;
  natV_t a; a << 4.,2.,6;
  containers::Vector<natV_t> myV(a);
  
  using natM_t = Eigen::Matrix<double,3,3>;
  natM_t M;
  M << 1,0,2,2,1,3,0,0,1;
  containers::Matrix<natM_t> myM(M);

  containers::Vector<natV_t> myR;
  containers::ops::product(myM, myV, myR);
  EXPECT_DOUBLE_EQ( myR[0], 16.0);
  EXPECT_DOUBLE_EQ( myR[1], 28.0);
  EXPECT_DOUBLE_EQ( myR[2], 6.0);
}//end TEST


TEST(containers_matrix_vector_product, eigenVectorSparseMatrix){
  using namespace pressio;
  // create vector
  using natV_t = Eigen::Matrix<double,3,1>;
  natV_t a; a << 4.,2.,6;
  containers::Vector<natV_t> myV(a);

  // create sparse matrix
  using mymat_t = containers::Matrix<
    Eigen::SparseMatrix<double,Eigen::RowMajor,int>>;
  mymat_t myM(4,3);
  using veci = std::vector<int>;
  using vecd = std::vector<double>;
  std::map<int,std::pair<vecd,veci>> DD;
  DD[0] = std::make_pair<vecd,veci>({1.,2.}, {0,2});
  DD[1] = std::make_pair<vecd,veci>({2.,3.}, {0,2});
  DD[2] = std::make_pair<vecd,veci>({1}, {2});
  DD[3] = std::make_pair<vecd,veci>({1}, {1});
  for (auto const & it : DD){
    int ind = it.first;
    vecd data = std::get<0>(it.second);
    veci cols = std::get<1>(it.second);
    myM.insertValues(ind,(int)data.size(),data.data(),cols.data());
  }
    
  containers::Vector<Eigen::Matrix<double,4,1>> myR;
  containers::ops::product(myM, myV, myR);
  EXPECT_DOUBLE_EQ( myR[0], 16.0);
  EXPECT_DOUBLE_EQ( myR[1], 26.0);
  EXPECT_DOUBLE_EQ( myR[2], 6.0);
  EXPECT_DOUBLE_EQ( myR[3], 2.0);
}//end TEST