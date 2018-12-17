
#include <iostream>
#include <thread>
#include <string>

using namespace std;

void thread_call(string* s) {
	*s = "456";
	cout << *s << endl;
}


int main() {
	string s{ "123" };
	std::thread thread1{ thread_call, &s };
	thread1.join();
	cout << s << endl;
	char a[8];
	cin >> a;
}