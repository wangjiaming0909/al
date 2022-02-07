## SQL模板
### 什么是SQL模板?
SQL模板就是一条SQL语句的骨架, 将其中的`库名`, `表名`, `列名`, `常量`等都替换为`?`之后生成的语句就是这条SQL语句的模板.  

### 为什么需要SQL模板?
当DBScale收到一条SQL语句的时候, 根据我们定义的SQL模板识别规则, 我们将每一条SQL语句都归类到不同的SQL模板中.  
属于相同SQL模板的语句都具有相似的特征, 大概率在DBScale端会生成相同的执行计划, 将SQL语句按照SQL模板分类之后可以比较方便的统计不同类型语句的时间消耗等信息.

### `pt-fingerprint`的SQL模板
`pt-fingerprint`工具是通过正则表达式匹配替换进行SQL模板解析的.`pt-fingerprint`对sql语句做了如下替换。
- 多值insert被替换为单值insert
- 删除注释
- USE 语句的database名字替换， 所有USE语句归为一类
- 替换所有字符串，NULL也被替换，某些identifier中的数字也会被替换
- 删除多余空格
- 整条语句转化为小写
- 替换IN 和VALUES内的所有内容为单一值
- 多个UNION查询整合为一个

### SQL模板是如何创建的?
DBScale首先收到SQL语句解析之后, 生成语句的`stmt_node`以及`record_scan`.  
这两个结构记录了当前语句的语句的metadata, 我们定义`stmt_node`以及`record_scan`的比较函数,并将都符合一定规则的语句判定为某个SQL模板.  
首先我们通过SQL语句类型区分大类, 如`SELECT`, `UPDATE`, `INSERT`等.  
接下来分不同语句类型进行下一级的元数据分析.  
规则细节可以再定义.  
如可以使用以下规则:
- 先根据语句类型分类
- 对于一个`SELECT`语句, 首先根据这个`record_scan`中`join_talbes`个数进行区分.  
- 对是否存在`where`或者`order by`进行再区分.  
    可以对where语句的condition列表内的表达式进行区分.
    如首先对表达式个数进行区分，再对表达式类型进行区分，是否存在子查询等。
- 再根据`select field list`内的Expression进行区分  
- Expression的区分可以根据类型区分, 如StrExpression 和 IntExpression可以归为一类
- 字符串常量等identifier不进行比较，因此无论内容是什么都认为是相同的。