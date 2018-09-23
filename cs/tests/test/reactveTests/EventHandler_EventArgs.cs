using System;
using System.Collections.Generic;
using System.Reactive.Linq;
using System.Text;

namespace test.reactveTests
{
    public class EventHandler_EventArgs
    {
        static MyEventArgs args = new MyEventArgs("1", "2", 3);
        //public delegate void EventHandler(object sender, EventArgs e);
        static public event EventHandler EatingShit;// event 封装了一个delegate
        
        //handler
        static public void OnEatingShit(object obj, EventArgs args)//这个是添加的监听事件
        {
            Console.WriteLine(obj.ToString() + " " + args.ToString());
        }

        static public void Handler(MyEventArgs args)//这个就只是调用的包装
        {
            EatingShit?.Invoke("EventHandler handler", args);
        }

        static public void test()
        {
            EatingShit += OnEatingShit;
            Handler(args);
        }




        




        //static public event D event2;
        //public delegate void D(EventArgs args);

        //static protected void OnEventName(EventArgs e)
        //{
        //    EventName?.Invoke("asd", e);
        //}

        //public static void handler(object obj, EventArgs args)
        //{
        //    Console.WriteLine(obj.ToString() + " " + args.ToString());
        //}

        //static public void testEvent()
        //{
        //    EventName += handler;
        //    //EventName("asd", new MyEventArgs("a", "b", 12));
        //    OnEventName(new MyEventArgs("a", "b", 12));
        //}
        //static public void test()
        //{
        //    IObservable<EventArgs> observable = Observable.FromEvent<D, EventArgs>(
        //        (handler) => event2 += handler,
        //        (handler) => event2 -= handler
        //    );
        //    observable.Subscribe((args) => Console.WriteLine(args.ToString()));
        //}
    }
}
