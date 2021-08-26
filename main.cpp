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

const int size = 20000;

int positions[6] = {0, 0, 0, 0, 0, 0};

void stencil_sequential();
void stencil_parallel(int);
void stencil_parallel_vers_2(int);
void init_matrix();
void display_values(int);

float *matrix;

int main(int argc, char** argv) {
    
    for(int i=1; i< argc; i++){
        
        if(i==7)
            break;
        
        positions[i-1] = stoi(argv[i]);
    }
    
    init_matrix();
    
    cout << "Starting timer." << endl;
    auto t1 = chrono::high_resolution_clock::now();

    
    stencil_sequential();

//    stencil_parallel(8);
    
//    stencil_parallel_vers_2(2);
    
    
    auto t2 = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    
    cout << "Time taken in milliseconds: " << duration << endl;
    
    
    
    return 0;
}


void stencil_sequential(){
    
    cout<<"Sequential:"<<endl;
    for (int i=1; i<size; i++) {
        
        for (int j=1; j<size; j++){
            
                matrix[(i*size) + j] = ( abs( sin( matrix[(i*size) + (j-1)]) )
                                      + abs( sin( matrix[((i-1)*size + (j-1))]) )
                                      + abs( sin( matrix[((i-1)*size) + j]) ) )*100;
        }
    }
    
    
   display_values(1);
}


void stencil_parallel(int num_threads){

    cout<<"Parallel:"<<endl;
    
     for (int i=2; i<2*size; i++) {
         
         
         if(i<size){
             
            #pragma omp parallel for num_threads(num_threads)
             for (int j = 1; j < i; j++)
             {
                 matrix[((i-j)*size) + j] = ( fabs( sin( matrix[(i-j-1)*size + j-1]))
                                               + fabs(sin(matrix[(i-j)*size + j-1]))
                                               + fabs(sin(matrix[(i-j-1)*size + j]))
                                               )*100;
             }
         }
         
         else{
             
             
             #pragma omp parallel for num_threads(num_threads)
             for (int j = size-1; j > i-size; j--)
             {
                 matrix[((i-j)*size) + j] = ( fabs( sin( matrix[(i-j-1)*size + j-1]))
                                             + fabs(sin(matrix[(i-j)*size + j-1]))
                                             + fabs(sin(matrix[(i-j-1)*size + j]))
                                             )*100;
             }
             
         }
         

     }
    
   display_values(num_threads);
    
}



void stencil_parallel_vers_2(int num_threads){
     cout<<"Parallel Second Version:"<<endl;

    for (int i=1; i<size; i++) {
        
        #pragma omp parallel num_threads(num_threads)
        {
            
            int threadNumber = omp_get_thread_num();
            
            for(int j=i; j<size; j++){
                
                int row = 0, col = 0;
                
                //If it is first thread,
                // changa values iterating into rows
                if(threadNumber == 0)
                {
                    row = i;
                    col = j;
                    
                }
                
                //If it is second thread,
                // changa values iterating into columns
                else
                {
                    row = j;
                    col = i;
                    
                }
                
                
                matrix[(row*size) + col] = ( abs( sin( matrix[(row*size) + (col-1)]) )
                                            + abs( sin( matrix[((row-1)*size + (col-1))]) )
                                            + abs( sin( matrix[((row-1)*size) + col]) ) )*100;
                
            }
            
        }
        
        //         #pragma omp barrier
        
    }
    
    display_values(num_threads);
    
}



void init_matrix(){
    
    
    matrix = new float[size*size];
    
    matrix[0] = 0;
    
    for (int i=1; i<size; i++) {
        
        matrix[i] = 250;
        matrix[i*size] = 150;
        
    }
}


void display_values(int num_threads){
    
    cout<< "Number of threads: "<<num_threads<<endl<< "Values: "<<endl;
    
    cout<< "Matrix["<<positions[0]<<"]["<<positions[1]<<"]:" << matrix[positions[0]*size+positions[1]]<<endl;
    cout<< "Matrix["<<positions[2]<<"]["<<positions[3]<<"]:" << matrix[positions[2]*size+positions[3]]<<endl;
    cout<< "Matrix["<<positions[4]<<"]["<<positions[5]<<"]:" << matrix[positions[4]*size+positions[5]]<<endl;
    
}

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

