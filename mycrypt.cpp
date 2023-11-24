#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <unistd.h>
#endif

enum DIRECTION {
	ENCRYPT = 0,
	DECRYPT = 1,
};

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

void EncryptDecrypt(const std::string& inputFile,
					const std::string& outputFile,
					const DIRECTION direction,
					const std::string& key = "helloworld")
{
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

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
std::string getFilename(const std::string inputFile) {
	std::string fileName = inputFile;
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
std::string findAvailableFileName(const std::string& baseName) {
    std::string fileName = baseName + ".bin";

    int index = 1;
#ifdef _WIN32
    while (_access(fileName.c_str(), 0) == 0) {
#elif __linux__
	while (access(fileName.c_str(), F_OK) == 0) {
#endif
        fileName = baseName + std::to_string(index) + ".bin";
        index++;
    } return fileName;
}


int main(int argc, char* argv[]) {
	if(argc < 2) { return -1; }
	//@TODO: add logic to parse input argv, to use multi flags
    std::string inputFile = argv[1];
    std::string encryptedFile = findAvailableFileName(getFilename(inputFile));
	std::cout << "Encrypted: " << encryptedFile << std::endl;
    EncryptDecrypt(inputFile, encryptedFile, ENCRYPT);
    std::cout << "File encrypted successfully." << std::endl;

    std::string decryptedFile = encryptedFile.substr(0, encryptedFile.length() - 4) + ".txt";
    EncryptDecrypt(encryptedFile, decryptedFile, DECRYPT);
    std::cout << "File decrypted successfully." << std::endl;

    return 0;
}



