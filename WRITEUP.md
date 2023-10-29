
# Description of code organization:

## There are 6 files for the code development. They are:
</br>1. mysort.cpp</br>
</br>2. bucket_sort.hpp</br>
</br>3. bucket_sort.cpp</br>
</br>4. all_locks.hpp</br>
</br>5. all_locks.cpp</br>
</br>6. sort_thread.cpp</br>
</br>7. sort_thread.hpp</br>
</br>8. counter.hpp</br>
</br>9. counter.cpp</br>
</br>10. counter_main.hpp</br>
</br>11. Makefile</br>

## Contents in each file:

**1. mysort.cpp:** This file has the main function. The following are the fucntionalities.<br>
&nbsp;&nbsp;a. The parsing functionality is implemented.<br>
&nbsp;&nbsp;b. Data is collected from input file and pushed into the array.<br>
&nbsp;&nbsp;c. Output file is created and the final sorted data is pushed into the output file.<br>
&nbsp;&nbsp;d. Bucket sort and merge sort class objects are created.<br>
&nbsp;&nbsp;e. Calls are made to the sorting algorithms.<br>

**2. bucket_sort.hpp:**  This file contains the class declaration and function declarations for the bucket_sort.</br>
&nbsp;The following class and function declarations are present:<br>
&nbsp;&nbsp;a. class name: bs<br>
&nbsp;&nbsp;&nbsp;Public Functions:<br>
&nbsp;&nbsp;b.vector_length(): This function provides length of the vector array.<br>
&nbsp;&nbsp;c.min_element(): This function provides minimum element of the vector array.<br>
&nbsp;&nbsp;d.max_element(): This function provides maximum element of the vector array.<br>
&nbsp;&nbsp;e.find_min_elements(): This function provides minimum length of the elements for each bucket.<br>
&nbsp;&nbsp;f.clear_vector(): This function clears the vector array.<br>
&nbsp;&nbsp;g.write_data():This function writes data of each bucket into the vector array.<br>
&nbsp;&nbsp;h.pushdatatovector(): This function takes the input path of the input file and pushes data into the vector from the file.<br>
&nbsp;&nbsp;i.outputtofile(): This function takes the output file path of the output file and pushes data from the vector into the file.<br>
&nbsp;&nbsp;j.print_vector(): This function takes the prints the vector of the file.<br>
&nbsp;&nbsp;&nbsp;Public variables:<br>
&nbsp;&nbsp;i.v1: Vector array to store the input data of the file.<br>
&nbsp;&nbsp;j.mins: Vector array to store minimum values of each bucket.<br>

**3. bucket_sort.cpp :** This file contains the class function definitions for the bucket_sort file.<br>
&nbsp;The following function definitions are present:<br>
&nbsp;&nbsp;a. class name: bs<br>
&nbsp;&nbsp;&nbsp;Public Functions:<br>
&nbsp;&nbsp;b.vector_length(): This function provides length of the vector array.<br>
&nbsp;&nbsp;c.min_element(): This function provides minimum element of the vector array.<br>
&nbsp;&nbsp;d.max_element(): This function provides maximum element of the vector array.<br>
&nbsp;&nbsp;e.find_min_elements(): This function provides minimum length of the elements for each bucket.<br>
&nbsp;&nbsp;f.clear_vector(): This function clears the vector array.<br>
&nbsp;&nbsp;g.write_data():This function writes data of each bucket into the vector array.<br>
&nbsp;&nbsp;h.pushdatatovector(): This function takes the input path of the input file and pushes data into the vector from the file.<br>
&nbsp;&nbsp;i.outputtofile(): This function takes the output file path of the output file and pushes data from the vector into the file.<br>
&nbsp;&nbsp;j.print_vector(): This function takes the prints the vector of the file.<br>
&nbsp;&nbsp;&nbsp;Public variables:<br>
&nbsp;&nbsp;i.v1: Vector array to store the input data of the file.<br>
&nbsp;&nbsp;j.mins: Vector array to store minimum values of each bucket.<br>

### Functionality of Multithreading Bucket sort:<br>
&nbsp;The Bucket sort algorithm follows the below steps:<br>
&nbsp;&nbsp;a. We start the algorithm by divding the array into sub-arrays and thereby assigning each part of the array to the respective thread.  <br>
&nbsp;&nbsp;Each thread takes its own high and low indexes.<br>
&nbsp;&nbsp;b. Based of the maximum and minimum value of the vector array a range is calculated and the based on the bucket count, bucket_count+1 length of min values are considered.<br>
&nbsp;&nbsp;c. By default the the thread count is equal to the bucket count.<br>
&nbsp;&nbsp;d. Based on the number of threads the total locks are considered. If there are 4 threads that means there are 4 locks. Each lock gaurds each bucket.<br>
&nbsp;&nbsp;e. Once the element is read and it falls in the min range of that bucket. It is pushed into that bucket.<br>
&nbsp;&nbsp;f. Once all the buckets are filled then they are sorted and stored into the vector.<br>

