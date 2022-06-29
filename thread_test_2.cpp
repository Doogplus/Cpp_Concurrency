//
// Created by keliangliu on 22-6-16.
//

#include <iostream>
#include <thread>

struct A {
    void operator()()const { std::cout << 1 << std::endl; }
};

int main() {
    A a;
    std::thread t1{a};
    // std::thread t2(A());
    std::thread t3{A()};
    std::thread t4{[] { std::cout << 2 << std::endl; }};
    std::cout << "I'm main thread" << std::endl;
    t1.join();
    // t2.join();
    t3.join();
    t4.join();

    return 0;
}