# Cpp_Concurrency
## 1. 基本概念
### 1.1 单核处理器和多核处理器
- 多核指的是拥有多个执行单元的 CPU
- 多处理器指的是有两个或者两个以上的 CPU 系统
- 多核有单个 CPU，而多处理器有多个 CPU
### 1.2 并发和并行的区别
- 并发（concurrency）：把任务在不同的时间点交给处理器进行处理。在同一时间点，任务并不会同时运行。
- 并行（parallelism）：把每一个任务分配给每一个处理器独立完成。在同一时间点，任务一定是同时运行。
###   1.3 为何使用并发
- 分离关注点
- 提高性能
## 2. 线程管控
### 2.1 线程启动
不论线程具体要做什么，只要通过C++标准库启动线程，归根结底，代码总会构造std::thread对象：
```
void do_some_work();
std::thread my_thread(do_some_work);
```
任何可调用对象都可用于启动一个线程，常见可调用对象包括普通函数、带有函数调用操作符的类和lambda表达式。
- 普通函数启动线程
```
void f() { std::cout << "Hello concurrency world!"; }
std::thread t(f);
```
- 类
```
class backgroundTask {
    public:
	void operator()() const {
	    do_something();
	    do_something_else();
	}
};

backgroundTask task;
std::thread t(task);
```
> 注：将函数对象传递给std::thread的构造函数时，要防范C++'s most vexing parse(C++最麻烦的解释)。假设传入的是临时变量，而不是具名变量，那么构造函数的语法可能和函数生命相同。避免方法：为临时对象命名，多用一对括号，或者采用同意初始化语法。例如：
```
std::thread my_thread((backgroundTask()));
std::thread my_thread{backgroundTask()};
```
- lambda表达式
```
std::thread my_thread([]{
     do_something();
     do_something_else();
});
```
一旦启动了线程，就需要明确要等待它结束（与之汇合），还是任由它自由运行（与之分离）。
> 注：假定程序不等待线程结束，那么在线程运行结束前，我们需要保证它所访问的外部数据始终正确。
```
struct func {
     int& i;
     func(int& i_) : i(i_) {}
     void operator()() { 
	 for (unsigned j = 0; j < 1000000; ++j) {
	    do_something(i);  // 可能访问悬空引用
	 }
     }
};

void oops() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread my_thread(my_func);  // 新线程可能仍在运行，而主线程的函数却已结束
	my_thread.detach();  // 不等待新线程结束
}
```
### 2.2 等待线程完成
只要调用了join(),隶属于该线程的任何存储空间即会因此清除，std::thread 对象遂不再关联到已结束的线程。对于某个给定的线程，join()仅可调用一次。
### 2.3 在异常情况下等待
```
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
```
### 2.4 在后台运行线程
```
std::thread t(do_something_work);
t.detatch();
asset(!t.joinable());
```
若要把std::thread对象与线程分离，就必须存在与其关联的执行线程！


### 2.5 向线程函数传递参数
- 线程内部具有存储空间，参数会按照默认方式复制到该处新创建的线程才能直接访问他们。这些副本会被当成临时变量，以右值形式传给新线程的函数或可调用对象。
```
void f(int i,  std::string const& s);
std::thread t(f, 3, "hello");
```
>如果参数是指针，并且指向自动变量，这一过程会产生严重影响。如下所示：
```
void f(int i, std::string const& s);
void oops(int some_param) {
        char buffer[1024];
        sprintf(buffer, "%i", some_param);
        std::thread t(f, 3, buffer);
        t.detach();
}
```
因为oops可能已经退出，导致局部数组被销毁而引发未定义的行为。std::thread原样复制所提供的值，并未令其转换为预期的数据类型。解决方法是，在传入std::thread的构造函数之前，就把它转换成std::string对象。
```
void f(int i, std::string const& s);
void not_oops(int some_param) {
        char buffer[1024];
        sprintf(buffer, "%i", some_param);
        std::thread t(f, 3, std::string(buffer));
        t.detach()
}
```
- 传入非const引用参数
```
void udpate_data_for_widget(widget_id w, widget_data& data);
void oops_again(widget_id w) {
  widget_data data;
  std::thread t(update_data_for_widget, w, data);
  display_status();
  t.join();
  process_widget_data(data);
}
```
解决方法：
创建线程改成：
```
std::thread t(updata_data_for_widget, w, std::ref(data));
```

- 将类的成员函数设定为线程函数，应传入一个函数指针，指向该函数成员，还要给出合适的对象指针，作为该函数的第一个参数。
```
class X {
  public:
    void do_lengthy_work();
};

X my_x;
std::thread t(&X::do_lengthy_work, &my_x);
```
- 使用std::move向线程转移动态对象的归属权。
```
void process_big_object(std::unique_ptr<big_object>);
std::unique_ptr<big_object> p(new big_object);
p->prepare_data(42);
std::thread t(process_big_object, std::move(p));
```
