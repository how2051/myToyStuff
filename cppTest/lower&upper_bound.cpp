#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 5, 7, 8, 9, 10};
    int target = 7;

    // 使用 std::lower_bound 查找不小于目标值的第一个元素
    auto lower = std::lower_bound(numbers.begin(), numbers.end(), target);

    // 使用 std::upper_bound 查找大于目标值的第一个元素
    auto upper = std::upper_bound(numbers.begin(), numbers.end(), target);

    // 输出查找结果
    std::cout << "Lower bound: " << std::distance(numbers.begin(), lower) << std::endl;
    std::cout << "Upper bound: " << std::distance(numbers.begin(), upper) << std::endl;
	std::cout << "lower: " << *lower << std::endl;
	std::cout << "upper: " << *upper << std::endl;

    return 0;
}
