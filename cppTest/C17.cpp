#include <iostream>
using namespace std;

[[nodiscard]] int func(const string& str, int val) {	//表明函数返回值不可以被忽略
	cout << str << endl;
	return val;
}

int main() {
	[[maybe_unused]] int val = 7;	//表明该变量可能未用到
	int ret = func("hello", 17);
	cout << ret << endl;

	if(int a = func("world", 9); a > 2) {	//if 语句能像 for 一样定义变量
		cout << a << endl;
	}

	string_view stv("hello world");	//string_view 轻量级的 string
	cout << stv << endl;

	switch (1) {
		case 1:
			//do nothing
			[[fallthrough]]
		case 2:
			cout << "here" << endl;
			[[fallthrough]]
		default:
			cout << "default" << endl;
			break;
	}
}



