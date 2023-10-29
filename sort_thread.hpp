/**
 * @author: Raghu Sai Phani Sriraj Vemparala
 * @file  : sort_thread.hpp
 * @brief : This file contains the functions declarations related to thread creation and initialization.
 * 
 *              
 * Date   : 21 september 2023
*/
#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include "bucket_sort.hpp"

using namespace std;

void global_init();
void global_cleanup();
void local_init();
void local_cleanup();
void bucket_thread_init(bs &, std::string);




