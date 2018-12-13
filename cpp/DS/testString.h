
#include <iostream>
#include <string>
using namespace std;

void printAdress(void* p) {
	cout << p << endl;
}

void test_string() {
	string s = "123";
	printAdress(&s);
	string s2(s);
	printAdress(&s2);
}
