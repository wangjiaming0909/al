### 关于字符串替换的问题
    - 你想替换一个字符串中的某个字串,但是不是所有该字串都要替换 
	- 比如我要替换"123"->"abc", 不是每个"123"都需要替换,特定位置的"123"才需要替换
	- `千万`不能直接对该字符串replace("123", "abc"), 这样就把所有的替换了
**********
### use ssh to check connectivity of a server
$ ssh -q -o BatchMode=yes -o ConnectTimeout=5 root@127.0.0.1 "echo 2>&1" && echo $host SSH_OK || echo $host SSH_NOK SSH_OK

**********