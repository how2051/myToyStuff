#include <iostream>
#include <fstream>
#include <string>

void xorEncryptDecrypt(const std::string& inputFile,
					   const std::string& outputFile,
					   const std::string& key)
{
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

    char byte;
    size_t keyIndex = 0;
    while (ifs.get(byte)) {
        // 对每个字节进行异或运算
        byte ^= key[keyIndex];
        ofs.put(byte);

        // 循环使用密钥
        keyIndex = (keyIndex + 1) % key.size();
    }

    ofs.close();
    ifs.close();
}

int main() {
    std::string inputFile = "plaintext.txt";
    std::string encryptedFile = "encrypted.bin";
    std::string decryptedFile = "decrypted.txt";
    std::string key = "helloworld";

    // 加密
    xorEncryptDecrypt(inputFile, encryptedFile, key);
    std::cout << "File encrypted successfully." << std::endl;

    // 解密
    xorEncryptDecrypt(encryptedFile, decryptedFile, key);
    std::cout << "File decrypted successfully." << std::endl;

    return 0;
}
