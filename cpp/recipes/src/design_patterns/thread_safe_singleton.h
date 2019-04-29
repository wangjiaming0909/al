#ifndef _THREAD_SAFE_SINGLETON_H_
#define _THREAD_SAFE_SINGLETON_H_
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
#include <vector>

namespace design_patterns
{
template <typename T>
class thread_safe_singleton{
public:
    static std::shared_ptr<T> getInstance()
    {
//        asm volatile ("" : : : "memory");
        if(!instance_)
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if(!instance_)
            {
                T* temp = new T;
//                asm volatile ("" : : : "memory");
                instance_.reset(temp);
            }
        }
        return instance_;
    }
private:
    static std::mutex              mutex_;
    static std::shared_ptr<T>      instance_;
};

struct A
{
    A() : _(1000){}
    std::string name = "abcd";
    std::vector<std::vector<std::string>> _;
};

template<typename T> std::mutex thread_safe_singleton<T>::mutex_;
template<typename T> std::shared_ptr<T> thread_safe_singleton<T>::instance_;

void thread_routine()
{
    auto tssstr = thread_safe_singleton<A>::getInstance();
    tssstr->name = "123";
    std::cout << std::this_thread::get_id() << " " << (*tssstr).name << std::endl;
}

void thread_safe_singleton_test(){

    for(int i = 0; i < 10000; i++)
    {
        std::thread thread1{thread_routine};
        std::thread thread2{thread_routine};
        std::thread thread3{thread_routine};
        std::thread thread4{thread_routine};
        std::thread thread5{thread_routine};
        std::thread thread6{thread_routine};

        thread1.join();
        thread2.join();
        thread3.join();
        thread4.join();
        thread5.join();
        thread6.join();
    }
}

}

#endif // _THREAD_SAFE_SINGLETON_H_
