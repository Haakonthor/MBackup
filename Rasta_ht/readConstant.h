//read constant from file
//also includes cyclicbitshift and chi functions. 
#include <stdio.h>
#include <vector>
#include <fstream>
#include <NTL/ZZ.h>
#include <NTL/GF2X.h>

NTL_CLIENT


std::vector<long> readConstantsFromFile(const EncryptedArray& ea, const char *fname){
    /*
    * reads in a single vector of constants from file, obsulete for our use. 
    */
    
    long n = ea.size();
    FILE *fp;
    int i, bb;
    vector<long> constants(n,0);

    fp = fopen(fname,"r");
    if(fp == NULL) {cout<<"could not find constants" << endl; exit(0);}

    for(i=0; i<n; ++i){
        fscanf(fp, "%d", &bb);
        constants[i] = bb;
    }
    fclose(fp);
    
    return constants; 
}
 //test method to read severa lines form a file

std::vector<std::vector<long>> readConstantsFromFile_all(const EncryptedArray& ea, const char*fname){
    /*
    * reads in multiple lines of constants and return them as a matrix of constants. 
    */
    long n = ea.size();
    FILE *fp;
    int i,j, bb;
    vector<long> tempData(n,0);
    vector<vector<long>> constants;
    constants.resize(n);
    for(i=0; i<n; ++i){
        constants[i].resize(n);
    }


    fp = fopen(fname,"r");
    if(fp == NULL) {cout<<"could not find constants" << endl; exit(0);}
    for(j=0; j<10; ++j){
        for(i=0; i<n; ++i){
            
            fscanf(fp, "%d", &bb);
            tempData[i] = bb;
        }
        constants[j] = tempData;
    }
    fclose(fp);
    
    return constants; 
}


void addConstants(Ctxt &c, const EncryptedArray& ea, std::vector<long> constants){
    /*
    * adding a full array worth of constants to a ciphertext object.
    */
    long n = ea.size();
    ZZX emask;
    vector<long> pmask(n, 0);
    for(int i=0; i<n; ++i){ 
        pmask[i] = (constants)[i];
    } 
    ea.encode(emask,pmask);
    c.addConstant(emask); //using HElibs built in function 
}

void addKey(Ctxt &c, const EncryptedArray& ea, std::vector<long> key){
    /*
    * adding a full array worth of constants to a ciphertext object. the same operation as addconstants just for the key. 
    */
    long n = ea.size();
    ZZX emask;
    vector<long> pmask(n, 0);
    for(int i=0; i<n; ++i){ 
        pmask[i] = (key)[i];
    } 
    ea.encode(emask,pmask); 
    c.addConstant(emask);
}




