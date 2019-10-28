# Http
## 关于http configuration 的初始化
parser 在碰到http块时: 进入到`ngx_http_block`, 此函数是`ngx_http_module`中` http command`的`set`函数.
此函数中:
- 1, 初始化所有http module, 遍历所有http module 分别调用每一个module的`create_main_conf`, `create_crv_conf`, `create_loc_conf`(如果有的话);
- 2, 之后对所有http module 调用`preconfiguration`, 大部分http module 在此函数中都是初始化各自需要的`变量`, 添加到对应conf结构的`hash` 表中;
> 注意此处的添加到hash表中只是将变量的`key`添加, 变量的值并不进行计算, 每一个`variable`提供了`get_handler`来获取其值, 只在真正需要访问该变量时,才计算该值.

> 但其实现在根本不可能初始化, 因为有的变量还没有得到其值, 如http_user_agent, 请求还没有来, 根本不知道agent是啥, 所以只能等到请求到来的时候再获取这个变量的值

- 3, 接下来进行解析http块内的具体配置项
- 4, init所有http模块的main_conf, merge配置项
- 5, init locations, create location trees
- 6, 初始化http处理的phases
- 7, init headers in hash
- 8, 调用所有http模块的`post configuration`
- 9, init variables??


## ngx_http_block
- nginx在解析http块下的其他子配置项时, 会将`ngx_conf_t` -> `ctx` 指向 `ngx_http_conf_ctx_t`. 
- 所有http块下的 `conf_ctx` 在 `ngx_conf_t` 中可以找到
- 此对象是在`ngx_conf_t`的`pool`中创建的, 其包含了所有http模块的三个conf 指针, 因此用指针数组表示
- 在遍历所有`http module`时, 调用`create_x_conf`时创建的所有conf都由这三个指针引用.
- 因此在`http_conf_ctx`中就已经创建了一遍所有http module的三个conf结构
```c
typedef struct {
    void        **main_conf;
    void        **srv_conf;
    void        **loc_conf;
} ngx_http_conf_ctx_t;
```

## ngx_http_core_server
- ngx_http_core_server函数会在解析http块时, 遇到server块时调用
- 此函数开头就创建了一个 `ngx_http_conf_ctx_t`;
- 将`http_block`中初始化的 `conf_ctx`中的`main_conf`指针递给server自己的`conf_ctx`
- 然后开始为自己的 `server_conf` 和 `loc_conf` 数组分配内存
- 之后遍历所有的http module, 分别调用 create_x_conf, 创建好conf, 用上面的数组存下其指针。
- 之后开始解析server块下的配置项
- 最后如果在上一步中没有添加listen, 那么nginx就会添加一个默认的listen, 根据用户id选择端口号, 80或者8000