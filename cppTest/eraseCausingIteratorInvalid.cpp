#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>


// in main thread, we keep iterate and print value of item in map
// in child thread, we erase one item
// run this demo several times, and sometimes it will print invalid number
// this is because erase causing the iterator become invalid


int main() {
	std::unordered_map<int, int> um;
	um.clear();
	std::cout << "map size : " << um.size() << std::endl;

	um[1] = 2;
	um[2] = 3;
	um[3] = 4;
	std::cout << "map size : " << um.size() << std::endl;

	auto mythreadfunc = [&um]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "going to erase map =================== " << std::endl;
		um.erase(1);
		std::cout << " =================== map size : " << um.size() << std::endl;
		return;
	};

	std::thread mythread(mythreadfunc);

	while(1) {
		int round = 0;
		for(auto pair : um) {
			std::cout << round++;
			std::cout << ", first : " << pair.first << ", second : " << pair.second << std::endl;
		}
	}

	return 0;
}

































