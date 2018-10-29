using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Text;

namespace test.reactveTests
{
    class castTest
    {
         static public void test()
        {
            ISubject<object> subject = new Subject<object>();
            subject.Cast<long>().Dump("Cast to Long");
            subject.OnNext(1L);
            subject.OnNext("1");
        }

        static public void OfTypeTest()
        {
            ISubject<object> subject = new Subject<object>();
            subject.OfType<string>().Dump("type of string");
            subject.OnNext("a");
            subject.OnNext(1);//ignored
            subject.OnNext(DateTime.Now.ToShortDateString());
            subject.OnNext(DateTime.Now);//ignored
        }
    }
}
