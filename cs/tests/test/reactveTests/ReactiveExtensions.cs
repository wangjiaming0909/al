using System;
using System.Collections.Generic;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Text;

namespace test.reactveTests
{
    static class ReactiveExtensions
    {

        //Any返回的是一个IObservable<bool>
        static public IObservable<bool> MyAny<T>(this IObservable<T> observable)
        {
            return Observable.Create<bool>(
                observer =>
                {
                    var hasValues = false;
                    return observable.Take(1).Subscribe(
                        _ => hasValues = true,// 只要有任何一个OnNext, 则将需要返回的值这是为true
                        observer.OnError,//除非出现了异常
                        () =>{//observable中其他的任何OnNext都可以继续执行,但是这儿直到它OnCompleted的时候才会OnNext一下,返回一个true,然后conplete
                            observer.OnNext(hasValues);
                            observer.OnCompleted();
                        });
                }
            );
        }

        public static void Dump<T>(this IObservable<T> observable, string name)
        {
            observable.Subscribe(
                (i) => Console.WriteLine($"{name}-->{i}"),
                (err) => {
                    var previousColor = Console.ForegroundColor;
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine($"{name} failed-->{err.Message}");
                    Console.ForegroundColor = previousColor;
                },
                () => Console.WriteLine($"{name} completed")
            );
        }
    }
}
