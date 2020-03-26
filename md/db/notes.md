
- git repo url:
    http://gitlab.greatopensource.com/wangjiaming/dbscale-chelianwang


- apt-get source list:
```
    deb http://mirrors.aliyun.com/ubuntu/ xenial main 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial main 
    deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main 
    deb http://mirrors.aliyun.com/ubuntu/ xenial universe 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial universe 
    deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates universe 
    deb http://mirrors.aliyun.com/ubuntu/ xenial-security main 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main 
    deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe 
    deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security universe
  ```

- cmake command:
```
    cmake -DCMAKE_BUILD_TYPE=DEBUG -DDOWNLOAD_DEPS=YES -Wno-dev..
  ```




- set env before run tests:
```
    export LANG=C.UTF-8
    export LC_ALL=C.UTF-8
    export DEBIAN_FRONTEND=noninteractive
    export LD_LIBRARY_PATH=/usr/local/lib
    export MYSQL57_BASEDIR=/mysql/mysql-5.7.29-linux-glibc2.12-x86_64/
    export ZKDIR=/zookeeper-3.4.11/
    export MYSQL_BASEDIR=/mysql/mysql-5.7.29-linux-glibc2.12-x86_64/
```

- greatopensource mail passwd
    8f4bb0cb34c0140a
    
- linux wechat: https://github.com/geeeeeeeeek/electronic-wechat/releases/download/V2.0/linux-x64.tar.gz

- gdb commands:
    thread apply all bt "bt to all threads
    info threads "show all threads
    thread 1 "switching threads
    

- use ssh to check connectivity of a server
```
$ ssh -q -o BatchMode=yes -o ConnectTimeout=5 root@127.0.0.1 "echo 2>&1" && echo $host SSH_OK || echo $host SSH_NOK SSH_OK
```
