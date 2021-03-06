﻿using System;
using System.Collections.Generic;
using System.Reactive.Linq;
using System.Text;

namespace test.reactveTests
{
    public class Customized_Delegate_FromEventPattern
    {
        //public delegate void D(EventArgs args);
        //public event D EatingShit;

        //protected virtual void OnEatingShit(EventArgs e)
        //{
        //    EatingShit?.Invoke(e);
        //}

        //public void Test()
        //{
        //    IObservable<EventArgs> observable = Observable.FromEventPattern<>
        //}
    }

    public class Customized_Delegate
    {
        public delegate void D(EventArgs args);
        static public event D EatingShit;
        static public void OnEatingShit(EventArgs args)
        {
            Console.WriteLine("Eating shit " + args.ToString());
        }
        public static void test()
        {
            EatingShit += OnEatingShit;
            Handler(new MyEventArgs("1", "2", 3));
        }

        public static void Handler(EventArgs args)
        {
            EatingShit?.Invoke(args);
        }

        public static void TestWithFromEvent()
        {
            IObservable<EventArgs> observable = Observable.FromEvent<D, EventArgs>(
                handler => { EatingShit += handler; },
                handler => { EatingShit -= handler; });
            //subscribe之后, 这个事件就被注册了, 当这个事件发生时,就会调用我subscribe的这个OnNext
            observable.Subscribe((args) => Console.WriteLine("subscribe: " + args.ToString()));
            Handler(new MyEventArgs("1", "2", 3));
        }
        
    }
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

        public static void TestWithFromEventPattern()
        {
            //参数与 FromEvent是不一样的, FromEvent只能是delegate的参数就是EventArgs
            //但是这儿, Event
            var ob = Observable.FromEventPattern<EventHandler, EventArgs>(
                handler =>
                {
                    EatingShit += handler;
                },
                handler =>
                {
                    EatingShit -= handler;
                }
            );
            IDisposable disposable =  ob.Subscribe((param) => Console.WriteLine(param.Sender.ToString() + " " + param.EventArgs.ToString()), () => Console.WriteLine("complsted"));
            Handler(new MyEventArgs("1", "2", 3));
            Handler(new MyEventArgs("4", "5", 6));
            disposable.Dispose();
            Handler(new MyEventArgs("7", "8", 9));
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
