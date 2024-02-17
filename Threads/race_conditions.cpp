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
 *  Description: Thread creation of a pull of threads with arguments and race conditions. 
 *  File Name: race_conditions.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y race_conditions.cpp -o exe -pthread
 ****************************************************************************
*/

#include <iostream>
#include <thread>   //std threads
#include <vector> //std vector
#include <algorithm> //for_each
#include<mutex>     //for the mutex

int v=0;

std::mutex m;

void option_lambda(){
    const int N = 2;
    std::string data = " | I am a thread inside a lambda function!! |";
    v = 0;    
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
        th2.push_back(std::thread([i,data]{
            //this is my thread lambda function
            std::cout << "ID: " << i << data << " V: " << v << std::endl;
            m.lock();
            v++;
            m.unlock();
            return;
        }));
    }
    for(auto &t : th2)
        t.join();        
}

//Thread class
class myThread{
public:
    int id;
    std::string data;
    myThread(int id, std::string data):id(id),data(data){}
    //thread class operator
    void operator()(){
        std::cout << "ID: " << id << data << " V: " << v << std::endl;
        m.lock();
        v++;
        m.unlock();
        return;        
    }
};
 
void option_objects(){
    const int N = 2;
    std::string data = " | I am a thread inside a object!! |";
    v = 0;
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread((myThread(i,data))));
    }
    for(auto &t : th2)
        t.join();
    std::cout << "Final value is: " << v << std::endl;
}

//thread function
void f(int id, std::string data){
    std::cout << "ID: " << id << data << " V: " << v << std::endl;
    m.lock();
    v++;
    m.unlock();
    return;
}
//Warning: when using poiters, we can have problems with the data.
void option_function(){
    const int N = 2;
    std::string data = " | I am a thread inside a function!! |";
    v = 0;
    //using std vector
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread(f,i,data));
    }
    for(auto &t : th2)
        t.join();
    std::cout << "Final value is: " << v << std::endl;
}

int main(int argc, char const *argv[]){
    option_lambda();
    option_objects();
    option_function();
    return 0;
}


