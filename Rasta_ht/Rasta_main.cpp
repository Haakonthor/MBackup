#include <iostream>
#include <stdio.h>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/matmul.h>
#include <NTL/BasicThreadPool.h>
#include "MyMatMulFull.h"
#include "readConstant.h"
#include "Non_singular_matrix_generation.h"
#include <sys/time.h>
#include <sys/resource.h>



//take a plaintext
/*
void Rasta(int rounds, vector<long> key){



  MyMatMulFull my_MMF_matrix(ea, "matrices329.txt"); //read in and create a matmullfull object
  MatMulFullExec MMFE_matrix(my_MMF_matrix, false); //create the matmulFullExec object

  //MMFE_matrix.mul(ctxt1); //multiply with ciphertext

  vector<long> constants = readConstantsFromFile(ea,"constants329.txt"); //read in constants
  cout<<"printing constans: " <<constants << endl; 
  cout << "constants size: " << constants.size() << endl; 


  addConstants(ctxt1, ea, constants); //add the constants to the object

    //do the bitshift for keccak
  Ctxt temp1 = ctxt1;
  cyclicBitShift(temp1,ea);
  Ctxt temp2 = temp1;
  cyclicBitShift(temp2,ea);

  //keccak_chi(ctxt1, ea);


  

  addKey(ctxt1, ea, key);

  ea.decrypt(ctxt1, secret_key, ptxt1);
  cout << "end decrypted: " << ptxt1 << endl; 

}
*/ 

void rasta_327_80(int rounds, vector<long> user_key,vector<vector<long>> constants, vector<MatMulFullExec>& MMFE_matrices, Ctxt& c, const EncryptedArray& ea);


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
  // Number of columns of Key-Switching matix (default = 2 or 3)
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

    // Get the number of slot (phi(m))
  long nslots = ea.size();
  std::cout << "Number of slots: " << nslots << std::endl; //()så mange slots som mulig? //baseres på m? 

    // Create a vector of long with nslots elements
  std::vector<long> ptxt(nslots); //key?
  // Set it with numbers 0..nslots - 1
  //for (int i = 0; i < nslots; ++i) {
  //  ptxt[i] = i%2; //i%2 
  //}
  //ptxt[nslots-2] = 0; 

  // Create a ciphertext
  Ctxt ctxt(public_key);

  // Encrypt the plaintext using the public_key
  ea.encrypt(ctxt, public_key, test_key);

  // Create a plaintext for decryption
  std::vector<long> decrypted(nslots);

  //#########bgv end of initzialise

  //MyMatMulFull my_MMF_matrix(ea, "matrices329.txt"); //read in and create a matmullfull object (only first matrix for now? just gonna use the first for now )
  //MatMulFullExec MMFE_matrix(my_MMF_matrix, false); //create the matmulFullExec object

  vector<MatMulFullExec> MMFE_matrices; 

  MyMatMulFull my_MMF_matrix1(ea, "matrix329_1.txt");
  MyMatMulFull my_MMF_matrix2(ea, "matrix329_2.txt");
  MyMatMulFull my_MMF_matrix3(ea, "matrix329_3.txt");
  MyMatMulFull my_MMF_matrix4(ea, "matrix329_4.txt");
  //MyMatMulFull my_MMF_matrix5(ea, "matrix329_5.txt");

  MatMulFullExec MMFE_matrix1(my_MMF_matrix1, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix2(my_MMF_matrix2, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix3(my_MMF_matrix3, false); //create the matmulFullExec object
  MatMulFullExec MMFE_matrix4(my_MMF_matrix4, false); //create the matmulFullExec object
  //MatMulFullExec MMFE_matrix5(my_MMF_matrix5, false); //create the matmulFullExec object

  MMFE_matrices.push_back(MMFE_matrix1);
  MMFE_matrices.push_back(MMFE_matrix2);
  MMFE_matrices.push_back(MMFE_matrix3);
  MMFE_matrices.push_back(MMFE_matrix4);
  //my_MMF_matrices.push_back(MMFE_matrix5);

  

  //vector<long> constants = readConstantsFromFile(ea,"constants329.txt"); //read in constants
  //cout<<"printing constans: " <<constants << endl; 
  //cout << "constants size: " << constants.size() << endl; 

  vector<vector<long>> constants = readConstantsFromFile_all(ea, "constants329.txt"); //(stores all constants ase a matrix for ease)

  clock_t BGVinit_end = clock();
  
    
  //#######################initiliaze end? ###############################################

  //round 1

  cout << "starting Rasta, with key: " << test_key << endl; 
  clock_t Rasta_start = clock(); 
  
  rasta_327_80(4, test_key, constants, MMFE_matrices, ctxt, ea); 

  clock_t Rasta_end = clock(); 
  clock_t total_time_end = clock();

  ea.decrypt(ctxt, secret_key, ptxt);
  cout << "end decrypted: " << ptxt << endl;

  double total_time = (double)(total_time_end-BGVinit_begin)/CLOCKS_PER_SEC;
  double initBGV_time =  (double)(BGVinit_end-BGVinit_begin)/CLOCKS_PER_SEC;
  double rasta_time =  (double)(Rasta_end-Rasta_start)/CLOCKS_PER_SEC; 
  

  cout << "timed results: " << endl; 
  cout << "total time: " << total_time << endl;
  cout << "initBGV  time: " << initBGV_time << endl;
  cout <<  "rasta time:"<< rasta_time << endl; 

    return 0;
}

void rasta_327_80(int rounds, vector<long> user_key, vector<vector<long>> constants, vector<MatMulFullExec>& MMFE_matrices, Ctxt& c, const EncryptedArray& ea){
  int i;
  int counter = 0; 
  double chi_time; 
  cout << "starting rasta rounds." << endl; 
  for(i = 0; i<rounds-1; i++){
    cout << "round: " << i << endl; 
    MMFE_matrices[i].mul(c);
    cout << "matrix.mul(c) ended" << endl; 
    addConstants(c, ea, constants[i]);
    cout << "add constants ended." << endl; 
    

    clock_t chi_timer_start = clock(); 
    keccak_chi(c,ea);  
    clock_t chi_timer_end = clock();
    chi_time =  (double)(chi_timer_end-chi_timer_start)/CLOCKS_PER_SEC;  


    cout << "keccak chi ended" << endl; 
    counter = counter + 1;
  }
  cout <<  "counter: " <<counter << endl;
  cout << "last round without keccak i:  " << i << endl; 

  clock_t matmul_timer_start = clock();
  MMFE_matrices[counter].mul(c);
  clock_t matmul_timer_end = clock(); 

  cout << "multiplying in C last time" << endl; 

  clock_t adding_const_timer_start = clock();
  addConstants(c, ea, constants[counter]);
  clock_t adding_const_timer_end = clock(); 
  cout << "last  add constant" << endl; 
  addKey(c, ea, user_key);
  cout << "added key again" << endl; 

  double matmul_time =  (double)(matmul_timer_end-matmul_timer_start)/CLOCKS_PER_SEC; 
  double addConst_time =  (double)(adding_const_timer_end-adding_const_timer_start)/CLOCKS_PER_SEC;
  cout <<"matMul timer:" << matmul_time << endl;
  cout <<"addingConst timer:" << addConst_time << endl;
  cout <<"chi timer: " << chi_time << endl;
  
}