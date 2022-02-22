
*********
# Transactions related
## Time, clocks, and the Ordering of Events in Distributed System
一个进程P就是分布式中的一个节点，每个进程内部的事件是可以排序的。  
### The Partial Ordering
定义关系`a ---> b`: `a`, `b`是两个事件  
- 如果`a`, `b`属于同一个进程P，并且a先，b后，那么`a ---> b`.  
- 如果`a`是进程P1中一个消息的发送事件, `b`是另一个进程P2中这个消息的接受事件, 那么`a ---> b`.  
- 如果`a ---> b`, `b ---> c`, 那么`a ---> c`.  
- 如果`a --/-> b`并且`b --/->a`, 那么a,b是并发的.  

### Logical Clocks
定义进程Pi在某个事件的时候会给这个事件分发一个逻辑时间.  
`Ci(a)`,表示Pi进程给事件a分发的逻辑时间.  
`C(b) = Cj(b)`当b是属于Pj的事件时成立.  

- 如果a和b属于同一个进程P, 并且a先,b后,那么Ci(a) < Ci(b);  
- 如果a是Pi的发送事件,b是Pj中这个消息的接收事件,那么Ci(a) < Cj(b);  

也就是如果a,b满足以上两个条件,那么也就满足:  
`a ---> b` 可以推出: `C(a) < C(b)`; 这就是`Clock Condition`  

于是只要能够保证两个事件a,b满足上面的两个条件中的任意一个,就可以保证满足`Clock Condition`.  

如何保证呢?  
- 每个进程给每个事件分发递增的逻辑时间.
- 如果Pi给时间a分发的逻辑时间:Tm=Ci(a), 在Pj给收到消息的事件分发的逻辑事件应大于等于自己分配的值,并且大于Ci(a);  

### Ordering the events totally

********



