// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <boost/timer.hpp>
#include <ctime>
#include <boost/progress.hpp>
#include <thread>
#include <cstdlib>
//#include <boost/thread.hpp>
using namespace boost;
using namespace std;

void print(int i) {
	progress_timer timer;
	for (size_t j = 0; j < 10000; j++)
	{
		//cout << i;
		i = i + i - i / 100 - i + i * (i - 2);
	}
	cout << endl;
}
void print1(){
	progress_timer timer;
	for (size_t j = 0; j < 10000; j++)
	{
		;
	}
	cout << endl;
}
int main()
{
	//timer t;
	//cout << CLOCKS_PER_SEC << endl;//CPU每秒的时钟数
	//cout << clock() << endl;//返回当前程序所占用的CPU时钟数
	//cout << t.elapsed_min() << endl;
	//cout << t.elapsed() << endl;
	//cout << t.elapsed_max() << endl;
	//timer t2;
	//cout << t2.elapsed() << endl;
	//cout << clock() << endl;;
 //   std::cout << "Hello World!\n"; 
	//print(10000);
	//std::thread t(print, 1);

	progress_display pd(100);
	for (int i = 0; i < 10000; i++)
	{
		if (i % 100 == 0) {
			++pd;
		}
	}
	getchar();
}
