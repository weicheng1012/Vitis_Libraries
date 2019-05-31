/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * usage: ./gemm_test.cpp PATH_TO_XCLBIN/gemx.xclbin PATH_TO_XCLBIN/config_info.dat
 * 
 */

#include <string>
#include "xf_blas.h"

# define IDX2R(i,j,ld) (((i)*( ld ))+(j))
# define m 128 // a - mxk matrix
# define n 128 // b - kxn matrix
# define k 128 // c - mxn matrix

using namespace std;

int main(int argc, char **argv) {
  unsigned int l_argIdx = 1;
  string l_xclbinFile(argv[l_argIdx++]);
  string l_configFile(argv[l_argIdx++]);
  int i, j; // i-row index ,j- column index
  short * a, * b, * c;
  a = ( short *) malloc (m*k* sizeof ( short )); // host memory for a
  b = ( short *) malloc (k*n* sizeof ( short )); 
  c = ( short *) malloc (m*n* sizeof ( short )); 

  for( i = 0; i<  m; i ++){ 
      for( j = 0; j < k; j ++){ 
          a[ IDX2R (i,j,k )]=( short ) 1; 
      } 
  } 

  for( i = 0; i<  k; i ++){ 
      for( j = 0; j < n; j ++){ 
          b[ IDX2R (i,j,k )]=( short ) 1; 
      } 
  } 

    for( i = 0; i<  m; i ++){ 
      for( j = 0; j < n; j ++){ 
          c[ IDX2R (i,j,k )]= 0; 
      } 
  } 

  xfblasEngine_t engineName = XFBLAS_ENGINE_GEMM;
  xfblasStatus_t status = xfblasCreate(const_cast<char*>(l_xclbinFile.c_str()), l_configFile, XFBLAS_ENGINE_GEMM);
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Create Handle failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  status = xfblasMalloc(m,k,sizeof(*a),a,k);
  
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Malloc memory for matrix A failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  
  status = xfblasMalloc(k,n,sizeof(*b),b,n);
  
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Malloc memory for matrix B failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  status = xfblasMalloc(m,n,sizeof(*c),c,n);
  
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Malloc memory for matrix C failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  
  status = xfblasSetMatrix(a);
  status = xfblasSetMatrix(b);
  status = xfblasSetMatrix(c);
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Set Matrix failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  
  status = xfblasSgemm(XFBLAS_OP_N, XFBLAS_OP_N, m, k, n, 1, a, k, b, n, 1, c, n);
  
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Matrix Multiplication failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  
  status = xfblasGetMatrix(c);
  
  if (status != XFBLAS_STATUS_SUCCESS) {
    cout<<"Get Matirx failed with error code: "<< status << "\n"; 
    return EXIT_FAILURE;   
  }
  
  for ( i = 0; i < 10; i ++){
        for ( j = 0; j < 10; j ++){
            cout<< (c[ IDX2R (i,j, k )])<<" ";
        }
        cout<<"\n";
  }
  
  xfblasFree(a);
  xfblasFree(b);
  xfblasFree(c);
  xfblasDestory();
  free(a);
  free(b);
  free(c);
  
  return EXIT_SUCCESS;
}
