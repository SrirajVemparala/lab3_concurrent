/**
 * @author: Raghu Sai Phani Sriraj Vemparala
 * @file  : sort_thread.cpp
 * @brief : This file contains the functions related to thread creation and initialization is performed.
 * 
 *              
 * Date   : 21 september 2023
*/

#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <barrier>
#include <algorithm>
#include "sort_thread.hpp"
#include "all_locks.hpp"
using namespace std;

//Thread vector pointer
vector<thread*> threads;
//Start time and end time
struct timespec startTime, endTime;
#define NUM_ITERATIONS 10

/*
Note that the barrier requires C++20 (use flag --std=c++2a)  
If you don't have access to C++20 on your local machine,
you'll get a #include <barrier> no such file error on compilation.
Jupyter does have C++20 installed.

Note that you can always use a pthread barrier instead, even in C++.
just #include<pthread> and see the C example for usage.
*/
barrier<> *bar; 
std::vector<std::mutex>* lk;
extern size_t thread_count;

Barrier *sens_bar;
MCS_lock *mcs_cntr_lk;
pet_lock *petersonlockseq;

void init_sens_bar()
{
	sens_bar = new Barrier(thread_count);
}

void init_mcs_lock()
{
	
	mcs_cntr_lk = new MCS_lock;
}

/**
 *this function initializes the barriers and locks
 * 
 */
void global_init(){
	bar = new barrier(thread_count);
	lk  = new std::vector<std::mutex>(thread_count);
}

void init_petson_lock()
{
	petersonlockseq = new pet_lock;
	petersonlockseq->lock_init();
}
/**
 *this function deletes the barriers and locks initialized
 * 
 */
void global_cleanup(){
	delete bar;
	delete lk;
}
/**
 *This thread function provides bucket_sort algorithm funcitonality
 * 
 * @param tid Thread ID
 * @param mssort passes refernce to the ms class object
 */
void* bucket_sort(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							(*lk)[j].lock();
                            buckets[j].push_back(bk_sort.v1[i]);
							(*lk)[j].unlock();
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}
/**
 *This thread function provides bucket_sort algorithm funcitonality
 * 
 * @param tid Thread ID
 * @param mssort passes refernce to the ms class object
 */
void* bucket_sort_tas(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							taslock();
                            buckets[j].push_back(bk_sort.v1[i]);
							tasunlock();
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}

/**
 *This thread function provides bucket_sort algorithm funcitonality
 * 
 * @param tid Thread ID
 * @param mssort passes refernce to the ms class object
 */
void* bucket_sort_ttas(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							ttas_lock();
                            buckets[j].push_back(bk_sort.v1[i]);
							ttas_unlock();
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}

void* bucket_sort_ticket(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							ticket_lock();
                            buckets[j].push_back(bk_sort.v1[i]);
							ticket_unlock();
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}

void* bucket_sort_sense(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	sens_bar->barrier_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							ttas_lock();
                            buckets[j].push_back(bk_sort.v1[i]);
							ttas_unlock();
                            break;
                        }
                }
            }
	sens_bar->barrier_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}

void* bucket_sort_mcs(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	Node *myNode = new Node;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							mcs_cntr_lk->acquire(myNode);
                            buckets[j].push_back(bk_sort.v1[i]);
							mcs_cntr_lk->release(myNode);
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}

void* bucket_sort_petson(int tid, bs& bk_sort, std::vector<vector<int>> &buckets, int bucket_count)
{
	int length = bk_sort.vector_length();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	bar->arrive_and_wait();// Wait for the barrier
	//low index
	int low = (int)((tid) * length) / thread_count;
	// High Index
	int high = (int)((((tid+1)*length) / thread_count) - 1);
	int i = low;
    int j = 0;
	Node *myNode = new Node;
	//Every thread loops its own part of the array
    		for(i = low;i <= high;i++)
    		{
                for(j = 0; j<bucket_count; j++)
                {
                        if(bk_sort.v1[i] > bk_sort.mins[j] && bk_sort.v1[i] <= bk_sort.mins[j+1])
                        {
							//Locking the bucket since only one thread should access the particular bucket
							petersonlockseq->lock(tid);
                            buckets[j].push_back(bk_sort.v1[i]);
							petersonlockseq->unlock(tid);
                            break;
                        }
                }
            }
	bar->arrive_and_wait();
	//Since my bucket_size is equal to number of threads, sorting the array
    std::sort(buckets[tid].begin(), buckets[tid].end());
	return 0;
}
/**
 *this function initializes the threads for the bucket_sort fucntionality.
 * 1 4 2 3 6
 *
 * @param bssort passes refernce to the ms class object
 */
void bucket_thread_init(bs &bk_sort, std::string operation)
{
	int bucket_count = thread_count;
	global_init();

    size_t i;
	std::vector<vector<int>> buckets(bucket_count); //buckets(bucket_count);
	int min_ele = bk_sort.min_element();
    int max_ele = bk_sort.max_element();
    //Creating range based on thread count
    int range = (max_ele-min_ele)/thread_count +1;
	threads.resize(thread_count);
	//Setting minimum count to push the data into each bucket
	bk_sort.find_min_elements(bucket_count, range,max_ele);
	for(i=0; i<thread_count; i++)
	{
        if(operation == "tas")
        {
	 	    threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_tas(i, bk_sort, buckets,bucket_count); });
        }
        else if(operation == "ttas")
        {
            threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_ttas(i, bk_sort, buckets,bucket_count); });
        }
		else if(operation == "ticket")
        {
            threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_ticket(i, bk_sort, buckets,bucket_count); });
        }
		else if(operation == "sense")
		{
			if(i == 0)
			{
				init_sens_bar();
			}
			threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_sense(i, bk_sort, buckets,bucket_count); });
		}
		else if(operation == "mcs")
		{
			if(i == 0)
			{
				init_mcs_lock();
			}
			threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_mcs(i, bk_sort, buckets,bucket_count); });
		}
		else if(operation == "pthread")
		{
			threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort(i, bk_sort, buckets,bucket_count); });
		}
		else if(operation == "petersonseq")
		{
			if(i == 0)
			{
				init_petson_lock();
			}
			threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort_petson(i, bk_sort, buckets,bucket_count);});
		}
		else
		{

		}
	}
    //Creating threads
	// for(i=1; i<thread_count; i++){
	//  	threads[i] = new std::thread([i, &bk_sort, &buckets,bucket_count]() { bucket_sort(i, bk_sort, buckets,bucket_count); });
	// }
	// i = 0;
    // // Main thread calling bucket_sort
	// bucket_sort(i, std::ref(bk_sort), buckets, bucket_count); // master also calls bucket_sort
	// join threads
	for(size_t i=0; i<thread_count; i++)
    {
		threads[i]->join();
		//printf("joined thread %zu\n",i);
		delete threads[i];
	}
	clock_gettime(CLOCK_MONOTONIC,&endTime);
    global_cleanup();
	//Clear the initial vector data
	bk_sort.clear_vector();
    //Storing the data in the result
	bk_sort.write_data(buckets, bucket_count);
	//Calculating the total time taken to run the thread
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);	
}
