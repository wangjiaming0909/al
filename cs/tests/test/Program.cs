using System;
using System.Reactive;
using System.Reactive.Linq;
using test.reactveTests;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            //SubscribeTest.test();
            //castTest.test();
            //castTest.OfTypeTest();
            //TestEvent.testEvent();
            //EventHandler_EventArgs.test();
            //Customized_Delegate.test();
            EventHandler_EventArgs.TestWithFromEventPattern();
            //Customized_Delegate.TestWithFromEvent();
        }
    }
}
