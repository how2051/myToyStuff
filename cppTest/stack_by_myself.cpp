#include <iostream>
#include <algorithm>
using namespace std;
#include <deque>
#include <cstring>

template<class T>
class mystack {
private:
	T* m_array;
	int m_cur;
	int m_capacity;
public:
	mystack(int cap = 10) : m_cur(0), m_capacity(cap+1) {
		m_array = new T[m_capacity];
		memset(m_array, 0, sizeof(m_array));
	}

	~mystack() {
		delete[] m_array;
	}

	bool empty() { return m_cur==0; }
	size_t size() { return m_cur; }
	T& top() { return m_array[m_cur-1]; };
	bool isFull() { return m_cur==m_capacity-1; }
	void push(T val) {
		if(isFull()) {
			cout << "full " << endl;
		} else {
			m_array[m_cur++] = val;
		}
	}

	void pop() {
		if(empty()) {
			cout << "empty" << endl;
		} else {
			m_cur--;
		}
	}
};

void test() {
	mystack<char> ms1;
}

int main() {
	cout << "hello world" << endl;
	test();
	return 0;
}
