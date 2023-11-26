#include <iostream>
using namespace std;

#include <vector>
#include <memory>

class Test {
public:
	Test(int val) : m_val(val) { cout << "ctor" << endl; }
	~Test(){ cout << "dtor" << endl; }
	Test(const Test& obj) {
		this->m_val = obj.m_val;
		cout << "cp ctor" << endl;
	}
	Test(Test&& obj) {
		this->m_val = obj.m_val;
		cout << "mv ctor" << endl;
	}
private:
	int m_val;
};

int main() {
	// vector<Test> mytest{0,1,};
	vector<Test> mytest2;
	mytest2.push_back(0);
	cout << "   " << endl;
	mytest2.push_back(1);
	cout << "   " << endl;

	return 0;
}



