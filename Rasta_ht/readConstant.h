//read constant from file

#include <stdio.h>
#include <vector>
#include <fstream>
#include <NTL/ZZ.h>
#include <NTL/GF2X.h>

NTL_CLIENT

std::vector<long> readConstantsFromFile(const EncryptedArray& ea, const char *fname){
    
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


void addConstants(Ctxt &c, const EncryptedArray& ea, std::vector<long> constants){
    long n = ea.size();
    ZZX emask;
    vector<long> pmask(n, 0);
    for(int i=0; i<n; ++i){ 
        pmask[i] = (constants)[i];
    } 
    ea.encode(emask,pmask);
    c.addConstant(emask);
}

void cyclicBitShift(Ctxt &c ,const EncryptedArray& ea){
    long n = ea.size();
    Ctxt t = c
    ea.rotate(c, 1);
    ea.rotate(t, 1);
    
    
    std::vector<long> h1Arr = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
    std::vector<long> h2Arr = {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
    ZZX h1;
    ZZX h2;
    ea.encode(h1, h1Arr);
    ea.encode(h2, h2Arr);

    t *= h1;
    Ctxt s = t;
    ea.rotate(s,2);
    c += s; 
    c *= h2; 
}

void keccak_chi(Ctxt &c, const EncryptedArray& ea){
    Ctxt temp1 = c; 
    cyclicBitShift(temp1, ea);
    
    Ctxt temp2 = temp1;
    cyclicBitShift(temp2, ea);

    //cyclicBitShift(c, ea, 1);
    c += temp2;
    temp1 *= temp2;
    c += temp1;
     
}

/*
void shiftBits(Ctxt &c){
    c >>= 3;

    ////In C++, you do ((n % M) + M) % M (if u wanna do mod on negative numbers) obsolete
}
*/

/*
std::vector<long> cyclic_shift(const std::vector<long> &input, Const EncryptedArray& ea, int shift){
    long n = ea.size();
    std::vector<long> output = input << shift;
    //output ^= (input >>(n - shift));
    return output; 
}
*/


/*
std::vector<ZZ> readConstantsFromFile2(const EncryptedArray& ea,const char *fname){
    long n = ea.size();
    //RBak bak; bak.save(); ea.getContext().alMod.restoreContext();
    FILE *fp;
    int i, bb; 
    std::vector<ZZ> constants;

    //constants.resize(n);
    //for() //might need a loop to resize, but dont think so. 

    fp = fopen(fname,"r");
    if(fp == NULL) {cout<<"could not find constants"<< endl; exit(0);}
    for(i = 0; i<n; ++i){
        fscanf(fp, "%d", &bb);
        constants = GF2X(bb);
    }
    fclose(fp)


}
    */