## 浮动IP设计方案


### **实现可挂载式执行浮动IP的绑定以及解绑定**
在dbscale内部通过popen函数调用当前目录下的bind_float_ip.sh或者unbind_float_ip.sh脚本执行具体的命令。并且两个脚本接受float_ip参数，在该节点上对浮动IP进行操作。
由于绑定或者解绑定浮动IP需要使用root权限执行，因此需要以root启动dbscale集群。


### **实现对绑定/解绑定浮动IP的语法解析器**
当主dbscale节点在datasource选主流程过程中，需要对旧mysql主节点进行解绑浮动IP对新的mysql主节点绑定浮动IP，此时需要通过命令通知对应的dbscale节点执行相关动作。
语法规则如下：
DBSCALE INTERNAL SET FLOAT_IP datasource_name opt_num
其中datasource_name是指定的单一mysql复制组，opt_num为0或1，0表示解绑定，1表示绑定


### **哪个dbscale应该执行该脚本?**
**一种方案是: 由master server所在机器的dbscale 节点去执行绑定脚本**.  
`The script`: 用于执行绑定或者解绑浮动IP的脚本，定义接口，由DBScale调用.  
`那么首先需要确定应该由那个DBScale去执行脚本:`  
当DBScale内部选出主之后, 可以确定那个`DataServer`是作为主的, 但是在集群中那个DBScale和这个DataServer在一个机器上是未知的.  
因此我们在配置中新增了一个选项, 在`data-server`下新增了选项`related-dbscale-node`.

**另一种方案是: 配置ssh登录方式进行浮动ip的绑定**  
第一个方案下,如若某个dbscle节点出现故障,那么集群将无法去该机器上解绑或者绑定ip.  
采用ssh登录的方式只需要一个DBScale节点活着就可以执行该操作.
此种方案不需要多dbscale之间进行通信, 不需要知道哪个dbscale和当前这个`master`server在同一台机器上.  


### **如何以及何时通知dbscale执行脚本?**
monitor线程和消息线程都在master dbscale,因此,在master dbscale monitor线程发现master server 已经down之后,需要首先unbind该datasource的浮动ip,  
此时对外已没有浮动ip暴露, 在`master dbscale`选出主之后, 再进行浮动ip绑定, 但是需要确定是哪一个dbscale所在的机器去绑定这个浮动ip.  
在确定谁去绑定之后, 给该dbscale发一个绑定的命令(需要实现), 进行绑定.若绑定失败, 则可以选择重试或者重新检查连通性甚至重新选主.

### **解绑失败或者绑定失败处理**
如果解绑失败,首先需要检查解绑失败原因, 可能的情况是当前机器上并没有绑定, 那么可能需要探测该浮动ip是否可达, 若可达则还需要去对应机器上进行解绑.  
绑定之前需要确定当前局域网内是否已经存在该浮动ip的绑定者, 若有则去对应机器解绑, 并到新主进行绑定操作, 如果绑定失败,则可以尝试重试, 重试依然失败,则可以尝试解绑并且重新选主.


### **可能存在的问题**
- 由于现在业务直连后端mysql, 因此dbscale无法控制客户端的连接, 在开启半同步的情况下, 若半同步slave个数不够,则可能会导致业务语句卡主,等待`semi sync ack`.  
当然可以延迟绑定 float ip的时间, 延迟到半同步server个数符合半同步规则之后.  
- dbscale 执行脚本需要root权限, 因此dbscale需要使用root启动.
- 数据源高可用方案可能存在未知问题,需测试验证.

![float_ip_1](http://172.16.70.172/codes/al/md/pic/float_ip1.png)
![float_ip_2](http://172.16.70.172/codes/al/md/pic/float_ip2.png)
![float_ip_3](http://172.16.70.172/codes/al/md/pic/float_ip3.png)

