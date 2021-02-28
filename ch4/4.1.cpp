#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <iostream>

bool more_data_to_prepare()
{
    return false;
}

struct data_chunk
{};

data_chunk prepare_data()
{
    return data_chunk();
}

void process(data_chunk&)
{}

bool is_last_chunk(data_chunk&)
{
    return true;
}

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    //put thread into 
    do {
        std::cout << "first";
        data_chunk const data=prepare_data();
        // limit the scope so that you unlock the mutex
        // and then notify the conditional variable
        // that way if the waiting thread wakes immediately, it
        // doesn't then have to block again, waiting for the
        // mutex to unlock
        {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        }
        data_cond.notify_one();
    }
    while(more_data_to_prepare());
}

void data_processing_thread()
{
    while(true)
    {
        std::cout << "second";
        std::unique_lock<std::mutex> lk(mut);

        //use a lambda to check if the queue is not empty be
        //the predicate (in this case our lamda) needs to be a fuction returning
        //bool

        //has to be notified and meet the predicate condition
        data_cond.wait(lk,[]{return !data_queue.empty();});
        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main()
{
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}
