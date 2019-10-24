# Http
## 关于http configuration 的初始化
parser 在碰到http块时: 进入到`ngx_http_block`, 此函数是`ngx_http_module`中` http command`的`set`函数.
此函数中:
- 1, 初始化所有http module, 遍历所有http module 分别调用每一个module的`create_main_conf`, `create_crv_conf`, `create_loc_conf`(如果有的话);
- 2, 之后对所有http module 调用`preconfiguration`, 大部分http module 在此函数中都是初始化各自需要的`变量`, 添加到对应conf结构的`hash` 表中;
>注意此处的添加到hash表中只是将变量的`key`添加, 变量的值并不进行计算, 每一个`variable`提供了`get_handler`来获取其值, 只在真正需要访问该变量时,才计算该值.

- 3, 接下来进行解析http块内的具体配置项
- 4, init所有http模块的main_conf, merge配置项
- 5, init locations, create location trees
- 6, 初始化http处理的phases
- 7, init headers in hash
- 8, 调用所有http模块的`post configuration`
- 9, init variables??
