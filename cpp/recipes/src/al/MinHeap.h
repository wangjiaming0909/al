#ifndef _AL_MINHEAP_H_
#define _AL_MINHEAP_H_

#include <vector>
#include <queue>
#include <memory>

namespace al
{

template <typename T>
class MinHeap
{
public:
    using CompareType = std::greater<T>;
    using ContainerType = std::priority_queue<T, std::vector<T>, CompareType>;
    using SizeType = typename ContainerType::size_type;
    explicit MinHeap() : container_(){}

    static std::shared_ptr<MinHeap> make_min_heap(const std::vector<T>& c)
    {
        std::shared_ptr<MinHeap> target = std::make_shared<MinHeap>();
        target->container_ = c;
        std::make_heap(target->container_.begin(), target->container_.end(), CompareType());
        return target;
    }
public:
    bool empty() const {return container_.empty();}
    SizeType size() const {return container_.size();}
    SizeType max_size() const {return container_.max_size();}
    void clear() {container_.clear();}
    const T& top() const {return container_.top();}
    void push(const T& t) { container_.push(t);}
    template<typename... Args >
    void emplace(Args&&... args)
    {
       container_.emplace(args...);
    }

    void pop(){container_.pop();}
    void reverse() {container_.reverse(container_.size());}
private:
    ContainerType container_;
};

}//al
#endif //_AL_MINHEAP_H_