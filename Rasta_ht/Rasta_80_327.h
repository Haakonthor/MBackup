
void rasta_327_80(int rounds, vector<long> user_key, vector<vector<long>> constants, vector<MatMulFullExec>& MMFE_matrices, Ctxt& c, const EncryptedArray& ea){
  int i;
  int counter = 0; 
  double chi_time =0; 
  double matmul_time = 0;
  double addConst_time = 0;  
  
  cout << "starting rasta rounds." << endl; 
  for(i = 0; i<rounds; i++){
    cout << "round: " << i << endl; 


    clock_t matmul_timer_start = clock(); 
    MMFE_matrices[i].mul(c);
    clock_t matmul_timer_end = clock();
    matmul_time += (double)(matmul_timer_end-matmul_timer_start)/CLOCKS_PER_SEC;


    cout << "matrix.mul(c) ended" << endl; 

    clock_t adding_const_timer_start = clock();
    addConstants(c, ea, constants[i]);
    clock_t adding_const_timer_end = clock();
    addConst_time += (double)(adding_const_timer_end-adding_const_timer_start)/CLOCKS_PER_SEC; 

    cout << "add constants ended." << endl; 
    

    clock_t chi_timer_start = clock(); 
    keccak_chi(c,ea);  
    clock_t chi_timer_end = clock();
    chi_time +=  (double)(chi_timer_end-chi_timer_start)/CLOCKS_PER_SEC;  


    cout << "keccak chi ended" << endl; 
    counter = counter + 1;
  }
  cout <<  "counter: " <<counter << endl;
  cout << "last round without keccak i:  " << i << endl; 

  clock_t matmul_timer_start = clock();
  MMFE_matrices[counter].mul(c);
  clock_t matmul_timer_end = clock(); 
  matmul_time += (double)(matmul_timer_end-matmul_timer_start)/CLOCKS_PER_SEC;

  cout << "multiplying in C last time" << endl; 

  clock_t adding_const_timer_start = clock();
  addConstants(c, ea, constants[counter]);
  clock_t adding_const_timer_end = clock(); 
  cout << "last  add constant" << endl; 
  addKey(c, ea, user_key);
  cout << "added key again" << endl; 

  //double matmul_time =  (double)(matmul_timer_end-matmul_timer_start)/CLOCKS_PER_SEC; 
  //double addConst_time =  (double)(adding_const_timer_end-adding_const_timer_start)/CLOCKS_PER_SEC;
  cout <<"matMul timer:" << matmul_time << endl;
  cout <<"addingConst timer:" << addConst_time << endl;
  cout <<"chi timer: " << chi_time << endl;  
}