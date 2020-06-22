#include <iostream>
#include <stdio.h>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/matmul.h>
#include <NTL/BasicThreadPool.h>
#include "MyMatMulFull.h"
#include "readConstant.h"
#include "CyclicRotation_chi.h"
#include <sys/time.h>
#include <sys/resource.h>
#include "Rasta_80_327.h"


//void rasta_327_80(int rounds, vector<long> user_key,vector<vector<long>> constants, vector<MatMulFullExec>& MMFE_matrices, Ctxt& c, const EncryptedArray& ea);


int main(int argc, char *argv[]){

  
    vector<long> test_key(329,1);
    test_key[328] = 0; 
    test_key[327] = 0; 

  //############################initializing #####################################
  clock_t BGVinit_begin = clock();
 
        //BGV Initialization
  // Plaintext prime modulus
  unsigned long p = 2; // p = 2 (mod 2) 
  // Cyclotomic polynomial - defines phi(m)
  unsigned long m = 30269; // over 10.000 oddetall, default: 32109 
  // Hensel lifting (default = 1)
  unsigned long r = 1; //stay as one
  // Number of bits of the modulus chain
  unsigned long bits = 300; //100-300 , default: 300
  // Number of columns of Key-Switching matrix (default = 2 or 3)
  unsigned long c = 2; // c = 2,3? default: 2
  
  std::cout << "Initialising context object..." << std::endl;
  // Intialise context
  FHEcontext context(m, p, r);
  // Modify the context, adding primes to the modulus chain
  std::cout  << "Building modulus chain..." << std::endl;
  buildModChain(context, bits, c);

  // Print the security level
  std::cout << "Security: " << context.securityLevel() << std::endl;

  // Secret key management
  std::cout << "Creating secret key..." << std::endl;
  // Create a secret key associated with the context
  FHESecKey secret_key(context);
  // Generate the secret key
  secret_key.GenSecKey();
  std::cout << "Generating key-switching matrices..." << std::endl;
  // Compute key-switching matrices that we need
  addSome1DMatrices(secret_key);

  // Public key management
  // Set the secret key (upcast: FHESecKey is a subclass of FHEPubKey)
  const FHEPubKey& public_key = secret_key;
  
  // Get the EncryptedArray of the context
  const EncryptedArray& ea = *(context.ea);

    // Get the number of slots (phi(m))
  long nslots = ea.size();
  std::cout << "Number of slots: " << nslots << std::endl; 

    // Create a vector of long with nslots elements
    //std::vector<long> ptxt(nslots); 
  

  // Create a ciphertext
  Ctxt ctxt(public_key);

  // Encrypt the plaintext using the public_key
  ea.encrypt(ctxt, public_key, test_key);

  // Create a plaintext for decryption
  std::vector<long> decrypted(nslots);

  clock_t BGVinit_end = clock();
  //#########bgv end of initzialise ######################

  clock_t read_matrix_timer_start = clock();

  //creates a vector of matmulFullexec matrices for use in the affine layer
  vector<MatMulFullExec> MMFE_matrices; 

  //reads in matrices from different files
  MyMatMulFull my_MMF_matrix0(ea, "matrix329_0.txt");
  MyMatMulFull my_MMF_matrix1(ea, "matrix329_1.txt");
  MyMatMulFull my_MMF_matrix2(ea, "matrix329_2.txt");
  MyMatMulFull my_MMF_matrix3(ea, "matrix329_3.txt");
  MyMatMulFull my_MMF_matrix4(ea, "matrix329_4.txt");
  //MyMatMulFull my_MMF_matrix5(ea, "matrix329_5.txt"); 

  //converts the MatMulFull objects into MarMulFullExec objects
  MatMulFullExec MMFE_matrix0(my_MMF_matrix0, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix1(my_MMF_matrix1, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix2(my_MMF_matrix2, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix3(my_MMF_matrix3, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix4(my_MMF_matrix4, false); //create the matmulFullExec object
  //MatMulFullExec MMFE_matrix5(my_MMF_matrix5, false); //create the matmulFullExec object 

  //adds the different matrices to the vector of MatMulFullExec matrices
  MMFE_matrices.push_back(MMFE_matrix0);
  MMFE_matrices.push_back(MMFE_matrix1);
  MMFE_matrices.push_back(MMFE_matrix2);
  MMFE_matrices.push_back(MMFE_matrix3);
  MMFE_matrices.push_back(MMFE_matrix4);
  //MMFE_matrices.push_back(MMFE_matrix5); 

  //vector<long> constants = readConstantsFromFile(ea,"constants329.txt"); //read in constants, obsolete

  vector<vector<long>> constants = readConstantsFromFile_all(ea, "constants329.txt"); //(stores all constants as a matrix )

  clock_t read_matrix_timer_end = clock();
  double read_matrix_time = (double)(read_matrix_timer_end-read_matrix_timer_start)/CLOCKS_PER_SEC;
    
    
  //#######################initiliaze end ###############################################

  cout << "starting Rasta: " << endl; 
  //cout << "starting Rasta, with key: " << test_key << endl; //shows initial key
  clock_t Rasta_start = clock(); 
  
  rasta_327_80(4, test_key, constants, MMFE_matrices, ctxt, ea); 

  clock_t Rasta_end = clock(); 
  clock_t total_time_end = clock();

  //ea.decrypt(ctxt, secret_key, test_key);
  //cout << "end decrypted: "  << endl;
  //cout << "end decrypted: " << ptxt << endl; //shows decrypted result. 

  double total_time = (double)(total_time_end-BGVinit_begin)/CLOCKS_PER_SEC;
  double initBGV_time =  (double)(BGVinit_end-BGVinit_begin)/CLOCKS_PER_SEC;
  double rasta_time =  (double)(Rasta_end-Rasta_start)/CLOCKS_PER_SEC; 
  

  cout << "timed results: " << endl; 
  cout << "total time: " << total_time << endl;
  cout << "initBGV  time: " << initBGV_time << endl;
  cout << "read in matrix phase timer" << read_matrix_time << endl; 
  cout <<  "rasta time:"<< rasta_time << endl; 

    
    return 0;
}

