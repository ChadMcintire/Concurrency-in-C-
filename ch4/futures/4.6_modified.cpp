#include <future>
#include <iostream>

int find_the_answer_to_ltuae()
{
    int count = 0;
    for (int i = 0; i < 2147483647; i++)
        count += 1;
    return count;
}

void do_other_stuff(){
    int count = 0;
    for (int i = 0; i < 2147483647; i++)
        count += 1;
}

int main() {
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout << "The answer is " << the_answer.get() << std::endl;
}
