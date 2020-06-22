
void cyclicBitShift(Ctxt &c ,const EncryptedArray& ea){
    // working cyclic bitshift for a encrypted array of size 2 more then intented slotsize (eg. 329 for 327)
    
    long n = ea.size();
   
    std::vector<long> h1Arr(n,0);
    std::vector<long> h2Arr(n,1); 
    h1Arr[n-2] = 1;
    h2Arr[n-1] = 0;
    h2Arr[n-2] = 0; 
    
    ZZX h1; //obejct polynomial over zz used for encoding the long vectors 
    ZZX h2;
    ea.encode(h1, h1Arr);
    ea.encode(h2, h2Arr);

    //HElibs built in rotate function. 
    ea.rotate(c, 1);
    Ctxt t = c;
    t.multByConstant(h1); //HElib built in multiply constant function

    //Ctxt s = t;
    ea.rotate(t,2);
    c += t;
    c.multByConstant(h2);    
}


void keccak_chi(Ctxt &c, const EncryptedArray& ea){
    // chi function used in rasta 
    
    Ctxt temp1 = c; //create copies of c
    cyclicBitShift(temp1, ea);
    
    Ctxt temp2 = temp1;
    cyclicBitShift(temp2, ea);

    c += temp2;
    temp1 *= temp2;
    c += temp1;
     
}