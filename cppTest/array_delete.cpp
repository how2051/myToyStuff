#include <iostream>
#include <memory>
using namespace std;

class Widget {
public:
	Widget() {
		m_id = m_cnt++;
		m_p = new char[10];
		cout << "ctor " << m_cnt << endl;
	}

	~Widget() {
		delete[] m_p;
		cout << "dtor " << m_cnt <<  endl;
		m_cnt--;
	}

	int id() {
		return m_id;
	}

private:
	static int m_cnt;
	int m_id;
	char* m_p;
};

int Widget::m_cnt = 0;


#include <algorithm>
int main() {
	typedef Widget ArrayWidget[4];
	ArrayWidget a1;
	Widget* pW = new ArrayWidget;
	for(int i = 0; i < 4; i++) {
		cout << pW[i].id() << endl;
	} delete[] pW;
	return 0;
}



