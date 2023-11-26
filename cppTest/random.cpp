#include <iostream>
using namespace std;


#include <random>
#include <vector>
#include <algorithm>


int main() {
	vector<int> vec;
	random_device rd;
	mt19937 rng(rd());
	for(int i = 0; i < 10; i++) {
		uniform_int_distribution<int> dis(0,9);
		vec.emplace_back(dis(rng));
		// vec.emplace_back(rng()%10);
	}
	for_each(vec.begin(), vec.end(), [](const int & val){cout << val << " ";}); cout << endl;
	shuffle(vec.begin(), vec.end(), rng);
	for_each(vec.begin(), vec.end(), [](const int & val){cout << val << " ";}); cout << endl;
	return 0;
}



