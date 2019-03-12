
#include <iostream>
#include <thread>
#include <string>

using namespace std;

void thread_call(string* s) {
	*s = "456";
	cout << *s << endl;
}
void test_double_add(){
    double d = 0;
    for(long i = 0; i < 1000000; i++){
        d += 0.3;
        cout << d << endl;
    }
}

int main() {
	test_double_add();
	//string s{ "123" };
	//std::thread thread1{ thread_call, &s };
	//thread1.join();
	//cout << s << endl;
	char a[8];
	cin >> a;
}