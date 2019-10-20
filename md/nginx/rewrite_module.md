# rewrite module

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
if | srv, loc, main | ngx_http_rewrite_if
set | srv, sif, loc, lif | ngx_http_rewrite_set
rewrite_log | http, stv, sif, loc, lif | ngx_conf_set_flag_slot
uninitialized_variable_warn | http, srv, sif, loc ,lif | ngx_conf_set_flag_slot

## rewrite module 结构
> ngx_http_rewrite_loc_conf_t
``` c
typedef struct {
    ngx_array_t  *codes;        /* uintptr_t */

    ngx_uint_t    stack_size;

    ngx_flag_t    log;
    ngx_flag_t    uninitialized_variable_warn;
} ngx_http_rewrite_loc_conf_t;
```

## if
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
    return 404;
}
//正则表达式一般也不需要加引号，若含有特殊字符，可加引号
```

其他if可用的选项
```c
//-f: 文件是否存在. -d: 目录是否存在 -e: 目录或者文件是否存在. -x: 文件是否可执行
if (-f $request_filename) {//判断文件是否存在
}
```

## break