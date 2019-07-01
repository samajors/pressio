
#include <gtest/gtest.h>
#include "CONTAINERS_ALL"
#include "Epetra_MpiComm.h"

struct containers_matrix_dense_to_crs_distributed_epetraFix
  : public ::testing::Test{
public:
  int rank_;
  Epetra_MpiComm * Comm_;
  int MyPID_;
  int NumProc_;

  // containers::Matrix<Epetra_CrsMatrix> * As_;
  // Epetra_BlockMap * smMap_;
  containers::Matrix<Epetra_MultiVector> * Ad_;
  Epetra_BlockMap * dmMap_;
  
  virtual void SetUp(){
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    Comm_ = new Epetra_MpiComm(MPI_COMM_WORLD);
    MyPID_ = Comm_->MyPID();
    NumProc_ = Comm_->NumProc();
    assert( NumProc_ == 3 );

    // // CRS matrix
    // smMap_ = new Epetra_Map(9, 0, *Comm_);
    // As_ = new containers::Matrix<Epetra_CrsMatrix>(*smMap_, 5);
    // //------------------------------------------

    // setup dense and vector for test 2
    dmMap_ = new Epetra_Map(9, 0, *Comm_);
    Ad_ = new containers::Matrix<Epetra_MultiVector>(*dmMap_, 5);
    //------------------------------------------    
  }
  
  virtual void TearDown(){
    delete Comm_;
    // delete smMap_;
    // delete As_;
    delete dmMap_;
    delete Ad_;
  }
};


TEST_F(containers_matrix_dense_to_crs_distributed_epetraFix, CRSMatTimesVector)
{
  {
    if (MyPID_ == 0){
      (*Ad_)(0,0) = 1.0;
      (*Ad_)(0,2) = 2.0;
      (*Ad_)(0,3) = 3.0;
      (*Ad_)(0,4) = 1.0;
    }
    if (MyPID_ == 1){
      (*Ad_)(1,0) = 3.1;
      (*Ad_)(1,2) = 2.2;
      (*Ad_)(1,3) = 3.3;
      (*Ad_)(1,4) = 4.4;
    }
    if (MyPID_ == 2){
      (*Ad_)(1,0) = 3.0;
      (*Ad_)(1,2) = 2.0;
      (*Ad_)(1,3) = 3.0;
      (*Ad_)(1,4) = 4.0;
    }
    Ad_->data()->Print(std::cout);
  }
  auto B = containers::denseToSparse(*Ad_);
  B.data()->Print(std::cout);

  std::vector<int> ind(5);
  std::vector<double> val(5);  
  int numEntr;  
  if(MyPID_==0){
    B.data()->ExtractGlobalRowCopy(0, 5, numEntr, val.data(), ind.data() );
    EXPECT_EQ(numEntr,5);
    for (int k=0; k<5; k++){
      if (ind[k]==0)
        EXPECT_DOUBLE_EQ( val[k], 1.0 );
      if (ind[k]==1)
        EXPECT_DOUBLE_EQ( val[k], 0.0 );
      if (ind[k]==2)
        EXPECT_DOUBLE_EQ( val[k], 2.0 );
      if (ind[k]==3)
        EXPECT_DOUBLE_EQ( val[k], 3.0 );
      if (ind[k]==4)
        EXPECT_DOUBLE_EQ( val[k], 1.0 );      
    }

    B.data()->ExtractGlobalRowCopy(1, 5, numEntr, val.data(), ind.data() );
    EXPECT_DOUBLE_EQ( val[0], 0.0 );
    EXPECT_DOUBLE_EQ( val[1], 0.0 );
    EXPECT_DOUBLE_EQ( val[2], 0.0 );
    EXPECT_DOUBLE_EQ( val[3], 0.0 );
    EXPECT_DOUBLE_EQ( val[4], 0.0 );
  }

  if(MyPID_==1){
    std::vector<double> val(5);
    B.data()->ExtractGlobalRowCopy(4, 5, numEntr, val.data(), ind.data() );

    EXPECT_EQ(numEntr,5);
    for (int k=0; k<5; k++){
      if (ind[k]==0)
        EXPECT_DOUBLE_EQ( val[k], 3.1 );
      if (ind[k]==1)
        EXPECT_DOUBLE_EQ( val[k], 0.0 );
      if (ind[k]==2)
        EXPECT_DOUBLE_EQ( val[k], 2.2 );
      if (ind[k]==3)
        EXPECT_DOUBLE_EQ( val[k], 3.3 );
      if (ind[k]==4)
        EXPECT_DOUBLE_EQ( val[k], 4.4 );      
    }
  }

  if(MyPID_==2){
    std::vector<double> val(5);
    B.data()->ExtractGlobalRowCopy(7, 5, numEntr, val.data(), ind.data() );
    EXPECT_EQ(numEntr,5);
    for (int k=0; k<5; k++){
      if (ind[k]==0)
        EXPECT_DOUBLE_EQ( val[k], 3. );
      if (ind[k]==1)
        EXPECT_DOUBLE_EQ( val[k], 0.0 );
      if (ind[k]==2)
        EXPECT_DOUBLE_EQ( val[k], 2. );
      if (ind[k]==3)
        EXPECT_DOUBLE_EQ( val[k], 3. );
      if (ind[k]==4)
        EXPECT_DOUBLE_EQ( val[k], 4. );      
    }
  }
  


  // //-----------
  // // FILL A
  // //-----------
  // {
  //   int myNR = smMap_->NumMyElements();

  //   std::vector<int> mygid(myNR);
  //   smMap_->MyGlobalElements( mygid.data() );
  //   std::array<double,5> vals;
  //   std::array<int,5> colind;
  //   for (auto const & it : mygid){
  //     if(it == 0){
  //     	vals = {1., 1., 1., 3., 3.};
  //     	colind = {0, 4, 7, 11, 12};
  //     	As_->insertGlobalValues(it, 5, vals.data(), colind.data());
  //     }
  //     if(it == 6){
  //     	vals = {1., 3., 6.};
  //     	colind = {1, 4, 7};
  //     	As_->insertGlobalValues(it, 3, vals.data(), colind.data());
  //     }

  //   }
  //   //    As_->fillingIsCompleted(*vMap_, *smMap_);
  //   As_->data()->Print(std::cout);
  // }
  
  // //-----------
  // // FILL b
  // //-----------
  // {
  //   if (MyPID_ == 0){
  //     (*b_)[0] = 1.;
  //     (*b_)[4] = 3.;
  //   }
  //   if (MyPID_ == 1){
  //     (*b_)[2] = 1.;
  //   }
  //   if (MyPID_ == 2){
  //     (*b_)[2] = 2.;
  //     (*b_)[3] = 2.;
  //   }
  //   b_->data()->Print(std::cout);
  // }
  
  // //------------------
  // // product: b = A b
  // //------------------
  // auto c = containers::matrixVectorProduct(*A_, *b_);
  // c.data()->Print(std::cout);

  // assert( c.globalSize() == 9);
  // static_assert( std::is_same<decltype(c),
  // 		 containers::Vector<Epetra_Vector>>::value, "" );
  // if (MyPID_ == 0){
  //   EXPECT_DOUBLE_EQ( c[0], 17. );
  // }
  // if (MyPID_ == 2)
  //   EXPECT_DOUBLE_EQ( c[0], 15. );
  
}
///////////////////////////////////////////
///////////////////////////////////////////



