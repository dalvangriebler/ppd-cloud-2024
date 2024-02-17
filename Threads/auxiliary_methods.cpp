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
 *  Description: Auxiliary methods for managing the C++ threads. 
 *  File Name: auxiliary_methods.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y auxiliary_methods.cpp -o exe -pthread
 ****************************************************************************
*/

#include <iostream>
#include <thread>   //std threads
#include <vector> //std vector
#include <algorithm> //for_each
#include <chrono>

void threads_sleeping(){
    const int N = 2;
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread([]{
            std::cout << "Sleeping for 200 milliseconds!!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            // create a time point pointing to 10 second in future
            std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now() + std::chrono::seconds(5);
            std::cout << "Sleeping until 5 seconds!!" << std::endl;
            std::this_thread::sleep_until(timePoint);
            return; 
        }));
    }
    for(auto &t : th2)
        t.join(); 
}
void threads_id(){
    const int N = 2;
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread([]{
            std::thread::id threadID = std::this_thread::get_id();
            std::cout << "Thread-ID: " << threadID << std::endl;
            return; 
        }));
    }
    for(auto &t : th2)
        t.join(); 
}
void threads_supported(){
    std::thread th([]{
        unsigned int n = std::thread::hardware_concurrency();
        std::cout << n << " concurrent threads are supported.\n";
        return; 
    }); 
    th.join(); 
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";
}

int main(int argc, char const *argv[]){
    threads_sleeping();
    threads_id();
    threads_supported();
    
    return 0;
}
