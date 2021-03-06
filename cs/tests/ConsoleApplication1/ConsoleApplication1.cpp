// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <functional>
#include <string>
#include <future>

using namespace std;

int print(int a, string s) {
	cout << a << s << endl;
	return 1;
}

void print1() {
	cout << 123 << endl;
}

int main()
{
 //   std::cout << "Hello World!\n"; 
	//typedef int (*Func)(int a, string b);
	//typedef int (*Func2)(void);
	//Func func_with_2_p = print;
	//auto func2 = std::bind(func_with_2_p, 1, "123");
	std::function<void(int, string)> task_with_param = print;
	std::function<void()> task = bind(task_with_param, 1, "123");
	task();
	promise<string> ps;
	ps.set_value("string");
	future<string> fs;
	//fs.
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
