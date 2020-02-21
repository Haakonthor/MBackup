#include <iostream>
#include <stdio.h>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/matmul.h>
#include <NTL/BasicThreadPool.h>
#include "MyMatMulFull.h"
#include "readConstant.h"
//#include "matrix.cpp"
//#include "MyMatMulFull.h"
//#include <NTL/RR.h>



int main(int argc, char *argv[]) {

  /*  Example of BGV scheme  */

 
  //################ adding c1*c2+c1 ###########################
  // Plaintext prime modulus
  unsigned long p = 2; // p = 2 (mod 2) 
  // Cyclotomic polynomial - defines phi(m)
  unsigned long m = 10001; // over 10.000 oddetall, default: 32109 
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

  // Print the context
  context.zMStar.printout();
  std::cout << std::endl;
  
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
  std::vector<long> ptxt1(nslots);
  // Set it with numbers 0..nslots - 1
  for (int i = 0; i < nslots; ++i) {
    ptxt1[i] = i%2; //i%2 
  }
  
  //create a vector of 16 slots for testing purposes, manually assigned
  std::vector<long> ptxt2={1,0,1,0,1,1,0,1,1,1,0,0,1,1,1,1};
  //std::cout<< "ptxt2" << ptxt2 << std::endl;
  

  // Create a ciphertext
  Ctxt ctxt1(public_key);
  Ctxt ctxt2(public_key);

  // Print the plaintext
  //std::cout << "Initial Ptxt1:   " << ptxt1 << std::endl;
  //std::cout << "Initial Ptxt2:  " << ptxt2 << std::endl;
  
  //original cipher text
  //std::cout<< "initial cipher text, 'ctxt'" << ctxt << std::endl;

  // Encrypt the plaintext using the public_key
  ea.encrypt(ctxt1, public_key, ptxt1);
  ea.encrypt(ctxt2, public_key, ptxt2);  //using ea.encrypt with the second values
  //std::cout<<"encrypted ctxt" << ctxt1 << std::endl;
  
  

  // Square the ciphertext
  //ctxt *= ctxt;
  // Double it (using additions)
  //ctxt += ctxt;

  //formula c1*c2 + c1
  //ctxt2 *= ctxt1;
  //ctxt2 += ctxt1;
  

  // Create a plaintext for decryption
  std::vector<long> decrypted(nslots);
  // Decrypt the modified ciphertext
  //ea.decrypt(ctxt, secret_key, decrypted);
  ea.decrypt(ctxt2, secret_key, decrypted);
  
  // Print the decrypted plaintext
  //std::cout<< "Decrypting on the plaintexts after c1*c2+c1"<<std::endl;
  //std::cout << "Decrypted Ptxt1: " << decrypted << std::endl;

  //===================== Using MyMatMulFull ===============

  MyMatMulFull my_MMF_matrix(ea, "testm16.txt"); //read in and create a matmullfull object

  //my_MMF_matrix.print(); //terrible printing, should be improved

  std::cout << std::endl; 
  MatMulFullExec MMFE_matrix(my_MMF_matrix, false); //create a matmullfullexec object
  MMFE_matrix.mul(ctxt2);
  ea.decrypt(ctxt2, secret_key, ptxt2); //decrypter ctxt2 inn i ptxt2 
  std::cout << "ptxt2*M = \n" << ptxt2 << std::endl;

  //vector<ZZ> testConst = readConstantsFromFile2("testC16.txt");

  //==================working way to add constant ===================
  //vector<long> testConst = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  //ZZX emask;
  //vector<long> pmask(nslots,0);
  //ea.encode(emask,testConst);
  //ctxt2.addConstant(emask);
  //###################

  //##### actually reading in the constants from file and, adding them to the result after v*Mat, hence v*Mat+const
  vector<long> constants = readConstantsFromFile(ea,"testC16.txt");
  cout << "read the constants from file" << endl;

  addConstants(ctxt2, ea, constants);
  ea.decrypt(ctxt2, secret_key, ptxt2);
  std::cout << "ptxt2*M+constants = \n" << ptxt2 << std::endl;
  
/*
  std::vector<long> shiftedptxt2_once = ptxt2;
  std::vector<long> shiftedptxt2_twice = ptxt2;

  Ctxt temp1 = ctxt2;
  Ctxt temp2 = ctxt2;

  cyclicBitShift(temp1,ea,1);
  cyclicBitShift(temp2,ea,2);

  ea.decrypt(temp1, secret_key, shiftedptxt2_once);
  ea.decrypt(temp2, secret_key, shiftedptxt2_twice);

  std::cout<< "ptxt2 shifted once: \n" << shiftedptxt2_once << std::endl;
  std::cout<< "ptxt2 shifted twice: \n" << shiftedptxt2_twice << std::endl;
*/
  

  
  //cyclicBitShift(ctxt2, ea, 1);
  //ea.decrypt(ctxt2, secret_key, ptxt2);
  //std::cout << "ptxt2*M+constant + 1 cyclicshift = \n"<< ptxt2 << std::endl; 

  //cyclicBitShift(ctxt2, ea, 1);
  cout << endl;
  cout << endl;
  std::cout << "Using keccak" << std::endl;
  //keccak_chi(ctxt2, ea); //#######################################

  ea.decrypt(ctxt2, secret_key, ptxt2);
  std::cout << "ptxt2*M+constant + cyclicshift keccak = \n"<< ptxt2 << std::endl;  


  

  

  //std::cout<<"public key: " <<chiffertemp << std::endl;
  
/*  
  // playing with bitshift
  int j = ptxt2.size();
  std::vector<long> ptxtTemp = ptxt2;

  cout <<"ptxt2 \n" <<ptxt2 << endl;
  

  int modBase = ptxt2.size();
  //In C, you do ((n % M) + M) % M (if u wanna do mod on negative numbers)
  for(int i =0; i<ptxt2.size(); i++)
  {
    int j = (((i-1)%modBase)+modBase)%modBase;
    //cout << j << ": value of j" << endl;
    ptxt2[i] =  ptxtTemp[j];

  }

  //std::cout << "ptxt2 with bitshift? \n " << ptxt2 << std::endl; 

  */



  //##################################################

  /*
  generere matriser,
  X=chi funksjon
  wrappe rundt vectoren riktig for chi funksjonen

  */
 ////lag en "random" matrise, trenger en ny "random" matrise for hver runde
 //std::cout << "random matrisen" << randomMatrix << std::endl; 

  // Generating NTL Matrix From file ######################################################
  //NTL::Mat<NTL::ZZ> matrixFromTxt = readMatrixFromFile("testm.txt");
  //std::cout<< testMatrix << std::endl;
  //std::cout<< "type of testMatrix: " << typeid(testMatrix).name() << std::endl;
  //#############################################################

  //Try do to something with that matrix
  //std::cout << "The matrix from my file \n" << matrixFromTxt << std::endl;
  //std::cout << "Trying to get my matrix into MatMul" << std::endl;


  //MatMulFullExec exec_mat1();
  //MatMulFullExec exec_mat2(matrixFromTxt, bool minimal=false);
  
  //std::cout<<"exec1 \n" <<exec_mat1 << std::endl; 
  //std::cout<<"exec2 \n" <<exec_mat2 << std::endl;
  
  //MatMulFullExec exex_mat(matrixFromTxt,false);
  

  //MatMulFullExec exec_matrix(mat_mul_full_matrix, bool minimal=false);  //this compiles
  //std::cout << "exec_matrix \n" << exec_matrix << std::endl;
  
  //exec_matrix.mul(ctxt1);


  
  return 0;
}

