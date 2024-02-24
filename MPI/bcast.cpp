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
 *  Description: This is a simple example of MPI_Bcast communication
 *  File Name: bcast.cpp
 *  Version: 1.0 (02/06/2018)
 *  Compilation Command: mpic++ -std=c++1y -Wall bcast.cpp -o exe
 *	Exacution Command: mpirun -np 4 ./exe
*/
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv []){
    int myrank, //who am i
    numprocs; //how many process
    const int ROOT=0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    int data = 111 * myrank;
    std::cout << "Before: process " << myrank << " has data " << data << std::endl;
    MPI_Bcast(&data, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    std::cout << "After: process " << myrank << " has data " << data << std::endl;
    MPI_Finalize();
    return 0;
}