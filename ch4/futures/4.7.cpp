#include <string>
#include <future>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>


//need to add a lockguard to foo and bar
struct X {
    void foo(int i, const std::string& str) {
        std::cout << str << ' ' << i << '\n';
    }
    std::string bar(std::string const& str){
        std::cout << str << ' ' << '\n';
    }
};


struct Y
{
    double operator()(double trouble){
        return trouble + 10;
    }
};


X baz(X& z){
   z.foo(15, "baz(X&)");
}


class move_only
{
public:
    move_only();
    move_only(move_only&&);
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&);
    move_only& operator=(move_only const&) = delete;
    void operator()();
};


/***
https://en.cppreference.com/w/cpp/thread/async is probably a better
example of this
***/

int main(){
    X x;
    Y y;
    // Calls (&x)->foo(42, "Hello") with default policy:
    // may print "Hello 42" concurrently or defer execution

    //Calls p->foo(42,“hello”) where p is &x
    auto f1 = std::async(&X::foo, &x, 42, "hello");

    //Calls tmpx.bar(“goodbye”) where tmpx is a copy of x
    auto f2=std::async(&X::bar,x, "goodbye");

    //Calls tmpy(3.141) where tmpy is move-constructed from Y()
    auto f3=std::async(Y(), 3.141);
    std::cout << "value of f3 = " << f3.get() << "\n";

    //Calls y(2.718)                  
    auto f4=std::async(std::ref(y), 2.718);
    std::cout << "value of f4 = " << f4.get() << "\n";

    //Calls baz(x)
    std::async(baz, std::ref(x));

    //auto f5 = std::async(move_only());
}
