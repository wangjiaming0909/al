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
            IObservable<string> observable = Observable.Create<string>(
                (ob) =>
                {
                    //ob.OnNext("1");
                    //ob.OnNext("2");
                    //ob.OnCompleted();
                    return subject.Subscribe(Console.WriteLine, () => Console.WriteLine("subject completed"));
                }
            );
            observable.Subscribe((str) => Console.WriteLine(str), () => Console.WriteLine("completed"));
            subject.OnNext("1");
        }
    }
}
