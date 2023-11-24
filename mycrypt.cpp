#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "CLI11.hpp"

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <unistd.h>
#endif

enum DIRECTION {
	ENCRYPT = 0,
	DECRYPT = 1,
};

constexpr std::string_view g_KEY = "helloworld";

// //for test
// void printBinaryChar(const char value) {
//     for (int i = 7; i >= 0; --i) {
//         int bit = (value >> i) & 1;
//         std::cout << bit;
//     } std::cout << std::endl;
// }

void swap2bit(char& byte) {
	char low2 = byte & 0x03;
	char high2 = (byte & 0xC0) >> 6;
	byte &= 0x3C;
	byte |= (low2 << 6 | high2);
}

void EncryptDecrypt(const std::string_view inputFile,
					const std::string_view outputFile,
					const DIRECTION direction,
					const std::string_view key = g_KEY)
{
    std::ifstream ifs(static_cast<std::string>(inputFile), std::ios::binary);
    std::ofstream ofs(static_cast<std::string>(outputFile), std::ios::binary);

    char byte;
    size_t keyIndex = 0;
    while (ifs.get(byte)) {
		// do `xor` for each byte
		if(direction == ENCRYPT) {
			byte ^= key[keyIndex];
			swap2bit(byte);
		} else if (direction == DECRYPT) {
			swap2bit(byte);
			byte ^= key[keyIndex];
		}
        ofs.put(byte);

        // recycle the key
        keyIndex = (keyIndex + 1) % key.size();
    }

    ofs.close();
    ifs.close();
}


//remove prefix path, replace `.` with `_`
std::string getFilename(const std::string_view inputFile) {
	std::string fileName = static_cast<std::string>(inputFile);
    size_t lastSlashPos     = inputFile.rfind('/');
    size_t lastBackslashPos = inputFile.rfind('\\');
	size_t lastSeparatorPos = std::max((lastSlashPos == std::string::npos) ? 0 : lastSlashPos,
									   (lastBackslashPos == std::string::npos) ? 0 : lastBackslashPos);

    if (lastSeparatorPos != 0) {
        fileName = inputFile.substr(lastSeparatorPos + 1);
        std::cout << "File Name: " << fileName << std::endl;
    }
	std::replace(fileName.begin(), fileName.end(), '.', '_');
	return fileName;
}

//find a new suitable filename if current name is occupied
std::string findAvailableFileName(const std::string_view baseName) {
    std::string fileName = static_cast<std::string>(baseName) + ".bin";

    int index = 1;
#ifdef _WIN32
    while (_access(fileName.c_str(), 0) == 0) {
#elif __linux__
	while (access(fileName.c_str(), F_OK) == 0) {
#endif
        fileName = static_cast<std::string>(baseName) + std::to_string(index) + ".bin";
        index++;
    } return fileName;
}


int main(int argc, char* argv[]) {
	if(argc < 2) { return -1; }

	std::string_view key = g_KEY;
	std::string_view filetobeEncrypted, filetobeDecrypted;

	CLI::App app{"parse input command line"};
	app.add_option("-k,--key", key, "the key string use to encrypt");
	app.add_option("-e,--encrypt", filetobeEncrypted, "the input file which will be encrypted");
	app.add_option("-d,--decrypt", filetobeDecrypted, "the input file which will be decrypted");

	CLI11_PARSE(app, argc, argv);

	if(app.count("--encrypt")) {
		std::cout << "Encrypting file..." << std::endl;
		std::string encryptedFile = findAvailableFileName(getFilename(filetobeEncrypted));
		std::cout << "Encrypted output: " << encryptedFile << std::endl;
		EncryptDecrypt(filetobeEncrypted, encryptedFile, ENCRYPT);
		std::cout << "File encrypted successfully." << std::endl;
	} else if (app.count("--decrypt")) {
		std::cout << "Decrypting file..." << std::endl;
		std::string decryptedFile = static_cast<std::string>(filetobeDecrypted.substr(0, filetobeDecrypted.length() - 4)) + ".txt";
		std::cout << "Decrypted output: " << decryptedFile << std::endl;
		EncryptDecrypt(filetobeDecrypted, decryptedFile, DECRYPT);
		std::cout << "File decrypted successfully." << std::endl;
	} else {	//@TODO: should use CLI rule to avoid coming here
		std::cerr << "no file input" << std::endl;
	}

    return 0;
}



