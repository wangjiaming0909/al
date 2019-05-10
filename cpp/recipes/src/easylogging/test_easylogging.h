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
        this_thread::sleep_for(400ms);
        LOG(INFO) << str << " logging..." << str;
    }
}

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dis(33, 126);

void main_thread_routine(string* str)
{
    using namespace std::chrono_literals;
    // while(1)
    // {
        this_thread::sleep_for(1s);
        // *str = dis(gen);
        delete str;
        // LOG(INFO) << "str has been released" << str;
    // }
}

void test()
{
    setupLogger();
    std::string* str = new string("abcdefd");
    std::thread thread1{threadRoutine, std::ref(*str)};
    std::thread thread2{threadRoutine, std::ref(*str)};
    main_thread_routine(str);
    thread1.join();
    thread2.join();
}

} //namespace easylogging

#endif //TEST_EASYLOGGING_H_