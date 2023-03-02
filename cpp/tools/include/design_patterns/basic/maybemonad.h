#ifndef MAYBEMONAD_H
#define MAYBEMONAD_H

#include <iostream>
#include <string>
#include <iostream>

namespace design_patterns{

struct Address{
    explicit Address(std::string* name) : name(name){}
    std::string *name;
};

struct Person{
    Person(){}
    explicit Person(Address* addr) : addr(addr){}
    Address *addr = nullptr;
};

template <typename T>
class Maybe{
public:
    explicit Maybe(T* const context) : context_(context){}
    template <typename Func>
    auto With(Func&& f)
    {
        using ret_type = typename std::remove_pointer<decltype (std::forward<Func>(f)(context_))>::type;
        if(context_ == nullptr)
        {
            return Maybe<ret_type>{nullptr};
        }
        else
        {
            return Maybe<ret_type>{f(context_)};
        }
    }

    template <typename Func>
    auto Do(Func&& f)
    {
        if(context_ != nullptr)
        {
            f(context_);
        }
        return *this;
    }
private:
    T *context_;
};


void Monad_test()
{
    Person p;
//    std::cout << *(p.addr->name) << std::endl;
    Maybe<Person> maybePerson{&p};
    maybePerson
        .With([](Person* const x){return x->addr;})
            .With([](Address* const x){return x->name;})
            .Do([](std::string* const x){std::cout << x << std::endl;});

    std::string addr_name = "Home";
    Address addr{&addr_name};
    p.addr = &addr;

    maybePerson
        .With([](Person* const x){return x->addr;})
            .With([](Address* const x){return x->name;})
            .Do([](std::string* const x){std::cout << *x << std::endl;});
}

}
#endif // MAYBEMONAD_H
