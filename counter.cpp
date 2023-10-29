#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <barrier>
#include <algorithm>
#include "all_locks.hpp"
using namespace std;

//Thread vector pointer
vector<thread*> counter_threads;
//Start time and end time
struct timespec startTime, endTime;

barrier<> *bar; 
std::mutex* lk_counter;
int cntr = 0;
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

void global_init()
{
	bar = new barrier(thread_count);
	lk_counter  = new mutex();
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
void global_cleanup()
{
	delete bar;
	delete lk_counter;
}

void* tas_thread(int tid, int num_iterations)
{
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i<num_iterations; i++)
    {
		taslock();
		cntr++;
		tasunlock();
	}
	return 0;
}

void* ttas_thread(int tid, int num_iterations)
{
	bar->arrive_and_wait();
	if(tid==0)
    {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}

	for(int i = 0; i<num_iterations; i++)
    {
		ttas_lock();
		cntr++;
		ttas_unlock();
	}
	bar->arrive_and_wait();
	
	return 0;
}

void* ticket_thread(int tid, int num_iterations)
{
	bar->arrive_and_wait();
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	for(int i = 0;i< num_iterations;i++)
	{
		ticket_lock();
		cntr++;
		ticket_unlock();
	}
	bar->arrive_and_wait();
	return 0;
}

void* sense_rev_barrier(int tid,int num_iterations)
{
	sens_bar->barrier_wait();
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	sens_bar->barrier_wait();
	for(int j = 0;j <thread_count;j++)
	{
		if(j == tid)
		{
			for(int i = 0;i< num_iterations;i++)
			{
				cntr++;
			}	
		}
		sens_bar->barrier_wait();
	}
	// bar->arrive_and_wait();
	return 0;
}

void* normal_bar(int tid,int num_iterations)
{
	bar->arrive_and_wait();
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	bar->arrive_and_wait();
	for(int j = 0;j <thread_count;j++)
	{
		if(j == tid)
		{
			for(int i = 0;i< num_iterations;i++)
			{
				cntr++;
			}	
		}
		bar->arrive_and_wait();
	}
	// bar->arrive_and_wait();
	return 0;
}

void* mcs_lock(int tid, int num_iterations)
{
	bar->arrive_and_wait();
	//printf("Entered MCS_lock\n");
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	Node *myNode = new Node;
	for(int i = 0;i< num_iterations;i++)
	{
		mcs_cntr_lk->acquire(myNode);
		cntr++;
		mcs_cntr_lk->release(myNode);
	}
	delete myNode;
	bar->arrive_and_wait();
	return 0;
}

void* petson_lock(int tid, int num_iterations)
{
	bar->arrive_and_wait();
	//printf("Entered MCS_lock\n");
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	for(int i = 0;i< num_iterations;i++)
	{
		petersonlockseq->lock(tid);
		cntr++;
		petersonlockseq->unlock(tid);
	}
	bar->arrive_and_wait();
	return 0;
}

void* normal_lock(int tid, int num_iterations)
{
	bar->arrive_and_wait();
	//printf("Entered MCS_lock\n");
	if(tid==0)
	{
		clock_gettime(CLOCK_MONOTONIC, &startTime);
	}
	for(int i = 0;i< num_iterations;i++)
	{
		lk_counter->lock();
		cntr++;
		lk_counter->unlock();
		
	}
	bar->arrive_and_wait();
	return 0;
}

int tas_func_init(int thread_count,int num_iterations, string operation, int bar_flag)
{
	int i = 0;
	global_init();
	counter_threads.resize(thread_count);
	//Spawning the threads
	for(i=0; i<thread_count; i++)
	{
        if(operation == "tas")
        {
	 	    counter_threads[i] = new thread(tas_thread,i,num_iterations);
        }
        else if(operation == "ttas")
        {
            counter_threads[i] = new thread(ttas_thread,i,num_iterations);
        }
		else if(operation == "ticket")
        {
            counter_threads[i] = new thread(ticket_thread,i,num_iterations);
        }
		else if(operation == "sense")
		{
			if(i == 0)
			{
				init_sens_bar();
			}
			counter_threads[i] = new thread(sense_rev_barrier,i,num_iterations);
		}
		else if(operation == "mcs")
		{
			if(i == 0)
			{
				init_mcs_lock();
			}
			counter_threads[i] = new thread(mcs_lock,i,num_iterations);
		}
		else if(operation == "pthread" && bar_flag !=1)
		{
			counter_threads[i] = new thread(normal_lock,i,num_iterations);
		}
		else if(operation == "petersonseq")
		{
			if(i == 0)
			{
				init_petson_lock();
			}
			counter_threads[i] = new thread(petson_lock,i,num_iterations);
		}
		else if(operation == "pthread" && bar_flag ==1)
		{
			counter_threads[i] = new thread(normal_bar,i,num_iterations);
		}
		else
		{
			return 0;
		}
	}
	for(size_t i=1; i<thread_count; i++)
    {
		counter_threads[i]->join();
		//printf("joined thread %zu\n",i);
		delete counter_threads[i];
	}
	clock_gettime(CLOCK_MONOTONIC,&endTime);
    global_cleanup();
	//Calculating the total time taken to run the thread
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);	
	return cntr;
}
