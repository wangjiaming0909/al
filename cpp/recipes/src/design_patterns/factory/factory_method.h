#ifndef _DESIGN_PATTERNS_FACTORY_METHOD_H_
#define _DESIGN_PATTERNS_FACTORY_METHOD_H_

namespace design_patterns
{

/*motivation: 
如果 不使用static 方法来提供创建对象的方法，
那么可能是直接构造子类(SelectReactorImpl 或者 PollReactorImpl)
但是可能只是想让客户知道ReactorImpl这个基类,而select和poll是具体实现，并不像暴露

或者ReactorImpl 可以提供一个enum 作为构造函数的参数, 来构造特定类型的ReactorImpl,
但是如果特定类型较多，那么在基类的构造函数中可能会看到一个大的switch case
*/

class ReactorImpl{
protected:
    ReactorImpl(int handle) : handle_(handle){}
    virtual ~ReactorImpl(){}
public:
    static ReactorImpl& makeSelectReactor();
    static ReactorImpl& makePollReactor();
    void destroy()
    {
        delete this;
    }
private:
    int handle_;
};

class SelectReactorImpl : public ReactorImpl{
public:
    SelectReactorImpl(int handle) : ReactorImpl(handle){}
};

class PollReactorImpl : public ReactorImpl{
public:
    PollReactorImpl(int handle) : ReactorImpl(handle){}
};

ReactorImpl& ReactorImpl::makeSelectReactor()
{
    return *(new SelectReactorImpl{1});
}
ReactorImpl& ReactorImpl::makePollReactor()
{
    return *(new PollReactorImpl{2});
}


void test_factory_method()
{
    auto& selectReactorImpl = ReactorImpl::makeSelectReactor();
    auto& pollReactorImpl = ReactorImpl::makePollReactor();

    selectReactorImpl.destroy();
    pollReactorImpl.destroy();
}

}

#endif // _DESIGN_PATTERNS_FACTORY_METHOD_H_


