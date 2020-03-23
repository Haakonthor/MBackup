//matrix file, should be called read matrix or something. 
#include <NTL/ZZ.h>
//include <helib/FHE.h>
//#include <helib/EncryptedArray.h> //might not need these imports in this class
//#include <helib/matmul.h>
//#include <array>
//#include <bitset>
using namespace NTL;

//simple function to read a matrix from a txt file into NTL MAT format
Mat<ZZ> readMatrixFromFile(const char *fname){
    std::ifstream freader;
    int i, j;
    Mat<ZZ> RM; 

    RM.SetDims(16,16);
    freader.open(fname);
    for(i=0; i<16; ++i){
        for(j=0; j<16; ++j){
            freader >> RM[i][j];
        }
    }   
    freader.close();
    return RM; 
}





