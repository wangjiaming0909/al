
#include <iostream>
#include <thread>

using namespace std;

int main() {
	cout << thread::hardware_concurrency() << endl;//8
	cout << 123 << endl;
	char a[8];
	cin >> a;
}