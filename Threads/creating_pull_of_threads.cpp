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
 *  Description: Thread creation of a pull of threads. 
 *  File Name: creating_pull_of_threads.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y creating_pull_of_threads.cpp -o exe -pthread
 ****************************************************************************
*/
#include <iostream>
#include <thread>   //std threads
#include <vector> //std vector
#include <algorithm> //for_each

void option_lambda(){
    const int N = 2;
    //using array
    std::thread th[N];
    for(size_t i = 0; i < N; i++){
       th[i] = std::thread([]{
            //this is my thread lambda function
            std::cout << "Lambda function-1!!" << std::endl;
            return; 
        });
    }   
    for(size_t i = 0; i < N; i++){
        th[i].join(); //joining the thread 
    }
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread([]{
            //this is my thread lambda function
            std::cout << "Lambda function-2!!" << std::endl;
            return; 
        }));
    }
    for(auto &t : th2)
        t.join();

    //using std vector with different join
    std::vector<std::thread> th3;
    for(size_t i = 0; i < N; i++){
       th3.push_back(std::thread([]{
            //this is my thread lambda function
            std::cout << "Lambda function-3!!" << std::endl;
            return; 
        }));
    }
    std::for_each(th3.begin(),th3.end(), std::mem_fn(&std::thread::join));
}

//Thread class
class myThread{
public:
    //thread class operator
    void operator()(){
        std::cout << "Object function!!" << std::endl;
        return;       
    }
};
 
void option_objects(){
    const int N = 2;
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread((myThread())));
    }
    for(auto &t : th2)
        t.join();
}

//thread function
void f(){
    std::cout << "Function!!" << std::endl;
    return;
}

void option_function(){
    const int N = 2;
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread(f));
    }
    for(auto &t : th2)
        t.join();
}

int main(int argc, char const *argv[]){
    option_lambda();
    option_objects();
    option_function();
    return 0;
}