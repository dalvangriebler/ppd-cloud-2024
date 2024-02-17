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
 *  Description: Thread affinity in C++ threads and machanisms. 
 *  File Name: thread_affinity.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y thread_affinity.cpp -o exe -pthread -lhwloc
 ****************************************************************************
*/
//adapted from https://github.com/eliben/code-for-blog/tree/master/2016/threads-affinity
//https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/

#include <iostream>
#include <thread>   //std threads
#include <vector> //std vector
#include <algorithm> //for_each
#include <chrono>
#include <cstdlib>
#include <sched.h>
#include <hwloc.h>
#include <mutex>
#include <pthread.h>

void report_thread_cpu() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Thread id " << std::this_thread::get_id() << ", running on CPU " << sched_getcpu() << "\n";
}

void hwloc_show(){
    report_thread_cpu();
    // Initialize hwloc.
    hwloc_topology_t topology;
    if (hwloc_topology_init(&topology) < 0) {
        std::cerr << "error calling hwloc_topology_init\n";
        std::exit(1);
    }

    if (hwloc_topology_load(topology) < 0) {
        std::cerr << "error calling hwloc_topology_load\n";
        std::exit(1);
    }

    // Describe the machine's HW topology, on each depth level.
    int topodepth = hwloc_topology_get_depth(topology);
    std::cout << "Depth of topology = " << topodepth << "\n";

    for (int depth = 0; depth < topodepth; depth++) {
        int num_objects = hwloc_get_nbobjs_by_depth(topology, depth);
        std::cout << "Level " << depth << ": " << num_objects << " objects\n";
        for (int i = 0; i < num_objects; i++) {
            hwloc_obj_t obj = hwloc_get_obj_by_depth(topology, depth, i);
            char s[512];
            hwloc_obj_type_snprintf(s, sizeof(s), obj, 1);
            std::cout << " " << i << ": " << s << "\n";
        }
    }

    // Now pin the calling thread to the last PU (logical CPU) on the system.
    int num_objects_last_depth =
    hwloc_get_nbobjs_by_depth(topology, topodepth - 1);
    hwloc_obj_t pu = hwloc_get_obj_by_depth(topology, topodepth - 1, num_objects_last_depth - 1);

    std::cout << "Pinning thread to last PU; OS index = " << pu->os_index << "\n";
    if (hwloc_set_cpubind(topology, pu->cpuset, HWLOC_CPUBIND_THREAD) < 0) {
        std::cerr << "Error calling hwloc_set_cpubind\n";
        std::exit(1);
    }

    report_thread_cpu();
}
void launch_thread_per_cpu(){
    unsigned num_cpus = std::thread::hardware_concurrency();
    std::cout << "Launching " << num_cpus << " threads\n";

    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
    std::vector<std::thread> threads(num_cpus);
    for (unsigned i = 0; i < num_cpus; ++i) {
        threads[i] = std::thread([&iomutex, i] {
        {
            // Use a lexical scope and lock_guard to safely lock the mutex only for
            // the duration of std::cout usage.
            std::lock_guard<std::mutex> iolock(iomutex);
            std::cout << "Thread #" << i << " is running\n";
        }

        // Simulate important work done by the tread by sleeping for a bit...
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        });
    }

    for (auto& t : threads)
        t.join();
}
void launch_threads_report_cpu(){
    constexpr unsigned num_threads = 4;
    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
    std::vector<std::thread> threads(num_threads);
    for (unsigned i = 0; i < num_threads; ++i) {
        threads[i] = std::thread([&iomutex, i] {
            for(size_t a = 0; a < 10; a++){
                {
                // Use a lexical scope and lock_guard to safely lock the mutex only
                // for the duration of std::cout usage.
                std::lock_guard<std::mutex> iolock(iomutex);
                std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
                }

                // Simulate important work done by the tread by sleeping for a bit...
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }
        });
    }
    for (auto& t : threads)
        t.join();
}
void thread_id_native_handle(){
    std::mutex iomutex;
    std::thread t = std::thread([&iomutex] {
        {
            std::lock_guard<std::mutex> iolock(iomutex);
            std::cout << "Thread: my id = " << std::this_thread::get_id() << "\n"
            << "        my pthread id = " << pthread_self() << "\n";
        }
    });
    {
        std::lock_guard<std::mutex> iolock(iomutex);
        std::cout << "Launched t: id = " << t.get_id() << "\n"
        << "            native_handle = " << t.native_handle() << "\n";
    }
    t.join();

}
void set_affinity(){
    constexpr unsigned num_threads = 4;
    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
    std::vector<std::thread> threads(num_threads);
    for (unsigned i = 0; i < num_threads; ++i) {
        threads[i] = std::thread([&iomutex, i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            for(size_t a = 0; a < 10; a++){
                {
                    // Use a lexical scope and lock_guard to safely lock the mutex only
                    // for the duration of std::cout usage.
                    std::lock_guard<std::mutex> iolock(iomutex);
                    std::cout << "Affinity Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
                }

                // Simulate important work done by the tread by sleeping for a bit...
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }
        });
        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
        // only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
    for (auto& t : threads)
        t.join();
}
void my_set_affinity(){
    unsigned int N = std::thread::hardware_concurrency();
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread([i]{
            for(size_t x = 0; x < 2; x++){
                std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }            
            return; 
        }));
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);
        int rc = pthread_setaffinity_np(th2[i].native_handle(), sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
    for(auto &t : th2)
        t.join(); 
}
void show_my_affinity(){
    unsigned int N = std::thread::hardware_concurrency();
    std::vector<std::thread> th2;
    for(size_t i = 0; i < N; i++){
       th2.push_back(std::thread([i]{
            for(size_t x = 0; x < 2; x++){
                std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }            
            return; 
        }));
    }
    for(auto &t : th2)
        t.join(); 
}
int main(int argc, char const *argv[]){
   // hwloc_show();
   // launch_thread_per_cpu();
   // launch_threads_report_cpu();
   // thread_id_native_handle();
   // set_affinity();
    std::cout << "----Showing default affinity----" << std::endl;
    show_my_affinity();
    std::cout << "----Setting my affinity----" << std::endl;
    my_set_affinity();
    return 0;
}