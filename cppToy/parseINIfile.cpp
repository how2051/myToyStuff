#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using IniData = std::map<std::string, std::map<std::string, std::string>>;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');

    if (first == std::string::npos || last == std::string::npos) {
		return "";
	}

    return str.substr(first, last - first + 1);
}

IniData parseIniFile(const std::string& filename) {
    IniData iniData;
    std::string currentSection;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);

            if (line.empty() || line[0] == ';') {
                // 空行或注释行
                continue;
            } else if (line[0] == '[' && line[line.size() - 1] == ']') {
                // 区段标头
                currentSection = trim(line.substr(1, line.size() - 2));
            } else {
                // 键值对
                size_t equalPos = line.find('=');
                if (equalPos != std::string::npos) {
                    std::string key = trim(line.substr(0, equalPos));
                    std::string value = trim(line.substr(equalPos + 1));
                    iniData[currentSection][key] = value;
                }
            }
        } file.close();
    } else {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }

    return iniData;
}

int main() {
    // 调用解析INI文件的函数
    IniData iniData = parseIniFile("someIni.conf");

    // 打印解析结果
    for (const auto& section : iniData) {
        std::cout << "[" << section.first << "]" << std::endl;

        for (const auto& key_value : section.second) {
            std::cout << key_value.first << " = " << key_value.second << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}
