#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> pq;

    pq.push(5);
    pq.push(2);
    pq.push(8);
    pq.push(1);
    pq.push(3);

    std::cout << "Priority Queue elements after insertion: ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }

    std::cout << std::endl;

    std::cout << "Priority Queue elements after pop: ";
    pq.push(5);
    pq.push(2);
    pq.push(8);
    pq.push(1);
    pq.push(3);
    pq.pop(); // 移除最大元素

    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }

    std::cout << std::endl;
	getchar();
    return 0;
}
