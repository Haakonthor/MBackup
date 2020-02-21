//MyMatMulFull

#include <stdio.h>
//#include <helib/matmul.h>
NTL_CLIENT

class MyMatMulFull : public MatMulFull_derived<PA_GF2> {
    PA_INJECT(PA_GF2)
    const EncryptedArray& ea; 
    vector<vector<RX>> data; //RX? //std

public:
    MyMatMulFull(const EncryptedArray& _ea, const char *fname): ea(_ea) {
        long n = ea.size();
        RBak bak; bak.save(); ea.getContext().alMod.restoreContext();
        FILE *fp;
        int i,j, bb;

        data.resize(n);
        for(i = 0; i <n; ++i){
            data[i].resize(n);
        }

        fp= fopen(fname,"r");
        if(fp==NULL) {cout<<"could not find matrix " <<fname<<endl; exit(0);}
        for(i=0;i<n; ++i){
            for(j=0;j<n; ++j){
                fscanf(fp,"%d",&bb);
                data[i][j] = GF2X(bb);
            }
        }
        fclose(fp);
    }
    bool get(RX& out, long i, long j) const override {
        long n = ea.size();

        if(i>= 0 && i<n && j>= 0 && j<n){
            out = data[i][j];
            if(IsZero(data[i][j]))
                return true;
            return false;
        }
        else{
            cout<< "indices (" << i <<","<< j <<") out of range (" << n << ")" << endl;
            exit(0);
        }
    }
    void print(){
        int i, j;
        long n = ea.size();

        for(i=0; i<n; ++i){
            for(j=0; j<n; ++j){
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }
    const EncryptedArray& getEA() const override {return ea;}


};
