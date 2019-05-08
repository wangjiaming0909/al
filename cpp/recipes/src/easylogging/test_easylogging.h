#ifndef TEST_EASYLOGGING_H_
#define TEST_EASYLOGGING_H_

#include "easylogging/easylogging++.h"
#include <iostream>
#include <thread>
#include <string>

namespace easylogging
{
void setupLogger()
{
    using namespace el;
    Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(Level::Global, ConfigurationType::Enabled, "true");
    defaultConf.set(Level::Global, ConfigurationType::Format, "%datetime, %thread, %file, %level, %line, %msg");
    defaultConf.set(Level::Global, ConfigurationType::PerformanceTracking, "false");
    defaultConf.set(Level::Global, ConfigurationType::ToFile, "false");
    defaultConf.set(Level::Global, ConfigurationType::ToStandardOutput, "true");
    defaultConf.set(Level::Global, ConfigurationType::Filename, "./log");
    defaultConf.set(Level::Global, ConfigurationType::LogFlushThreshold, "100");
    defaultConf.set(Level::Global, ConfigurationType::MaxLogFileSize, "2097152");

    Loggers::reconfigureAllLoggers(defaultConf);
}

void threadRoutine(const string& str)
{
    while(1)
    {
        using namespace std::chrono_literals;
        this_thread::sleep_for(10ms);
        LOG(INFO) << str << " logging...";
    }
}

void test()
{
    setupLogger();
    std::string str = "123456";
    std::thread thread1{threadRoutine, std::ref(str)};
    std::thread thread2{threadRoutine, std::ref(str)};
    threadRoutine(str);
    thread1.join();
    thread2.join();
}

} //namespace easylogging

#endif //TEST_EASYLOGGING_H_