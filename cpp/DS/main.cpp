#include <iostream>
#include <string>
#include <vector>
#include <exception>
using namespace std;

template <class T>
class Singleton {
public:
	static T* instance() {
		if (instance_ == 0) {
			instance_ = new T();
		}
		return instance_;
	}
private:
	static T* instance_;
};

template <class T>
T* Singleton<T>::instance_ = 0;

class Class_without_default_constructor {
public:
	Class_without_default_constructor(int a) {
		cout << 123 << endl;
	}
};

int main() {
	//auto ss = Singleton<string>::instance();
	//ss->append("asd");
	//cout << *ss << endl;

	////auto sc = Singleton<Class_without_default_constructor>::instance();
	//cout << is_trivial<Class_without_default_constructor>::value << endl;
	//cout << is_trivial<string>::value << endl;
	//cout << is_trivial<int>::value << endl;

	vector<string> v{};
	v.reserve(2);
	try {
		v.at(2) = "as";
		//v[2] = "12";
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	char a[16];
	cin >> a;
}
