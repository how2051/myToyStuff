#include <iostream>
#include <random>
#include <algorithm>
#include <memory>

using namespace std;

class OtherClass {
public:
	typedef function<int(int, int)> callbacktype;
	void registerCallBack(callbacktype cb) {
		cout << "test: " << cb(2, 4) << endl;
	}
};

class MyClass {
public:
	void func1();
	int func2(int a, int b);
private:
	OtherClass* _otherclass;
};

void MyClass::func1() {
	cout << "in func1" << endl;
	function<int(int, int)> cb = [](int a, int b) {
		return a + b;
	};
	_otherclass->registerCallBack(cb);


	// _otherclass->registerCallBack(func2);		//error


	auto cbf = std::bind(&MyClass::func2, this, std::placeholders::_1, std::placeholders::_2);
	_otherclass->registerCallBack(cbf);
}

int MyClass::func2(int a, int b) {
	cout << "in func2" << endl;
	return a + b;
}



int main() {
	MyClass mc;
	mc.func1();

	getchar();
	return 0;
}

































