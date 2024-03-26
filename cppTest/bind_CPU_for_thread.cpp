#include <iostream>
#include <unistd.h>
#include <memory>
#include <thread>
#include <vector>
#include <pthread.h>

void threadFunction(int coreId) {
    // 获取线程ID
    pthread_t threadId = pthread_self();

    // 创建一个CPU核心集合，只包含指定的CPU核心
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(coreId, &cpuset);

    // 将当前线程绑定到指定的CPU核心上
    int result = pthread_setaffinity_np(threadId, sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        std::cerr << "Failed to set thread affinity: " << result << std::endl;
    } else {
        std::cout << "Thread " << threadId << " is running on CPU core " << coreId << std::endl;
	}

    // 在这里执行你的线程任务
    // ...
}

int main() {
    // 获取系统中可用的CPU核心数量
    int numCores = sysconf(_SC_NPROCESSORS_ONLN);

    // 创建一个线程向量
    std::vector<std::thread> threads;

    // 创建与CPU核心数量相同的线程，并将它们绑定到不同的CPU核心上
    for (int i = 0; i < numCores; ++i) {
        threads.emplace_back(threadFunction, i);
		usleep(50);
    }

    // 等待所有线程结束
    for (std::thread& t : threads) {
        t.join();
    }

    return 0;
}

