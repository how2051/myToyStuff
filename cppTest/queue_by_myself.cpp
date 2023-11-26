#include <iostream>
#include <algorithm>
using namespace std;
#include <deque>
#include <cstring>

template<class T>
class myqueue {
private:
	T* m_array;
	int m_front;
	int m_rear;
	int m_capacity;

public:
	myqueue(int cap = 10) : m_front(0), m_rear(0), m_capacity(cap+1) {
		m_array = new T[m_capacity];
		memset(m_array, 0, sizeof(m_array));
	}

	~myqueue() {
		delete[] m_array;
	}

	bool empty() { return m_front==m_rear; }
	size_t size() { return (m_rear-m_front+m_capacity)%m_capacity; }
	T& front() { return m_array[m_front]; }
	T& back() {
		return m_array[(m_rear-1+m_capacity)%m_capacity];
	}
	bool isFull() {
		return (size() == m_capacity - 1);
	}
	void push(T val) {
		if(isFull()) {
			cout << "full " << endl;
		} else {
			m_array[m_rear] = val;
			m_rear = (m_rear + 1) % m_capacity;
		}
	}

	void pop() {
		if(!empty()) {
			m_front = (m_front + 1) % m_capacity;
		}
	}
};

void test() {
	myqueue<int> mq1;
}


int main() {
	cout << "hello world" << endl;
	test();
	cout << "the End" << endl;
	return 0;
}
