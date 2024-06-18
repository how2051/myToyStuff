#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <cstring>
#include <thread>

class Data {
public:
	std::vector<unsigned char> data;
};

using DataPtr = std::shared_ptr<Data>;

class RingBuffer {
private:
    int                          _buffer_size;
    std::list<DataPtr>           _ring_buffer;
    std::list<DataPtr>::iterator _it_wirter;
    std::list<DataPtr>::iterator _it_reader;
    volatile std::atomic_int     _buff_used = ATOMIC_VAR_INIT(0);
    std::condition_variable      _data_available_condition;
    std::mutex                   _data_access_mutex;

    void moveIterator(std::list<DataPtr>::iterator& it) {
        it++;
        if(it == _ring_buffer.end()) {
            it = _ring_buffer.begin();
        }
    }

public:
	RingBuffer() : RingBuffer(8) {}

	RingBuffer(int buffer_size) : _buffer_size(buffer_size) {
		_ring_buffer.clear();

		if(_buffer_size <= 0) {
			_buffer_size = 8;
		}

		for(int i = 0; i < _buffer_size; i++) {
			DataPtr data = std::make_shared<Data>();
			data->data.resize(640 * 480);
			_ring_buffer.push_back(data);
		}
		_it_reader = _it_wirter = _ring_buffer.begin();
		_buff_used = ATOMIC_VAR_INIT(0);
	}

	~RingBuffer() {
		_ring_buffer.clear();
	}

	void resetRingBuffer() {		
        const std::lock_guard<std::mutex> _(_data_access_mutex);
        _it_reader = _it_wirter = _ring_buffer.begin();
        _buff_used = ATOMIC_VAR_INIT(0);
	}

	void commitBuffer() {
        std::lock_guard<std::mutex> _(_data_access_mutex);

		std::list<DataPtr>::iterator it_cur = _it_wirter;
		moveIterator(it_cur);

		if(_buff_used.load() == _buffer_size - 1 && it_cur == _it_reader) {  // in case of full
			// _drop_count++;
			std::cout << "drop one frame data" << std::endl;
			moveIterator(_it_reader);
		} else {
			++_buff_used;
		}
		_it_wirter = it_cur;
		_data_available_condition.notify_all();
	}

	bool fetchBuffer(long timeout, DataPtr& data) {
		std::unique_lock<std::mutex> _(_data_access_mutex);
		if(!_data_available_condition.wait_for(_, std::chrono::milliseconds(timeout), [this](){
			return _buff_used.load() > 0;
		})) {
			std::cout << "time out" << std::endl;
			return false;
		}

		// read data
		data = std::make_shared<Data>();
		data->data.resize(640 * 480);
		memcpy(data->data.data(), (*_it_reader)->data.data(), 640 * 480);

		moveIterator(_it_reader);
		--_buff_used;
		return true;
	}

	DataPtr getWriteableData() {
		return *_it_wirter;
	}
};


void producer(RingBuffer& buffer) {
    while(true) {
        DataPtr write = buffer.getWriteableData();
        // simulate writing data
        write->data[0] = rand() % 256;
        buffer.commitBuffer();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
    }
}

void consumer(RingBuffer& buffer) {
    while(true) {
        DataPtr read;
		if(buffer.fetchBuffer(500, read)) {
			// simulate reading data
			std::cout << "read out data 0 : " << (int)read->data[0] << std::endl;
		}
    }
}

// g++ -o ring ringbuffer.cpp -lpthread
int main() {
    RingBuffer buffer;
    std::thread producer_thread(producer, std::ref(buffer));
    std::thread consumer_thread(consumer, std::ref(buffer));

    producer_thread.join();
    consumer_thread.join();

	return 0;
}




