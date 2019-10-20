# 1. Introduction of Innodb

## 1. 优点
优点如下：
- a. 如果服务器由于硬件或者软件问题宕机了, 在重启数据库之后, 不需要特殊的操作, InnoDB的`Crash Recovery`会自动的将crash之前已经commited的更改刷入磁盘, 并且撤回未commit的更改. 
- b. InnoDB 存储引擎维护了自己的缓冲池，用内存来缓存被访问的表和索引，频繁被访问的数据将直接从内存中处理，在专门的数据库服务器上，80%的物理内存都可以用来作为缓冲池。
- c. 你可以将相关的数据分散到不同的表中，并使用外键来加强引用一致性，更新或者删除某些数据时，其相关的位于其他表中的数据也会同时被更新或者删除. 尝试向某个表中插入一些外键引用的表中不存在的数据时会失败.
- d. 支持`checksum`, 若磁盘或者内存数据损坏，`checksum`会发出警告
- e. 当你设计数据库时使用了正确的主键，对主键所在列的操作会得到自动的优化，在where, order by, group by, 以及join中使用主键效率更高
- f. 插入，更新，删除操作都会被`change buffering`自动优化, InnoDb 不仅允许并发的对一张表读和写，它同时缓存了更改的数据以便流水线化磁盘I/O。
- g. `Adaptive Hash Index` 可以优化频繁被访问的表
- h. 可以压缩表和索引
- i. 创建和删除索引对性能和可用性影响较小
- j. 截断一个 `file-per-table` 的表格空间是很快的，它不仅释放了InnoDb可用的位于 `System tableSpace`中的空间，同时释放了磁盘空间供OS使用
- k. 使用`DYNAMIC` 行格式 可以使得`BLOB`和长字符的存储布局更高效
- l. 你可以通过查询`INFORMATION_SCHEMA`表来监测存储引擎的内部活动
- m. `Performance Schema`表可以监测存储引擎performance的细节
- n. 你可以在Mysql中混用不同存储引擎的表格文件，甚至是在同一个查询语句中，例如，你可以在一个`JOIN`语句中join两个分别为`InnoDB`和`MEMORY`的表
- o. InnoDB专为高效和大数据量设计
- p. InnoDB可以处理大量的数据，即使某些OS的文件大小限制了2G
## 2. Best practices For InnoDB

- 将频繁使用的列作为主键,

# 2. Innodb and ACID model