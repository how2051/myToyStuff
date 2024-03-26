#include <iostream>
#include <limits>

int main() {
    // 获取 float 类型的 NaN
    float nan = std::numeric_limits<float>::quiet_NaN();

    // 检查 NaN 是否等于自身
    if (nan != nan) {
        std::cout << "nan is not equal to itself" << std::endl;
		std::cout << nan << std::endl;
    } else {
        std::cout << "nan is equal to itself" << std::endl;
		std::cout << nan << std::endl;
    }

    return 0;
}