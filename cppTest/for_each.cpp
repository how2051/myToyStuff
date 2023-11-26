#include <iostream>
#include <algorithm>
using namespace std;

#include <vector>
int main() {
	vector<int> vec;
	for(int i = 0; i < 5; i++) {
		vec.emplace_back(i);
	}

	auto func = [](const int & val) {
		cout << val << endl;
	};

	for_each(vec.begin(), vec.end(), func);

	return 0;
}