**4. all_locks.hpp**: This file contains the class declaration and function declarations for the all_locks file.<br> 
&nbsp;&nbsp;The following class and function declarations are present:<br>
&nbsp;&nbsp;a. class name: Barrier<br>
&nbsp;&nbsp;&nbsp;Public Functions:<br>
&nbsp;&nbsp;b. barrier_wait(): Used for waiting barrier<br>
&nbsp;&nbsp;c. thread_count(): Used for obtaining thread count<br>
&nbsp;&nbsp;a. class name: MCS_lock<br>
&nbsp;&nbsp;&nbsp;Public Functions:<br>
&nbsp;&nbsp;b. acquire(): Used for accquiring mcs lock<br>
&nbsp;&nbsp;c. release(): Used for releasing mcs lock<br>
&nbsp;&nbsp;a. class name: pet_lock<br>
&nbsp;&nbsp;&nbsp;Public Functions:<br>
&nbsp;&nbsp;b. lock_init(): Initialize tpeterson lock.<br>
&nbsp;&nbsp;c. lock(): Used for peterson locking the file<br>
&nbsp;&nbsp;c. unlock(): Used for peterson unlocking the file<br>

**5. all_locks.cpp:** This file contains the class function definitions for the all_locks.hpp file.<br>
&nbsp;The following function defintions are present: <br>
&nbsp;&nbsp;a. tas_lock()<br>
&nbsp;&nbsp;b. tasunlock()<br>
&nbsp;&nbsp;c. taslock()<br>
&nbsp;&nbsp;d. tasunlock()<br>
&nbsp;&nbsp;e. ttas_lock()<br>
&nbsp;&nbsp;f. ttas_unlock()<br>
&nbsp;&nbsp;g. ticket_lock()<br>
&nbsp;&nbsp;h. ticket_unlock()<br>
&nbsp;&nbsp;i. barrier_wait()<br>
&nbsp;&nbsp;j. thread_count()<br>
&nbsp;&nbsp;k. acquire()<br>
&nbsp;&nbsp;l. release()<br>
&nbsp;&nbsp;m. lock_init()<br>
&nbsp;&nbsp;n. lock()<br>
&nbsp;&nbsp;o. unlock()<br>

### Functionality in all_locks.cpp:<br>
&nbsp;The all_locks has the below functionalities impleemnted:<br>
&nbsp;&nbsp;a. Each lock use SEQ_CST memory order<br>
&nbsp;&nbsp;b. Each lock uses multiple mechanisms to attain the goal<br>

**6. sort_thread.cpp:** This file contains function definitons of thread intialization and bucket_sort algorithm.<br>
&nbsp;&nbsp;The following class and function definitions are present:<br>
&nbsp;&nbsp;a. global_init() - Initializes barries and mutexes.<br>
&nbsp;&nbsp;b. global_cleanup() -Deletes the barrier and the mutex.<br>
&nbsp;&nbsp;c. thread_sort() -this thread function calls merge sort function.<br>
&nbsp;&nbsp;d. bucket_sort() - this thread function provides bucket_sort algorithm funcitonalityt. Each lock has its own name added to the name bucketsort For example bucketsort_lock()<br>
&nbsp;&nbsp;e. sort_thread_init() - this function initializes the threads for merge sort functionality.<br>
&nbsp;&nbsp;f. bucket_thread_init() - this function initializes the threads for the bucket_sort fucntionality.<br>
&nbsp;&nbsp;g. final_merge() - this function merges the final list of sorted arrays in merge sort.<br>

**7. sort_thread.hpp:** The following operations can be performed using the MakeFile.<br>
&nbsp;&nbsp;a. global_init()<br>
&nbsp;&nbsp;b. global_cleanup()<br>
&nbsp;&nbsp;c. thread_sort()<br>
&nbsp;&nbsp;d. bucket_sort()<br>
&nbsp;&nbsp;e. sort_thread_init()<br>
&nbsp;&nbsp;f. bucket_thread_init()<br>
&nbsp;&nbsp;g. final_merge()<br>

**8. counter.hpp:** The following operations can be performed using the MakeFile.<br>
&nbsp;&nbsp;a. tas_func_init()<br>

**9. counter.cpp:** This file contains function definitons of thread intialization and bucket_sort algorithm.<br>
&nbsp;&nbsp;The following class and function definitions are present:<br>
&nbsp;&nbsp;a. initilization functions - Initialization functions are used for intialising the barriers and locks.<br>
&nbsp;&nbsp;b. global_cleanup() -Deletes the barrier and the mutex.<br>
&nbsp;&nbsp;c. tas_thread() -this thread function implements tas locks.<br>
&nbsp;&nbsp;d. ttas_thread() - this thread function implements ttas locks.<br>
&nbsp;&nbsp;e. ticket_thread() - this thread function implements counter increment ticket locks.<br>
&nbsp;&nbsp;f. sense_rev_barrier() - this thread function implements sense reversal barrier counter increment functionality.<br>
&nbsp;&nbsp;g. normal_bar() - this thread function implements ususal barrier counter increment function.<br>
&nbsp;&nbsp;g. mcs_lock() - this thread function implements mcs counter increment function.<br>
&nbsp;&nbsp;g. petson_lock() - this thread function implements peterson sequence counter increment function.<br>
&nbsp;&nbsp;g. normal_lock() - this thread function implements mutex counter increment function.<br>
**8. Makefile:** The following operations can be performed using the MakeFile.<br>
&nbsp;&nbsp;a. make clean - clean the -o files and bin files.<br>
&nbsp;&nbsp;b. make - Generates a executable with name mysort.<br>

