#include <mutex>
#include <stdexcept>
#include <climits>

class hierarchical_mutex
{
private:
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    //value is accessible to all mutex instances, but has a different
    //value on each thread
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation();
    void update_hierarchy_value();
public:
     explicit hierarchical_mutex(unsigned long value):
        hierarchy_value(value),
        previous_hierarchy_value(0)
    {}
    void lock();
    void unlock();
    bool try_lock();
}; 
//initialize thread to maximum value, 
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);


