## bridge pattern
> what problems can the Bridge desigin pattern solve?
- reactor 和 reactorImpl
- 抽象与实现的分离
- 分离编译时的抽象与实现的绑定, 可以在运行时动态的将不同实现与抽象绑定到一起

## decorator pattern
> what problems can the Bridge desigin pattern solve?
- Responsibilities should be added to (and removed from) an object dynamically at run-time.
- A flexible alternative to subclassing for extending functionality should be provided.(Mixin Inheritance in c++)
> function decorator
```c++
typename <typename R, typename... Args>
struct Logger{
    std::function<R(Args...)> func;
    R opertor()(Args... args)
    {
        //decorating codes here....
        R result = func(args...);
        //decorating codes here....
        return result;
    }
}; 
```

> class decorator
```c++
struct Circle
{
    float radius = 10.0f;
    void resize(const float factor) { }
    std::string str() { }
};
template <typename T> struct ColoredShape : T //class decorator
{
    std::string color;
    template <typename... Args>
    ColoredShape(const std::string& clr, Args... args) : T::T(args..), color(clr){ }
    std::string str()
    {//T只需要存在str函数即可,若想限制T的类型,可以添加static_assert(is_base_of(Shape, T)::value, "message")
        return T::str() + " which is colored " + color;
    }
}; 
```
> Actually, class decorator is a decorator of member functions