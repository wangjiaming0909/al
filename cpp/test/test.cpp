// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <boost/timer.hpp>
#include <ctime>
using namespace boost;
using namespace std;

int main()
{
	timer t;
	cout << clock() << endl;;
	cout << t.elapsed_max() << endl;
	cout << t.elapsed_min() << endl;
	cout << t.elapsed() << endl;
    std::cout << "Hello World!\n"; 
}