//
// Created by doolplus on 22-6-29.
//

#include <thread>
#include <utility>

class [[maybe_unused]] joining_thread {
    std::thread t;
public:
    joining_thread() noexcept = default;

    template<typename Callable, typename ... Args>
    [[maybe_unused]] explicit joining_thread(Callable&& func, Args&& ... args) : t(std::forward<Callable>(func),
            std::forward<Args>(args)...) {}
    [[maybe_unused]] explicit joining_thread(std::thread t_) noexcept : t(std::move(t_)) {}
    joining_thread(joining_thread&& other) noexcept : t(std::move(other.t)) {}

    joining_thread& operator=(joining_thread&& other) noexcept {
        if (joinable()) { join(); }
        t = std::move(other.t);
        return *this;
    }
    joining_thread& operator=(joining_thread other) noexcept {
        if (joinable()) { join(); }
        t = std::move(other.t);
        return *this;
    }

    ~joining_thread() noexcept {
        if (joinable()) { join(); }
    }

    [[maybe_unused]] void swap(joining_thread& other) noexcept {
        t.swap(other.t);
    }

    [[maybe_unused]] [[nodiscard]] std::thread::id get_id() const noexcept {
        return t.get_id();
    }
    [[nodiscard]] bool joinable() const noexcept {
        return t.joinable();
    }
    void join() { t.join(); }
    void detach() { t.detach(); }
    std::thread& as_thread() noexcept {
        return t;
    }
    [[nodiscard]] const std::thread& as_thread() const noexcept {
        return t;
    }
};

void func() {}

int main() {
    std::thread t(func);
    joining_thread jt(std::move(t));

    return 0;
}