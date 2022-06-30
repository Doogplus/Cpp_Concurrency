//
// Created by doolplus on 22-6-30.
//

#include <iostream>
#include <mutex>

class A {
public:
    [[maybe_unused]] static void lock() { std::cout << 1; }

    [[maybe_unused]] static void unlock() { std::cout << 2; }

    [[maybe_unused]] static bool try_lock() {
        std::cout << 3;
        return true;
    }
};

class B {
public:
    [[maybe_unused]] static void lock() { std::cout << 4; }

    [[maybe_unused]] static void unlock() { std::cout << 5; }

    [[maybe_unused]] static bool try_lock() {
        std::cout << 6;
        return true;
    }
};

int main() {
    A a;
    B b;
    {
        std::scoped_lock l(a, b);  // 16
        std::cout << std::endl;
    }  // 23
}