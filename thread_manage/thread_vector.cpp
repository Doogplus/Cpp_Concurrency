//
// Created by doolplus on 22-6-29.
//

#include <vector>
#include <thread>
#include <functional>

void do_work(unsigned id) {}

void f() {
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < 20; ++i) {
        threads.emplace_back(do_work,i);
    }
    std::for_each(threads.begin(),threads.end(),
                  std::mem_fn(&std::thread::join));
}

int main() {
    f();
}