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
 *  Description: Thread creation by lambda function. 
 *  File Name:creating_by_lambda_function.cpp
 *  Version: 1.0 (27/05/2018)
 *  Compilation Command: g++ -std=c++1y creating_by_lambda_function.cpp -o exe -pthread
 ****************************************************************************
*/
#include <iostream>
#include <thread>

int main(int argc, char const *argv[]){
    std::thread th([]{
        //this is my thread lambda function
        std::cout << "I am a thread!!" << std::endl;
        return; 
    }); //creating the thread
    th.join(); //joining the thread    
    return 0;
}