// TEST_F(containers_matrix_vec_product_distributed_epetraFix, DenseMatTimesVector)
// {

//   //-----------
//   // FILL A
//   //-----------
//   {
//     if (MyPID_ == 0){
//       (*Ad_)(0,0) = 1.0;
//       (*Ad_)(0,2) = 2.0;
//       (*Ad_)(0,3) = 3.0;
//       (*Ad_)(0,4) = 1.0;
//     }
//     if (MyPID_ == 1){
//       (*Ad_)(1,0) = 3.0;
//       (*Ad_)(1,2) = 2.0;
//       (*Ad_)(1,3) = 3.0;
//       (*Ad_)(1,4) = 4.0;
//     }

//     Ad_->data()->Print(std::cout);
//   }

//   //-----------
//   // FILL b
//   //-----------
//   {
//     if (MyPID_ == 0){
//       (*b2_)[0] = 1.;
//       (*b2_)[1] = 2.;
//     }
//     if (MyPID_ == 1){
//       (*b2_)[0] = 1.;
//     }
//     if (MyPID_ == 2){
//       (*b2_)[0] = 1.;
//     }
//     b2_->data()->Print(std::cout);
//   }
  
//   //------------------
//   // product: b = A b
//   //------------------
//   auto c = containers::matrixVectorProduct( *Ad_, *b2_ );  
//   c.data()->Print(std::cout);

//   assert( c.globalSize() == 9);
//   static_assert( std::is_same<decltype(c),
//   		 containers::Vector<Epetra_Vector>>::value, "" );
//   if (MyPID_ == 0){
//     EXPECT_DOUBLE_EQ( c[0], 4. );
//   }
//   if (MyPID_ == 1)
//     EXPECT_DOUBLE_EQ( c[1], 9. );
  
// }