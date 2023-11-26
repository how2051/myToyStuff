#include <iostream>
#include <algorithm>
using namespace std;
#include <deque>

template<class T, class Sequence = deque<T>>
class myqueue {
private:
	Sequence m_c;
public:
	bool empty() { return m_c.empty(); }
	size_t size() { return m_c.size(); }
	T& front() { return m_c.front(); }
	T& back() { return m_c.back(); }
	void push(T val) { m_c.push_back(val); }
	void pop() { m_c.pop_front(); }
};



int main() {
	cout << "hello world" << endl;

	cout << "the End" << endl;
	return 0;
}
