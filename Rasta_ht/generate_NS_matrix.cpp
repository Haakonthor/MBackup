//generate_NS_matrix.cpp
#include <iostream>
#include <NTL/mat_GF2.h>
#include <NTL/matrix.h>
#include <NTL/vec_vec_GF2.h>
#include <vector>
#include <fstream>
#include <stdlib.h>

NTL_CLIENT


using namespace std;


void writeMatToFile(vector<vector<long>>& a, const char *fname){
    
    long n = a.size();
    ofstream myfile;
    myfile.open(fname,ios::app);
    int i, j; 

    for(i=0; i<n; i++){
        for(j=0;j<n;j++){
            myfile << a[i][j] << ' ';
        }
        myfile << endl;
        
    }
    myfile << endl; 
    myfile.close();


}
void writeConstToFile(vector<long>& v, const char *fname){
    long n = v.size();
    ofstream myfile;
    myfile.open(fname, ios::app);
    int i;

    for(i=0; i<n; i++){
        myfile << v[i] << ' ';
    }
    myfile << endl;
    myfile.close();
}

void generateConstants(vector<long>& v){ //pseudo random generation of constants
    long n = v.size();
    for(int i=0; i< n-2; i++){
        long r = rand() %2; 
        v[i] = r;
    }

}

void convertGF2_to_vec(mat_GF2& gf2mat, vector<vector<long>>& matrix){
    long n = gf2mat.NumRows();
    //vector<vector<long>> matrix_y(n, vector<long>(n));
    GF2 g;
    long a;

    for (long i = 0; i < n; i++)
    {
        for(long j = 0; j < n; j++)
        {
            g = gf2mat.get(i,j); 
            a = conv<long>(g);
            (matrix[i])[j] = a; 
            
        }
    } 
}


void printVecMat(vector<vector<long>> const &matrix){
    for(const vector<long> &v : matrix){
        for (long x : v)cout << x << ' ';
        cout << endl; 
    }
}

int main(){

    long n = 327;
     
    mat_GF2 x; 
    GF2 d; 
    vector<vector<long>> myVec(n+2,vector<long>(n+2));
    vector<long> myConst(n+2,0);

    //generate 10 string of constant (will only need 5 or 6)
    for(int i = 0; i < 10; i++){
        generateConstants(myConst);
       // writeConstToFile(myConst, "constants329.txt"); //uncomment to recreate new constants in file
    }


    //cout << x << endl; 
    for (int i = 0; i < 20; i++)
    {
        random(x,n,n);
        determinant(d,x);
        //x.SetDims(n+2,n+2);
        /*
        for(long i= 0; i<n-2; i++){
            x.put(i,n-1,0);
            x.put(i,n-2,0);
        } 

        for(long j= 0; j<n; j++){
            x.put(n-1,j,0);
            x.put(n-2,j,0);
        } */

        if(d ==1){
            cout<< "matrix {" << i << "} in nonsingular" << endl; 
            //cout<< x << endl; 
           
            convertGF2_to_vec(x,myVec);
            cout<< "matrix {" << i << "} in convertet form" << endl; 
            //printVecMat(myVec);
            //writeMatToFile(myVec,"matrices329.txt"); //uncomment to rewrite to file
           
        }
     
    }
    return 0; 
}
