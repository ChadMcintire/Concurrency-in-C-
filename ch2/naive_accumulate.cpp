
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
T parallel accumulate(Iterator first, Iterator last, T init)
{

    //std::distance, number of increments needed to go from first to last
    unsigned long const length=std::distance(first,last);
    if(!length) { return init;}
    unsigned long const min_per_thread=25;

}

