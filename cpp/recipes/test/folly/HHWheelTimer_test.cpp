#include <gtest/gtest.h>
#include <folly/io/async/HHWheelTimer.h>
#include <folly/io/async/EventBase.h>
#include <iostream>

void timeoutFn()
{
    using namespace std;
    cout << "timeout expired..." << endl;
}

TEST(HHWheelTimer, normal_test)
{
    using namespace folly;
    using namespace std::chrono_literals;
    EventBase base;

    HHWheelTimer *timer = new HHWheelTimer{&base};

    timer->scheduleTimeoutFn(timeoutFn, 1s);
    timer->scheduleTimeoutFn(timeoutFn, 2ms);
    timer->scheduleTimeoutFn(timeoutFn, 3ms);

    base.loop();
}