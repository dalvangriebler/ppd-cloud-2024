/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 *  Authors: Dalvan Griebler <dalvangriebler@gmail.com>
 *         
 *  Copyright: GNU General Public License
 *  Description: This is a simple parallel matrix multiplication with C++ threads. 
 *  File Name: mm_threads.cpp
 *  Version: 1.1 (23/02/2024)
 *  Compilation Command: g++ -Wall -std=c++1y -O3 mm_threads.cpp -o exe -pthread 
 ****************************************************************************
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include <thread>
#include <tuple>
#include <functional>

//all the matrix
long int **matrix1, **matrix2, **matrix;

void val(int dim){
	long int i, j;
	for(i=0; i<dim; i++){
		for(j=0; j<dim; j++){
			matrix1[i][j] = 4;
			matrix2[i][j] = 5;
			matrix[i][j] = 0;
		}
	}	
}
void printMatrix(long int **matrix, int dim){		
	int i, j;
	for(i=0; i<dim; i++){
		printf("\n");
		for(j=0; j<dim; j++){
			printf("%ld ", matrix[i][j]);
		}
	}
	printf("\n");				
}
void multiply_parallel(int dim, int nw){
	#pragma omp parallel for 
	//schedule(runtime) collapse(2)
	for(long int i=0; i<dim; i++){
		for(long int j=0; j<dim; j++){
			for(long int k=0; k<dim; k++){
				matrix[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}	
}
void multiply(int dim){
	for(long int i=0; i<dim; i++){
		for(long int j=0; j<dim; j++){
			for(long int k=0; k<dim; k++){
				matrix[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}	
}
//main function
int main(int argc, char const *argv[]){

	if(argc == 1) {
        std::cout << "Usage is: " << argv[0] << " dim [matrix dimention] nw [number of workers] " << std::endl;
        return(0);
    }

	const int dim = atoi(argv[1]);
    int nw = atoi(argv[2]); 

	matrix = new long int*[dim];  
	matrix1 = new long int*[dim];
	matrix2 = new long int*[dim];

	for (long int i=0; i < dim; i++){
	    matrix[i] = new long int[dim];
	    matrix1[i] = new long int[dim];
	    matrix2[i] = new long int[dim];
	}
	
	//assigning fixed values to the matrix			
	val(dim);

	auto t_start = std::chrono::high_resolution_clock::now();
	//matrix multiplication algorithm call
	if(nw == 0){
		multiply(dim);
	}else{
		multiply_parallel(dim,nw);
	}
	auto t_end = std::chrono::high_resolution_clock::now();
	std::cout << "Execution time(s): " << std::chrono::duration<double>(t_end-t_start).count() << std::endl;
	
	printf("SIZE= %d\n", dim);
	//printing the resultant matrix (you may comment when bigger sizes will be set-up)
	//printMatrix(matrix,dim);

	delete matrix;
	delete matrix1;
	delete matrix2;
}
