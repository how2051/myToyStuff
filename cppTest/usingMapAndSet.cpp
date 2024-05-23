#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

class MyStruct {
public:
	int a = 0;
	float b = 0;

	// for unordered_map and unordered_set
	bool operator==(const MyStruct& other) const {
		return a == other.a && b == other.b;
	}

	// for map and set
	bool operator<(const MyStruct& other) const {
		return a < other.a && b < other.b;
	}
};

// for unordered_map and unordered_set
struct MyHash {
	// this should be double const
	std::size_t operator()(const MyStruct& ms) const {
		std::size_t h1 = std::hash<int>()(ms.a);
		std::size_t h2 = std::hash<float>()(ms.b);
		return h1 ^ (h2 << 1);
	}
};



int main() {
	std::unordered_map<MyStruct, int, MyHash> um;
	um.clear();

	MyStruct aa;
	MyStruct bb;

	um[aa] = 0;
	um[bb] = 1;

	std::unordered_set<MyStruct, MyHash> us;

	us.insert(aa);
	us.insert(bb);

	std::set<MyStruct> s;
	s.insert(aa);
	s.insert(bb);

	std::map<MyStruct, int> m;
	m[aa] = 0;
	m[bb] = 1;

	return 0;
}

































