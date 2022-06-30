//
// Created by doolplus on 22-6-30.
//

#include <mutex>

class A {
public:
    void f() {}
};

class B {
public:
    A* get_data() {
        std::lock_guard<std::mutex> l(m_);
        return &data_;
    }

private:
    std::mutex m_;
    A data_;
};

int main() {
    B b;
    A* p = b.get_data();
    p->f();  // 未锁定 mutex 的情况下访问数据
}