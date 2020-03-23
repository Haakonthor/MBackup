#include <iostream>
#include <stdio.h>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/matmul.h>
#include <NTL/BasicThreadPool.h>
#include "MyMatMulFull.h"
#include "readConstant.h"
#include "Non_singular_matrix_generation.h"



//take a plaintext

void Rasta(int rounds, vector<long> key){

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
  std::vector<long> ptxt1(nslots); //key?
  // Set it with numbers 0..nslots - 1
  for (int i = 0; i < nslots; ++i) {
    ptxt1[i] = i%2; //i%2 
  }
  ptxt1[nslots-2] = 0; 

  // Create a ciphertext
  Ctxt ctxt1(public_key);

  // Encrypt the plaintext using the public_key
  ea.encrypt(ctxt1, public_key, ptxt1);

  // Create a plaintext for decryption
  std::vector<long> decrypted(nslots);

  MyMatMulFull my_MMF_matrix(ea, "matrices329.txt"); //read in and create a matmullfull object
  MatMulFullExec MMFE_matrix(my_MMF_matrix, false); //create the matmulFullExec object

  MMFE_matrix.mul(ctxt1); //multiply with ciphertext

  vector<long> constants = readConstantsFromFile(ea,"constants329.txt"); //read in constants

  addConstants(ctxt1, ea, constants); //add the constants to the object

    //do the bitshift for keccak
  Ctxt temp1 = ctxt1;
  cyclicBitShift(temp1,ea);
  Ctxt temp2 = temp1;
  cyclicBitShift(temp2,ea);

  keccak_chi(ctxt1, ea);

  addKey(ctxt1, ea, key);

  ea.decrypt(ctxt1, secret_key, ptxt1);
  cout << "end decrypted: " << ptxt1 << endl; 

}


int main(int argc, char *argv[]){
    vector<long> Key(329,1);
    Key[328] = 0; 
    Key[327] = 0; 

    Rasta(1,Key); 



    return 0;
}