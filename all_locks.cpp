#include "all_locks.hpp"
#define SEQ_CST std::memory_order_seq_cst
#define RELAXED std::memory_order_relaxed
std::atomic<bool> x(false);
std::atomic<int> next_num = 0;
std::atomic<int> now_serving = 0;

//extern size_t thread_count;

bool tas_lock()
{
    bool expected = false;
    bool out = x.compare_exchange_strong(expected,true,SEQ_CST);
    return out;
}

void tasunlock()
{
    x.store(false,SEQ_CST);
}

void taslock()
{
    while(tas_lock() == false){};
}

void ttas_lock()
{
    bool expected = false;
    while((x.load(RELAXED))||!(x.compare_exchange_strong(expected,true,SEQ_CST,SEQ_CST))){expected = false;}
}

void ttas_unlock()
{
    x.store(false,SEQ_CST);
}

void ticket_lock()
{
    int my_num = atomic_fetch_add_explicit(&next_num, 1,SEQ_CST);
    while(now_serving.load(SEQ_CST)!=my_num){}
}

void ticket_unlock()
{
    atomic_fetch_add_explicit(&now_serving, 1, SEQ_CST);
}

Barrier::Barrier(int thread_count)
{
    N = thread_count;
}

void Barrier::barrier_wait()
{
        static thread_local bool my_sense = 0;
        //printf("TC:%d\n\r",thread_count());
        if(my_sense == 0){my_sense = 1;}
        else{my_sense = 0;}
        int cnt_cpy = atomic_fetch_add_explicit(&cnt, 1, SEQ_CST);
        //printf("cntcpy:%d\n\r",cnt_cpy);
        if(cnt_cpy == N -1)
        {
            cnt.store(0, RELAXED);
            sense.store(my_sense, SEQ_CST);
        }
        else
        {
            while(sense.load(SEQ_CST)!= my_sense){}
        }
}

int Barrier::thread_count()
{
    return N;
}

void MCS_lock::acquire(Node* myNode)
{
    Node* oldTail = tail.load(SEQ_CST);
    myNode->next.store(NULL,RELAXED);
    while(!tail.compare_exchange_strong(oldTail,myNode,SEQ_CST))
    {
        oldTail = tail.load(SEQ_CST);   
    }
    if(oldTail!=NULL)
    {
        myNode->wait.store(true,RELAXED);
        oldTail->next.store(myNode,SEQ_CST);
        while (myNode->wait.load(SEQ_CST)) {}
    }
}

void MCS_lock::release(Node* myNode)
{
    Node* m = myNode;
    if(tail.compare_exchange_strong(m, NULL, SEQ_CST)) {
         // no one is waiting, and we just 
        // freed the lock
    } 
    else
    {
        // hand lock to next waiting thread
        while(myNode->next.load(SEQ_CST)==NULL){}
        myNode->next.load(SEQ_CST)->wait.store(false, SEQ_CST);
    }
}

void pet_lock::lock_init()
{
    desires[0].store(false,SEQ_CST);
    desires[1].store(false,SEQ_CST); 
    turn.store(0,SEQ_CST);
}

void pet_lock::lock(int tid)
{
    desires[tid].store(true,SEQ_CST);
    turn.store(1-tid,SEQ_CST);
    while(desires[1-tid].load(SEQ_CST) && turn.load(SEQ_CST)==(1-tid)){}
}

void pet_lock::unlock(int tid)
{
    desires[tid].store(false,SEQ_CST); 
}
