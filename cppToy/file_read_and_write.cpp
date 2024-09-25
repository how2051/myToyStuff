#include <iostream>
#include <random>
#include <algorithm>
#include <memory>

#include <iostream>
#include <fstream>       // std::ifstream
#include <vector>        // std::vector

int main() {
	std::ifstream input_file("input.png", std::ios::binary);
	if(!input_file.is_open()) {
		std::cerr << "open input failed" << std::endl;
		return 1;
	}

	input_file.seekg(0, std::ios::end);
	std::streampos file_size = input_file.tellg();
	input_file.seekg(0, std::ios::beg);

	std::vector<char> file_vector(file_size);

	input_file.read(file_vector.data(), file_size);
	input_file.close();

	std::ofstream output_file("output.png", std::ios::binary);
	if(output_file.is_open()) {
		output_file.write(file_vector.data(), file_size);
		output_file.close();
		std::cout << "save new output" << std::endl;
	}

	return 0;
}

































