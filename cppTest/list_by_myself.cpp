#include <iostream>
#include <algorithm>
using namespace std;

template <class T>
struct _listNode {
	void* m_prev;
	void* m_next;
	T m_data;
};

template <class T>
struct _listIterator {
	typedef _listIterator<T> iterator;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef _listNode<T>* link_type;

	link_type m_node;

	_listIterator(link_type x) : m_node(x) { };
	_listIterator() = default;
	_listIterator(const iterator& x) : m_node(x.m_node) { };

	bool operator==(const _listIterator<T>& x) const {
		return this->m_node == x.m_node;
	}
	bool operator!=(const _listIterator<T>& x) const {
		return this->m_node != x.m_node;
	}
	reference operator*() const { return (*m_node).m_data; }
	pointer operator->() const { return &(operator*()); }
	_listIterator<T>& operator++() {
		m_node = (link_type)m_node->m_next;
	}
	_listIterator<T> operator++(int) {
		_listIterator<T> tmp = *this;
		++*this;
		return tmp;
	}
};


template <class T>
class mylist {
	typedef _listNode<T> listNode;
	listNode* m_node;

	// iterator begin() { return (link_type)((*m_node).m_next); }
};

int main() {
	cout << "hello world" << endl;
	return 0;
}
