#include <functional>
#include <vector>
#include <numeric>
#include <thread>
// template for the until C++20 accumulate which computes the sum of the value
//init and the elements in the range
template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result=std::accumulate(first, last, result);
    }
};


template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    //std::distance, number of increments needed to go from first to last
    unsigned long const length=std::distance(first,last);

    //return this if the range is empty
    if(!length) { return init;}

    unsigned long const min_per_thread=25;

    unsigned long const max_threads= (length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads = std::thread::hardware_concurrency();

    //if this feature is not supported make it 2, else set it to the hardware number
    //or max_threads whichever is lower
    unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,
                                               max_threads);

    unsigned long const block_size = length/num_threads;

    std::vector<T> results(num_threads);

    std::vector<std::thread> threads(num_threads-1);

    Iterator block_start=first;

    for(unsigned long i=0; i<(num_threads-1); ++i)
    {

        Iterator block_end=block_start;

        //increment iterator by n elements
        std::advance(block_end, block_size);

        threads[i]=std::thread(
            accumulate_block<Iterator, T>(),
            block_start,
            block_end,
            std::ref(results[i]));

        block_start=block_end;
    }

    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads-1]);

    for(auto& entry: threads)
        entry.join();
    return std::accumulate(results.begin(), results.end(), init);
}


