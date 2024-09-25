#include <chrono>
#include <iostream>
#include <thread>

#include <queue>
#include <algorithm>
#include <condition_variable>


class Widget {

private:
	int                                _queue_len;
	bool                               _is_work_started;
	std::thread                        _callback_worker_thread;
	std::queue<std::function<void()>>  _tasks_queue;
	std::mutex                         _task_mtx;
	std::condition_variable            _task_cv;

	void WorkerFunc()
	{
		std::cout << "WorkerFunc start" << std::endl;
		while(_is_work_started) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> _(_task_mtx);
				_task_cv.wait(_, [this](){
					return (!this->_tasks_queue.empty() || _is_work_started == false);
				});

				if(this->_tasks_queue.empty()) continue;
				task = this->_tasks_queue.front();
				this->_tasks_queue.pop();
			}
			task();
		} std::cout << "WorkerFunc stop" << std::endl;
		return;
	}

public:
	Widget() : _is_work_started(false), _queue_len(5) {
		//
	}

	~Widget() {
		//
	}

	void init() {
		_is_work_started = true;
		if(!_callback_worker_thread.joinable()) {
			_callback_worker_thread = std::thread([this]() {
				WorkerFunc();
			});
		}
	}

	void uninit() {
		_is_work_started = false;
		_task_cv.notify_one();

		if(_callback_worker_thread.joinable()) {
			_callback_worker_thread.join();
		}
	}

	void submit(std::function<void()> task) {
		{
			std::lock_guard<std::mutex> _(_task_mtx);
			if(_tasks_queue.size() >= _queue_len) {
				_tasks_queue.pop();
			}
			_tasks_queue.push(std::move(task));
		}
		_task_cv.notify_one();
	}

};




int main() {
	Widget widget;

	widget.init();

	for(int i = 0; i < 6; i++) {
		widget.submit([i]() {
			std::cout << "task " << i << std::endl;
			return;
		});
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	widget.uninit();

	return 0;
}




