#ifndef _THREAD_SAFE_SINGLETON_H_
#define _THREAD_SAFE_SINGLETON_H_
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

namespace design_patterns
{
//it's not thread safe actually
template <typename T>
class thread_safe_singleton{
public:
    static std::shared_ptr<T> getInstance()
    {
        //开不开启内存屏障 对x86处理器并没有影响, 一样是线程不安全的
        // asm volatile ("" : : : "memory");
        if(!instance_)
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if(!instance_)
            {
                T* temp = new T;
                // asm volatile ("" : : : "memory");
                instance_.reset(temp);

                // instance_.reset(new T);
            }
        }
        return instance_;
    }
private:
    static std::mutex              mutex_;
    static std::shared_ptr<T>      instance_;
};

template<typename T> std::mutex thread_safe_singleton<T>::mutex_;
template<typename T> std::shared_ptr<T> thread_safe_singleton<T>::instance_;

template <typename T>
class ThreadSafe_Singleton_WithoutLock
{
public:
    static std::shared_ptr<T> getInstance()
    {
        static std::shared_ptr<T> instance = std::make_shared<T>();
        return instance;
    }
};

struct A
{
    A() : _(1000){
        count++;
    }
    std::string name = "abcd";
    std::vector<std::vector<std::string>> _;
    static int count;
};

int A::count = 0;


void thread_routine()
{
    auto tssstr = thread_safe_singleton<A>::getInstance();
    std::this_thread::sleep_for(10ms);
    tssstr->name = "123";
    std::cout << std::this_thread::get_id() << " " << (*tssstr).name << std::endl;
}

//thread safe version
template <typename T>
class SingletonWithCallOnce{
public:
    static std::shared_ptr<T> getInstance()
    {
        if(!instance_)
        {
            std::call_once(instance_init_flag_, &init);
        }
        return instance_;
    }

private:
    static void init()
    {
        instance_.reset(new T);
    }
private:
    static std::once_flag instance_init_flag_;
    static std::shared_ptr<T> instance_;
};

template <typename T>
std::once_flag SingletonWithCallOnce<T>::instance_init_flag_;
template <typename T>
std::shared_ptr<T> SingletonWithCallOnce<T>::instance_;

void thread_routine2()
{
    auto swithCallOnce = SingletonWithCallOnce<A>::getInstance();
    std::this_thread::sleep_for(10ms);
    std::cout << std::this_thread::get_id() << " " << (*swithCallOnce).name << std::endl;
}

template <typename T>
class SingletonUsingVolatile{
public:
    static T* getInstance()
    {
        if(!isInitialized_)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!isInitialized_)
            {
                //isInitialized 必须在instance_已经初始化之后
                //考虑这样一种情况：
                /*
                    但前线程已经将 isInitialized 初始化为true，然而，isntance_还没有初始化，
                    即便加了 volatile 也只是限制 isInitialized, 并不能影响到在代码上确实存在先后顺序的两句
                                                                        isInitialized_ = true;
                                                                        instance_ = new T;
                    另一个线程读到 isInitialized 已经初始化了，因此就得到了一个错误的 instance_
                */
                // isInitialized_ = true;
                instance_ = new T;
                isInitialized_ = true;
            }
        }
        return instance_;
    }
private:
    static T* instance_;
    static std::mutex mutex_;
    static volatile bool isInitialized_;
};

template <typename T> T* SingletonUsingVolatile<T>::instance_ = nullptr;
template <typename T> std::mutex SingletonUsingVolatile<T>::mutex_; 
template <typename T> volatile bool SingletonUsingVolatile<T>::isInitialized_ = false;

void thread_routine3()
{
    auto swithCallOnce = SingletonUsingVolatile<A>::getInstance();
    std::this_thread::sleep_for(10ms);
    std::cout << std::this_thread::get_id() << " " << (*swithCallOnce).name << std::endl;
}

void thread_routine4()
{
    auto sWithoutLock = ThreadSafe_Singleton_WithoutLock<A>::getInstance();
    std::this_thread::sleep_for(10ms);
    std::cout << std::this_thread::get_id() << " " << (*sWithoutLock).count << std::endl;
}

template <typename Func> 
void run_threads(Func f)
{
    std::thread thread1{f};
    std::thread thread2{f};
    std::thread thread3{f};
    std::thread thread4{f};
    std::thread thread5{f};
    std::thread thread6{f};

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
}

void thread_safe_singleton_test(){
    // run_threads(thread_routine);         // not thread safe
    // run_threads(thread_routine2);           //safe
    // run_threads(thread_routine3);           //safe
    run_threads(thread_routine4);
}

}

#endif // _THREAD_SAFE_SINGLETON_H_
