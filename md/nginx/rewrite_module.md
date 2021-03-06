# rewrite module
rewrite模块为http的一个模块

## 初始化流程
#### 在解析配置文件时, 读到server 块之后, 会首先初始化所有http模块, 因此也会初始化rewrite模块
#### 调用rewrite模块的create_main_conf, create_srv_conf, create_loc_conf, 但其实rewrite模块只有create_loc_conf, rewrite 也没有preconfiguration, 因此可能rewrite也没有什么变量需要初始化. 
#### rewrite有merge location conf的函数


## rewrite module 的指令
- rewrite
- return
- break
- if
- set
- rewrite_log
- uninitialized_variable_warn

directive | 可出现的块 | 解析配置遇到该指令时的响应函数
- | - | -
rewrite | srv, sif, loc, lif | ngx_http_rewrite
return | srv, sif, loc, lif | ngx_http_rewrite_return
break | srv, sif, loc, lif | ngx_http_rewrite_break
if | srv, loc | ngx_http_rewrite_if
set | srv, sif, loc, lif | ngx_http_rewrite_set
rewrite_log | http, stv, sif, loc, lif | ngx_conf_set_flag_slot
uninitialized_variable_warn | http, srv, sif, loc ,lif | ngx_conf_set_flag_slot

## rewrite module loc_conf结构体
> ngx_http_rewrite_loc_conf_t
``` c
typedef struct {
    ngx_array_t  *codes;        /* uintptr_t */

    ngx_uint_t    stack_size;

    ngx_flag_t    log;
    ngx_flag_t    uninitialized_variable_warn;
} ngx_http_rewrite_loc_conf_t;
```

## if指令
if 可以用 "=", "!=" 进行字符串比较：
```c
if ($request_method = POST){
    return 405;
}
//此处的POST不需要双引号
```
也可以使用正则表达式比较，使用符号："~", "~*", "!~", "!~*";

符号 | 含义
-|-|-
~ | 大小写敏感
~* | 大小写不敏感
!~ | 大小写敏感，但是取非
!~* | 大小写不敏感，但是取非
例如：
```c
if ($http_user_agent ~ MSIE) {
    //只要user_agent 包含MSIE 就会返回404
    return 404;//return 指令见  return
}
//正则表达式一般也不需要加引号，若含有特殊字符，可加引号
```

其他if可用的选项
```c
//-f: 是否是一个文件. -d: 是否是一个目录 -e: 是否是一个文件或者目录. -x: 文件是否可执行
//!-f, !-d, !-e, !-x: 表示取反
if (-f $request_filename) {
}
```
以上几个if条件的响应函数都为`ngx_http_script_file_code`, 此函数会在rewrite phase被调用

### code 分析
```c
//if command
    ngx_command_t 
    { ngx_string("if"),
      NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_BLOCK|NGX_CONF_1MORE,
      ngx_http_rewrite_if,//解析配置文件时, 读取到if时调用的响应函数
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL 
    },
```
#### ngx_http_rewrite_if
```c
ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));//首先创建ctx
if (ctx == NULL) {
    return NGX_CONF_ERROR;
}

pctx = cf->ctx;//cf是全局的conf
ctx->main_conf = pctx->main_conf;
ctx->srv_conf = pctx->srv_conf;

ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
if (ctx->loc_conf == NULL) {
    return NGX_CONF_ERROR;
}

for (i = 0; cf->cycle->modules[i]; i++) {//遍历所有的http module, 创建 location conf
    if (cf->cycle->modules[i]->type != NGX_HTTP_MODULE) {
        continue;
    }

    module = cf->cycle->modules[i]->ctx;

    if (module->create_loc_conf) {

        mconf = module->create_loc_conf(cf);
        if (mconf == NULL) {
            return NGX_CONF_ERROR;
        }

        ctx->loc_conf[cf->cycle->modules[i]->ctx_index] = mconf;
    }
}
```
- 此函数首先创建一个ctx, if只能放在server,或者location块中, 因此ctx首先将main和server conf指向全局conf的ctx中对应的conf, 也就是http块下的 main_conf, server_conf
- 接下来分配自己loc conf的内存, 遍历所有http module, 创建loc conf, 塞到自己的loc conf数组中
- 问题： 为什么rewrite if需要创建一个location块呢？？

## break
break将下一步的code设置为: ngx_http_script_exit, 其实就是NULL, 表示当前rewrite phase结束, 即使当前块后还有别的if 等rewrite指令也不会再被执行到
example:
```c
server{
//...
//break;//如果再server块下直接有一个break,那么就等于没有rewrite阶段,直接进如下一个http处理阶段
if ($request_method ~ GET) {
    break;
    return 404 "find nothing!\n";//不会被执行
}
if ($request_method = GET) {//此if也不会被执行, 因为rewrite phase已经结束了
    return 404;
}
//...
}
```

## return
1, 可以返回状态码和一条message

- return 404 "$reqeust_filename Not found";

2, 也可以返回一个url, status是302, 组装一个302的response包发给客户端

- return http://192.168.0.2

nginx 在处理时首先解析第二个参数, 尝试转换成整数, 
- 失败就开始判断是否是以`http` 或者`https` 或者`schema`开头的, 尝试组装一个302的`response`包, 
- 如果成功转换成整数,就用给定的`status code`和`message`组装一个`response`包


## rewrite