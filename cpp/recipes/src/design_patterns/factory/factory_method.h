#ifndef _DESIGN_PATTERNS_FACTORY_METHOD_H_
#define _DESIGN_PATTERNS_FACTORY_METHOD_H_

namespace design_patterns
{

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


