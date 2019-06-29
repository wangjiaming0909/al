## boost_intrusive 

### hook types
- base_hook
    ```c++
    template <class ...Options>
    class list_base_hook;
    ```

    `options:`
    * *tag\<class Tag\>*
     as a tag of the `base_hook`, you can derive from different `base_hooks` which using different `tags`, so you can put the object into different intrusive containers, if no tag is specified, the default tag will be used
    * *link_mode<link_mode_type LinkMode>*
     linking policy: 3 modes: `normal_link`, `safe_link`(default), `auto_unlink`
    * *void_pointer\<class VoidPointer>* pointer type will be used in the hook(default is `void*`)

- base_hooks
    * list_base_hook
    ```c++ 
    #include <boost/intrusive/list.hpp>
    using namespace boost::intrusive;
    class Foo /*Base hook with default tag, raw pointers and safe_link mode*/
    :  public list_base_hook<>
    { /**/ };

    ```
    * for list we have three options:
        >specify the relationship with the type T
        >> `base_hook<class Hook>`\
        >> `member_hook<class T, class Hook, Hook T::* PtrToMember>`\
        >> `value_traits<class ValueTraits>`

        > Specifies if a constant time size() function is demanded for the container\
        > `constant_time_size<bool Enabled>`

        > Specifies an unsigned type that can hold the size of the container. default(std::size_t)\
        > `size_type<class SizeType>`


    ```c++
    template <class T, class ...Options>
    class list;
    ```

    * list base hook example: recipes tests.h line 318

- member hook
    ```c++
    template <class ...Options>
    class list_member_hook;
    //This class can be configured with the same options as list_base_hook except the option tag:

    class Foo
    {
    public:
    list_member_hook<> hook_;
    //...
    };
    ```

    When member hooks are used, the member_hook option is used to configure the list:
    ```c++
    //This option will configure "list" to use the member hook
    typedef member_hook<Foo, list_member_hook<>, &Foo::hook_> MemberHookOption;

    //This list will use the member hook
    typedef list<Foo, MemberHookOption> FooList;
    Now we can use the container:

    //An object to be inserted in the list
    Foo foo_object;
    FooList list;

    list.push_back(object);

    assert(&list.front() == &foo_object);
    ```

- Using both hooks

### safe hooks
- `link_mode<safe_link>`
    > `safe_link` is the **default** link mode\
    > with safe link mode users can detect if the node is inserted in a container without the reference of container using `is_linked()`\
    > can't invoke `unlink`, only `auto_unlink` mode can unlink
- `link_mdoe<auto_unlink>`
    > you can  use `unlink()`\
    > when the object is detroyed(destructed), the container will auto delete the node\
    > `auto_unlink` is incompatible with `constant_time_size<true>`, cause:
    * `auto_unlink` hooks don't store any reference to the container
    * if you `unlink` one node using node's `unlink()`, you can't modify the size of container because you do not have the reference to the container
    