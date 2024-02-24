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
 *  Description: This is a simple MPI program to show if the process are 
 * 	running on remote hosts
 *  File Name: mpi_show_hostname.cpp
 *  Version: 1.0 (02/06/2018)
 *  Compilation Command: mpic++ -std=c++1y -Wall mpi_show_hostname.cpp -o exe
 *	Exacution Command: mpirun -np 4 ./exe
*/

#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

char *getCommandResult(const char *cmd){
	if (cmd == NULL) return NULL;
	FILE *pipe = popen(cmd, "r");
	char *data = (char *) malloc(sizeof(char) * 2566);
	char *bf = (char *) malloc(sizeof(char) * 256);
	if (pipe) {
		while (fgets(bf, 256, pipe) != NULL) strcat(data,bf);
		pclose(pipe);
	}else{
		perror("popen");
		pclose(pipe);
		return NULL;
	}
	free(bf);
	return data;
}
int main(int argc, char **argv){
	int myrank, //who am i
	numprocs; //how many process

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	char buff[256];
	sprintf(buff,"hostname && gcc --version");
	char *dat = getCommandResult("hostname && gcc --version");
	std::cout << "myrank: " << myrank << " of " << numprocs << " -> " << dat << std::endl;

	MPI_Finalize();
	return 0;
}
