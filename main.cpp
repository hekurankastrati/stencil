//
//  main.cpp
//  cpp-project
//
//  Created by Hekuran Kastrati on 11/30/19.
//  Copyright © 2019 Hekuran Kastrati. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <omp.h>
#include <chrono>

using namespace std;

const int size = 35000;

void stencil_sequential();
void stencil_parallel(int);

int main() {
   
    cout << "Starting timer." << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
//    cout<<"Sequential:"<<endl;
//    stencil_sequential();
    
    cout<<"Parallel:"<<endl;
    stencil_parallel(2);
    
    
    auto t2 = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    
    cout << "Time taken in milliseconds: " << duration << endl;
    
    
    
    return 0;
}


void stencil_sequential(){
    
    float *matrix = new float[size*size];
    
    
    
    for (int i=0; i<size; i++) {
        
        for (int j=0; j<size; j++){
            
            if(i==0 && j==0){
                matrix[0] = 0;
            }
            else if(i==0 && j>0){
                matrix[(i*size) + j] = 250;
            }
            else if(i>0 && j==0){
                matrix[(i*size) + j] = 150;
            }
            else {
                matrix[(i*size) + j] = ( abs( sin( matrix[(i*size) + (j-1)]) )
                                      + abs( sin( matrix[((i-1)*size + (j-1))]) )
                                      + abs( sin( matrix[((i-1)*size) + j]) ) )*100;
            }
        }
    }
    
    
    cout<< "Matrix[50][50]:" << matrix[8168*size+8400]<<endl;
}


void stencil_parallel(int num_threads){
    
    float *matrix = new float[size*size];
    
     for (int i=0; i<size; i++) {
        #pragma omp parallel num_threads(2)
         {
             
             int threadNumber = omp_get_thread_num();
             
             for(int j=i; j<size; j++){
                 
                 
                 
                //If it is first thread,
                // changa values iterating into rows
                if(threadNumber == 0)
                {
                    int row = i;
                    int col = j;
                    
                    if(row == 0 && col == 0){
                        matrix[0] = 0;
                    }
                    else if(row == 0){
                        matrix[(row*size) + col] = 250;
                    }
                    else{
                        matrix[(row*size) + col] = ( abs( sin( matrix[(row*size) + (col-1)]) )
                                            + abs( sin( matrix[((row-1)*size + (col-1))]) )
                                            + abs( sin( matrix[((row-1)*size) + col]) ) )*100;
                    }
                }
                 
                 //If it is second thread,
                 // changa values iterating into columns
                 else if(threadNumber == 1)
                 {
                     int row = j;
                     int col = i;
                     
                     if(row == 0 && col == 0){
                         matrix[0] = 0;
                     }
                     else if(col == 0){
                         matrix[(row*size) + col] = 150;
                     }
                     else{
                     
                         matrix[(row*size) + col] = ( abs( sin( matrix[(row*size) + (col-1)]) )
                                                 + abs( sin( matrix[((row-1)*size + (col-1))]) )
                                                 + abs( sin( matrix[((row-1)*size) + col]) ) )*100;
                     }
                 }
                 
                 
             }
           
         }
        
//         #pragma omp barrier

     }
    
    cout<< "Matrix[50][50]:" << matrix[8168*size+8400]<<endl;
    
}
