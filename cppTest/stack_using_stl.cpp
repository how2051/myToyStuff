#include <iostream>
#include <algorithm>
using namespace std;
#include <deque>

template<class T, class Sequence = deque<T>>
class mystack {
private:
	Sequence m_c;
public:
	bool empty() { return m_c.empty(); }
	size_t size() { return m_c.size(); }
	T& top() { return m_c.back(); }
	void push(T val) { m_c.push_back(val); }
	void pop() { m_c.pop_back(); }
};

int main() {
	cout << "hello world" << endl;
	return 0;
}
