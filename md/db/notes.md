
- git repo url:
    http://gitlab.greatopensource.com/wangjiaming/dbscale-chelianwang


- apt-get source list:
    deb http://mirrors.aliyun.com/ubuntu/ xenial main deb-src http://mirrors.aliyun.com/ubuntu/ xenial main deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main deb http://mirrors.aliyun.com/ubuntu/ xenial universe deb-src http://mirrors.aliyun.com/ubuntu/ xenial universe deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates universe deb http://mirrors.aliyun.com/ubuntu/ xenial-security main deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security universe

- cmake command:
    cmake -DCMAKE_BUILD_TYPE=DEBUG -DDOWNLOAD_DEPS=YES -Wno-dev..




- set env before run tests:
    export LANG=C.UTF-8
    export LC_ALL=C.UTF-8
    export DEBIAN_FRONTEND=noninteractive
    export LD_LIBRARY_PATH=/usr/local/lib
    export MYSQL57_BASEDIR=/mysql/mysql-5.7.29-linux-glibc2.12-x86_64/
    export ZKDIR=/zookeeper-3.4.11/
    export MYSQL_BASEDIR=/mysql/mysql-5.7.29-linux-glibc2.12-x86_64/

- greatopensource mail passwd
    8f4bb0cb34c0140a
    
- linux wechat: https://github.com/geeeeeeeeek/electronic-wechat/releases/download/V2.0/linux-x64.tar.gz

- gdb commands:
    thread apply all bt "bt to all threads
    info threads "show all threads
    thread 1 "switching threads


- mysql vairiables:
    mysql> show variables like 'gtid%';
    +----------------------------------+-----------+
    | Variable_name                    | Value     |
    +----------------------------------+-----------+
    | gtid_executed                    |           |
    | gtid_executed_compression_period | 1000      |
    | gtid_mode                        | ON        |
    | gtid_next                        | AUTOMATIC |
    | gtid_owned                       |           |
    | gtid_purged                      |           |
    +----------------------------------+-----------+
    6 rows in set (0.00 sec)

    show slave status\G

    mysql> show slave hosts; "to see slaves of current master
    +-----------+------+-------+-----------+--------------------------------------+
    | Server_id | Host | Port  | Master_id | Slave_UUID                           |
    +-----------+------+-------+-----------+--------------------------------------+
    |         3 |      | 13002 |         1 | 0ceeca3f-66a0-11ea-b18c-e454e8b0c44f |
    |         2 |      | 13001 |         1 | 0ce87e43-66a0-11ea-b186-e454e8b0c44f |
    +-----------+------+-------+-----------+--------------------------------------+
    2 rows in set (0.00 sec)

    mysql> show processlist\G "Binlog means slave connections in master
    -----on a master
    *************************** 3. row ***************************
       Id: 4
       User: root
       Host: localhost:48974
       db: NULL
    Command: Binlog Dump
       Time: 16054
      State: Master has sent all binlog to slave; waiting for more updates
       Info: NULL
    *************************** 4. row ***************************
       Id: 5
       User: root
       Host: localhost:48976
       db: NULL
    Command: Binlog Dump
       Time: 16054
      State: Master has sent all binlog to slave; waiting for more updates
       Info: NULL

    ----- on a slave, 
    two threads: 
      io thread read log from master, write to lay log
      sql thread to read lay log and execute sql commands
    *************************** 12. row ***************************
       Id: 23
       User: system user
       Host: 
       db: NULL
    Command: Connect
       Time: 16092
      State: Waiting for master to send event
       Info: NULL
    *************************** 13. row ***************************
       Id: 24
       User: system user
       Host: 
       db: NULL
    Command: Connect
       Time: 16041
      State: Slave has read all relay log; waiting for more updates
       Info: NULL





