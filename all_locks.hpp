#include <iostream>
#include<atomic>
#include <thread>

bool  tas_lock();
void  tasunlock();
void  taslock();
void  ttas_lock();
void  ttas_unlock();
void ticket_lock();
void ticket_unlock();

class Barrier
{
    private:
        std::atomic<int> cnt = 0;
        std::atomic<int> sense = 0;
        std::atomic<int> N = 0;
    public:
        void barrier_wait();
        int thread_count();
        Barrier(int);
};

class Node
{
    public: 
        std::atomic<Node*> next;
        std::atomic<bool> wait;
};

class MCS_lock
{
    std::atomic<Node*> tail = NULL;
    public:
        void acquire(Node*);
        void release(Node*);
};

class pet_lock
{
    std::atomic<bool> desires[2];
    std::atomic<int> turn;
    public:
        void lock_init();
        void lock(int);
        void unlock(int);
};