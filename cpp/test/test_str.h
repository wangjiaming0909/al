#pragma once
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

void printAddress(const void* p) {
	printf("%d", p);
}

void test_str() {
	const char *p = "1234567890";
	printAddress(p);
	string s(p);
	printAddress(s.c_str());
	string s2 = s;
	printAddress(s2.c_str());
}

