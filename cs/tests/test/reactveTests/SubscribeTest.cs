using System;
using System.Collections.Generic;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Text;

namespace test.reactveTests
{
    class SubscribeTest
    {
        public static void test()
        {
            ISubject<string> subject = new Subject<string>();
            bool b = true;

            //Observable.Create创建的observable: 其中它的subscribe已经定义好了,只要一subscribe, 所有结果就会得到
            IObservable<string> observable = Observable.Create<string>(
                (ob) =>
                {
                    ob.OnNext("1");
                    ob.OnNext("2");
                    ob.OnNext("3");
                    ob.OnCompleted();//加上这句,对subject调用OnNext不会有任何效果
                    //return 了一个别的subject的subscribe的结果, 等于是对别的subject进行了一次设置而已
                    //但是, 如果在里面写了OnCompleted, 则出去之后,这个新建的Observable就会dispose,所以subject也就dispose了
                    if (b)
                        return subject.Subscribe(Console.WriteLine, () => Console.WriteLine("subject completed"));
                    else
                        return subject.Subscribe(str => Console.WriteLine("b == false " + str));
                }
            );
            //IObservable<bool> ob2 = observable.Any();
            //ob2.Subscribe(str => Console.WriteLine(str + "  ob2"), () => Console.WriteLine("Complted"));

            //IObservable<bool> ob3 = observable.MyAny();
            //ob3.Subscribe(Console.WriteLine, () => Console.WriteLine("completed"));
            //observable.Subscribe((str) => Console.WriteLine(str+ " ob1"), () => Console.WriteLine("completed"));
            //subject.OnNext("1");
            //b = false;
            //observable.Subscribe(Console.WriteLine);
            //subject.OnNext("2");


            //observable.Subscribe((str) => Console.WriteLine(str + " ob1"), () => Console.WriteLine("completed"));
            observable.Dump("observable");
            observable.FirstAsync().Dump("firstAsync");
            //All返回的也是一个IObservable<bool>, 意思是所有的成员都必须满足这个条件
            IObservable<bool> ob4 = observable.All((str) => str.Length == 2);
            //ob4.Subscribe(Console.WriteLine);//当subscirbe的时候,回去检查是否每一个元素都满足这个条件
            ob4.Dump("ob4");

            subject = new Subject<string>();
            try
            {
                subject.FirstAsync().Dump("empty subject first async");
                subject.LastAsync().Dump("empty subject last async");
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
            subject.OnCompleted();

            ISubject<string> subject2 = new Subject<string>();
            //虽然SingleAsync是先Dump的,但是其实真正打印的时候,却是subject2.Dump先打印
            subject2.SingleAsync().Dump("2 element subject2 single Async");
            subject2.Dump("subject2");
            subject2.OnNext("1");
            //subject2.OnNext("2");
            subject2.OnCompleted();
        }
    }
}
