#include "stack.hpp"

struct empty_stack: std::exception
{
    const char* what() const noexcept;
};

template<typename T>
threadsafe_stack<T>::threadsafe_stack(){}

template<typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack& other)
{
    std::lock_guard<std::mutex> lock(other.m);
    data=other.data;
}

template<typename T>
void threadsafe_stack<T>::push(T new_value)
{
    std::lock_guard<std::mutex> lock(m);
    data.push(std::move(new_value));
}

//pointer can be freely copied without throwing an exception
//downside is memory allocation is expensive for some primitive types
//which is why we have the other pop

//also avoid extra calls to new and delete
template<typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<std::mutex> lock(m);
    //check if stack is empty before popping
    if(data.empty()) throw empty_stack();

    //top() then pop() breaks up operations if data is not 
    //copied safely then it doesn't hurt the data
    //this split can cause race conditions
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
}


//good pop option but not flexible on what types can be used
// because the stored type has to be assignable
template<typename T>
void threadsafe_stack<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(m);
    if(data.empty()) throw empty_stack();
    //top() then pop() breaks up operations if data is not 
    //copied safely then it doesn't hurt the data
    //this split can cause race conditions
    value=data.top();
    data.pop();
}

template<typename T>
bool threadsafe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
}


