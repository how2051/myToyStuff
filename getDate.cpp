#include <iostream>
#include <chrono>
#include <ctime>
#include <tuple>
#include <cmath>

// this is used to create a func that generate some data w.r.t. date,
// and make it not obvious
// if some hardcode logic using some magic number, and this number is
// - related to date, like year, then when the year changes, error will happend

int main() {
	auto getXYZ = []() -> std::tuple<int, int, int> {
		auto a = std::chrono::system_clock::now();
		std::time_t b = std::chrono::system_clock::to_time_t(a);
		std::tm c = *std::localtime(&b);
		int* p = reinterpret_cast<int*>(&c);
		return std::make_tuple(p[5] + 1900, p[4] + 1, p[3]);
	};

	auto [a, b, c] = getXYZ();

	// std::cout << a << ":" << b << ":" << c << std::endl;

	auto calculateR = [](int a, int b, int c) {
		return (std::pow(std::sin(b), 2) + a - std::pow(std::cos(c), 2))
				+ (std::cos(b)*std::cos(b) - (1 - std::cos(2*c)) / 2);
	};

	// std::cout << calculateR(a, b, c) << std::endl;

	int i = 1;
	if(i < calculateR(a, b, c) - 2021) {
		std::cout << "work normally" << std::endl;
	} else {
		std::cout << "going to crash!!!" << std::endl;
		exit(-1);
	}

    return 0;
}
































