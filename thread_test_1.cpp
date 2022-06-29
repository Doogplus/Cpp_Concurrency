//
// Created by keliangliu on 22-6-16.
//

#include <iostream>
#include <thread>

void f() { std::cout << "hello c++ thread\n"; }

int main() {
    std::thread t{f};
    std::cout << "I'm main thread" << std::endl;
    t.join();

    return 0;
}