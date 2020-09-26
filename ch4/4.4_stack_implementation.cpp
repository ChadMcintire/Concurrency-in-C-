#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <thread>
template<typename T>
class threadsafe_queue
{
private:
    //std::mutex mut;
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty)
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    //why will this code not work if mutex is not muteable
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    int get_size() const
    {
    std::lock_guard<std::mutex> lk(mut);
    std::cout << data_queue.size();
    return data_queue.size();
    }
};


int main(){
    threadsafe_queue<int> que;
    std::cout << que.empty();
    //push to que in the treads
    std::thread t1(&threadsafe_queue<int>::push, &que, 1);
    std::thread t2(&threadsafe_queue<int>::push, &que, 2);
    std::thread t3(&threadsafe_queue<int>::push, &que, 3);
    std::thread t4(&threadsafe_queue<int>::push, &que, 4);
    std::thread t5(&threadsafe_queue<int>::push, &que, 5);
    std::thread t6(&threadsafe_queue<int>::push, &que, 6);
    std::thread t7(&threadsafe_queue<int>::push, &que, 7);
    std::thread t8(&threadsafe_queue<int>::push, &que, 8);
    
    //make sure the threads end before the program
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    //check size after the joins
    std::cout << que.get_size();

    //wait and pop get the top
    std::cout << *que.wait_and_pop();
    //std::thread twp(&threadsafe_queue<int>::try_pop, &que);
    //twp.join();


    

    //test if try and pop works
    std::cout << *que.try_pop();
    que.empty();
    
    std::thread t9(&threadsafe_queue<int>::get_size, &que);
    t9.join();
    //std::thread t5(&threadsafe_queue<int>::push, &que, 2);
    //std::thread t6(&threadsafe_queue<int>::push, &que, 3);
    //std::cout << *que.try_pop();
}
