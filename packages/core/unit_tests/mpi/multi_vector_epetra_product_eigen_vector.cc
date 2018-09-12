
#include <gtest/gtest.h>
#include "Epetra_MpiComm.h"
#include "CORE_ALL"

struct epetraFix
  : public ::testing::Test{
public:
  int rank_;
  Epetra_MpiComm * Comm_;
  int MyPID_;
  int NumProc_;
  const int localSize_ = 3;
  const int numVectors_ = 4;
  int numGlobalEntries_;
  Epetra_Map * dataMap_;
  Epetra_MultiVector * mv_;
  
  virtual void SetUp(){
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    Comm_ = new Epetra_MpiComm(MPI_COMM_WORLD);
    MyPID_ = Comm_->MyPID();
    NumProc_ = Comm_->NumProc();
    assert(NumProc_ == 3);
    numGlobalEntries_ = Comm_->NumProc() * localSize_;
    dataMap_ = new Epetra_Map(numGlobalEntries_, 0, *Comm_);
    mv_ = new Epetra_MultiVector(*dataMap_, numVectors_);
  }

  virtual void TearDown(){
    delete Comm_;
    delete dataMap_;
    delete mv_;
  }
};


TEST_F(epetraFix, MVecEpetraProductEigenVector){

  using mvec_t = core::MultiVector<Epetra_MultiVector>;
  STATIC_ASSERT_IS_CORE_MULTI_VECTOR_WRAPPER(mvec_t);
  mvec_t MV(*mv_);

  EXPECT_EQ( MV.globalNumVectors(), 4 );
  EXPECT_EQ( MV.localNumVectors(), 4 );
  EXPECT_EQ( MV.globalLength(), 9 );
  EXPECT_EQ( MV.localLength(), 3);
  for (int i=0; i<localSize_; i++)
    for (int j=0; j<MV.globalNumVectors(); j++)
      EXPECT_NEAR( 0.0, MV(i,j), 1e-12);

  if(MyPID_==0){
    MV(0,0) = 3.2;
    MV(1,0) = 1.2;
    MV(2,1) = 4;
    MV(0,1) = 1.2;}
  if(MyPID_==1){
    MV(2,2) = 3;}

  MV.data()->Print(std::cout);
  //----------

  using eigv_t = Eigen::Matrix<double,4,1>;
  eigv_t bn;
  using vec_t = core::Vector<eigv_t>;
  STATIC_ASSERT_IS_CORE_VECTOR_WRAPPER(vec_t);
  vec_t b(bn);
  b[0] = 1.;
  b[1] = 2.;
  b[2] = 3.;
  b[3] = 4.;

  auto res = core::ops::product(MV, b);
  res.data()->Print(std::cout);

  if (rank_==0){
    EXPECT_DOUBLE_EQ( res[0], 5.6);
    EXPECT_DOUBLE_EQ( res[1], 1.2);
    EXPECT_DOUBLE_EQ( res[2], 8.0);
  }

  if (rank_==1){
    EXPECT_DOUBLE_EQ( res[0], 0.0);
    EXPECT_DOUBLE_EQ( res[1], 0.0);
    EXPECT_DOUBLE_EQ( res[2], 9.0);
  }

  if (rank_==2){
    EXPECT_DOUBLE_EQ( res[0], 0.);
    EXPECT_DOUBLE_EQ( res[1], 0.);
    EXPECT_DOUBLE_EQ( res[2], 0.);
  }

}