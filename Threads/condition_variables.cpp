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
 *  Description: Implementing condition variable with C++ threads. 
 *  File Name: condition_variables.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y condition_variables.cpp -o exe -pthread
 ****************************************************************************
*/

#include <iostream>
#include <thread>   //std threads
#include <vector> //std vector
#include <algorithm> //for_each
#include <mutex>     //for the mutex
#include <condition_variable>

std::mutex mtx;             
std::condition_variable cv;   
bool ready = false;         
int cur = 0;  
void work(int id){
	std::unique_lock<std::mutex> lck(mtx);
	while(id != cur || !ready){ cv.wait(lck); }
	cur++;
	std::cout << "Thread: " << id << "-- " << cur << std::endl;
	cv.notify_all(); 
}
void run(){
	std::unique_lock<std::mutex> lck(mtx);
	ready = true;
	cv.notify_all();
}
int main(int argc, char const *argv[]){
    const int N = 5;
	std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread(work,i));
    }
	run();
	for(auto &t : th2)
        t.join();
    
    return 0;
}