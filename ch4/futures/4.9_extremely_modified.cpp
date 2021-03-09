#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <functional>
#include <utility>
#include <cmath>
#include <iostream>
#include <tuple>

using pt = std::packaged_task<int(int, int)>;

class Packaged_Tasks_Example{
private:
    std::mutex m;
    std::deque< std::tuple<pt, int, int> > tasks;
    bool shutdown_message = false;
    std::future<int> current;

public:
    void shutdown(){
        shutdown_message = true;
    }

    bool gui_shutdown_message_received(){
        if (shutdown_message == true){
            return false; 
        }
        else{
            return false; 
        }
    }

    void gui_thread()
    {
        std::cout << "does this happen somehow?";
        //while(!gui_shutdown_message_received())
        {
            pt task;
            //std::future<int> res;
            {
                std::lock_guard<std::mutex> lk(m);
                if(tasks.empty())
                {

                }
                    //continue;
                //how do you get this to move with a move only and 2 ints 
                //const auto& [task1, f1, f2] = std::move(tasks.front());
                pt task1 = std::move(std::get<0>(tasks.front()));
                int f1 = std::get<1>(tasks.front());
                int f2 = std::get<2>(tasks.front());
                //std::future<int> res=task1.get_future();
                task1(f1 , f2);

                //res = task1.get_future();
                //std::cout << "task_thread:\t" << res.get() << '\n';
                tasks.pop_front();
            }
            std::cout << "is this the correct future " << current.get() << "\n";
            //std::cout << "f1 and f2 = " << f1 << " " << f2;
            
        }
    }

    template<typename Func>
    //std::future<int> post_task_for_gui_thread(
    void post_task_for_gui_thread(
        Func a)
        //std::function<int (int, int)> a)
    {
        pt task(a);
        //std::future<int> res=task.get_future();
        current = std::move(task.get_future());
        std::lock_guard<std::mutex> lk(m);
        std::tuple<pt, int, int> tup(std::move(task), 2, 10); 
        //tasks.push_back(std::move(task), 2, 10);
        //need to use a move because the tuple contains the packaged task
        //which is a move only object
        tasks.push_back(std::move(tup));
        //std::thread task_td(std::move(task), 2, 10);
        //task_td.join();

        //std::cout << "task_thread:\t" << res.get() << '\n';

        //return res;
    }
};

int f(int x, int y) { return std::pow(x,y); }

int main(){
    Packaged_Tasks_Example pte;
    //std::future<int> res = pte.post_task_for_gui_thread(f);
    pte.post_task_for_gui_thread(f);

    pte.gui_thread();
    
}
