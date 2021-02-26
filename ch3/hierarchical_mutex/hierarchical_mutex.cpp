#include "hierarchical_mutex.hpp"

//void hierarchical_mutexi::lock()
//{
//    check_for_hierarchy_violation();
//    internal_mutex.lock();
//    update_hierarchy_value();
//}

void hierarchical_mutex::check_for_hierarchy_violation()
{
    if(this_thread_hierarchy_value <= hierarchy_value)
    {
        throw std::logic_error("mutex hierarchy violated");
    }
}

void hierarchical_mutex::update_hierarchy_value()
{
    previous_hierarchy_value=this_thread_hierarchy_value;
    this_thread_hierarchy_value=hierarchy_value;
}

void hierarchical_mutex::lock()
{
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
}

void hierarchical_mutex::unlock()
{
    this_thread_hierarchy_value=previous_hierarchy_value;
    internal_mutex.unlock();
}

bool hierarchical_mutex::try_lock()
{
    check_for_hierarchy_violation();
    if(!internal_mutex.try_lock())
        return false;
    update_hierarchy_value();
    return true;
}

