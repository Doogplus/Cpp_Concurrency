//
// Created by keliangliu on 22-6-16.
//

#include <iostream>
#include <thread>

struct Func {
    int& i;
    explicit Func(int& i_) : i(i_) { }
    void operator()()const {
        for (unsigned j = 0; j < 100000; ++j) {
            std::cout << i + j << std::endl;
        }
    }
};

class ThreadGuard {
    std::thread& t;
public:
    explicit ThreadGuard(std::thread& t_) : t(t_) { }
    ~ThreadGuard() {
        if (t.joinable()) {
            std::cout << "Join child thread." << std::endl;
            t.join();
        }
    }

    ThreadGuard(ThreadGuard const&) = delete;
    ThreadGuard& operator=(ThreadGuard const&) = delete;
};

int main() {
    int local_i = 10;
    Func my_func(local_i);
    std::thread t(my_func);
    ThreadGuard tg(t);
    std::cout << "Do something in main thread." << std::endl;

    return 0;
}