# Description of Algorithms and Challenges Overcome:

* tas lock:
    * This code implements a basic lock using the Test-and-Set (TAS) operation. It allows threads to acquire the lock and enter a critical section while ensuring that only one thread can access the critical section at a time. If a thread attempts to acquire the lock but it's already held by another thread, it will spin in the taslock() function until it can successfully acquire the lock, at which point it proceeds to execute the critical section
* ttas_lock:
    * The TTAS algorithm is a lock that combines testing and setting the lock state with an additional test operation (load) to check the lock's current state. It loops until it successfully acquires the lock and allows a thread to enter the critical section. When a thread unlocks, it immediately makes the lock available for other threads to acquire. 
    * By using additional load cache_miss is reduced.
* ticket lock:
    * The ticket lock implementation ensures that threads enter the critical section in a strictly ordered manner, based on their ticket numbers. Threads that request access to the critical section are given a unique ticket, and they spin-wait until their ticket number matches the one currently being served. When a thread exits the critical section, it increments the now_serving counter to allow the next thread in line to enter. This approach provides fairness in access to the critical section and avoids issues like thread starvation.
* Sense Reversal Barrier:
    *  The code implements a sense-reversing barrier where threads alternate their sense and use an atomic counter to track how many threads have arrived. Once all threads have arrived, the barrier is released, and the threads can continue. This type of barrier is typically used to synchronize a group of threads to a common point in their execution before allowing them to proceed.
* MCS_lock:
    * The MCS lock algorithm is designed to provide efficient and fair locking for multiple threads. Threads are organized into a queue, and the lock is handed off from one thread to the next. Threads waiting for the lock are not spinning but are instead in a more efficient state until it's their turn to acquire the lock. This makes MCS locks suitable for situations where contention for the lock is expected, as it minimizes the overhead of busy-waiting.
* Peterson lock:
    * The Peterson lock algorithm ensures that only one of the two threads can be in the critical section at a time. The use of the desires array and the turn variable helps coordinate the two threads, allowing them to take turns in acquiring the lock. This algorithm is simple and effective for providing mutual exclusion between two threads.

# Challenges
    Development of MCS lock is complex to implement. It took more time to implement.

# Performance

* Runtime Analysis:
    1.  It can be observed that MCS lock took more time to run because it is FIFO lock system and this affects the performance since the cache line is avialable one lock after the other.<br>
* Cache hits:
    1.  In Bucket sort algorithm TTAS has better hit rate because these algorithms implement LIFO based system which has good hit rate.<br>
* Branch Predication Hit rate:
    1. Branch prediction rate is how well a processor predicts the outcome of conditional branch instructions (e.g., if-else statements and loops) in the program's code. MCS has good branch prediction rate as a synchronization mechanism, is designed to provide exclusion for multiple threads efficiently.<br>


### Performance Table

### Counter Performance

### Counter Performance  

| Primitive       | Runtime(s) | L1 cache hit (%) |Branch Pred Hit Rate (%)|Page Fault
|-----------------|:----------:|-----------------:|:-----------------------|:---------
| TAS Lock        |  0.673     |       96.79      |       97.72            |    146
| TTAS Lock       |  0.594293  |       95.30      |       98.19            |    150
| MCS Lock        |  1.515565  |       99.11      |       99.87            |    152
| Peterson Lock   |  0.226164  |       99.56      |       98.20            |    142
|pthread Lock     |  0.177671  |       98.15      |       99.21            |    149
| sense barrier   |  0.000277  |       93.84      |       97.42            |    152
| Ticket Lock     |  0.096503  |       98.15      |       98.23            |    152

### Bucket sort performance
| Primitive       | Runtime(s) | L1 cache hit (%) |Branch Pred Hit Rate (%)|Page Fault
|-----------------|:----------:|-----------------:|:-----------------------|:---------
| TAS Lock        |  0.129     |       98.73      |       96.6             |    2664
| TTAS Lock       |  0.124     |       99.20      |       98.62            |    2599
| MCS Lock        |  0.259292  |       99.51      |       99.56            |    2727
| Peterson Lock   | 0.096155   |       99.02      |       99.48            |    2617
| pthread Lock    |  0.039     |       99.05      |       96.05            |    2710
| sense barrier   |  0.11      |       99.25      |       95.66            |    2703
| Ticket Lock     |  1.001612  |       99.06      |       99.11            |    2570

## Extant Bugs:
&nbsp;No bugs were observed during the tests and autograde.sh was successful.<br>

