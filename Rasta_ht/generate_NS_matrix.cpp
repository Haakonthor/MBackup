//generate_NS_matrix.cpp
#include <iostream>
#include <NTL/mat_GF2.h>
#include <NTL/matrix.h>
#include <NTL/vec_vec_GF2.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

NTL_CLIENT


using namespace std;

//function to take a matrix and write it to a txt file
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
//writes the constants to a txt file. 
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
//pseudo random generation of constants 
void generateConstants(vector<long>& v){ //pseudo random generation of constants
    long n = v.size();
    for(int i=0; i< n-2; i++){
        long r = rand() %2; 
        v[i] = r;
    }

}
//converts a NTL GF2 matrix to a vector of vectors. 
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

    double matrix_time = 0; 
    clock_t matrix_gen_timer_start = clock(); 

    vector<vector<long>> myVec(n+2,vector<long>(n+2)); //creates a n+2xn+2 0 matrix
    vector<long> myConst(n+2,0);

    int numOfNSMatrices = 0; 

    //generate 10 vectors of constants (will only need 4 to 6)
    for(int i = 0; i < 5; i++){
        generateConstants(myConst);
       // writeConstToFile(myConst, "constants329.txt"); //uncomment to recreate new constants in file
    }


    //cout << x << endl; 
    //creates matrices and checks if they are non-singular, if they are non-singular they are written to .txt file
    for (int i = 0; i < 100; i++) //set to 100, loop will end when enough non-singular matrices are made. 
    {
        random(x,n,n); //NTL creates a random nxn matrix
        determinant(d,x); //checks the determinant for the matrix
       

        if(d ==1){
            numOfNSMatrices += 1; 
            cout<< "matrix {" << i << "} is nonsingular" << endl; 
            //cout<< x << endl; 
           
            convertGF2_to_vec(x,myVec);
            cout<< "matrix {" << i << "} in convertet form" << endl; 
            //printVecMat(myVec);
            //writeMatToFile(myVec,"matrices329.txt"); //uncomment to rewrite to file
            
           
        }
        if(numOfNSMatrices == 5){ //set to the required amount of matrices. 
            cout << "found 5 NS matrices, breaking out of loop" << endl; 
            break; 
        }
     
    }
    clock_t matrix_gen_timer_end = clock();
    matrix_time = (double)(matrix_gen_timer_end-matrix_gen_timer_start)/CLOCKS_PER_SEC; 
    cout << "matrix timer: " << matrix_time << endl; 
    return 0; 
}
