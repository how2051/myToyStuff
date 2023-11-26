#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <list>
#include <iostream>
#include <exception>
#include <pthread.h>
#include "locker.h"


template <typename T>
class Threadpool {
public:
	Threadpool(int thread_number = 8, int max_requests = 10000) :
		m_thread_number(thread_number), m_max_requests(max_requests),
		m_stop(false), m_threads(nullptr)
	{
		if(thread_number <= 0 || max_requests <= 0) {
			throw std::exception();
		}

		m_threads = new pthread_t[m_thread_number];
		if(!m_threads) {
			throw std::exception();
		}

		if(pthread_mutex_init(&m_mutex, nullptr) != 0) {
			throw std::exception();
		}

		if(sem_init(&m_sem, 0, 0) != 0) {
			throw std::exception();
		}

		for(int i = 0; i < m_thread_number; i++) {
			cout << "creating the " << i << " thread" << endl;
			if(pthread_create(m_threads + i, nullptr, worker, this) != 0) {
				delete [] m_threads;
				throw std::exception();
			}

			if(pthread_detach(m_threads[i])) {
				delete [] m_threads;
				throw std::exception();
			}
		}
	}

	~Threadpool() {
		delete [] m_threads;
		m_stop = true;
	}

	bool append(T* request) {
		pthread_mutex_lock(&m_mutex);
		if(m_request_queue.size() > m_max_requests) {
			pthread_mutex_unlock(&m_mutex);
			return false;
		}
		m_request_queue.push_back(request);
		pthread_mutex_unlock(&m_mutex);
		sem_post(&m_sem);
		return true;
	}

private:
	static void* worker(void* arg) {
		Threadpool* pool = static_cast<Threadpool*>(arg);
		pool->run();
		return pool;
	}

	void run() {
		while(!m_stop) {
			sem_wait(&m_sem);
			pthread_mutex_lock(&m_mutex);
			if(m_request_queue.empty()) {
				pthread_mutex_unlock(&m_mutex);
				continue;
			}
			T* request = m_request_queue.front();
			m_request_queue.pop_front();
			pthread_mutex_unlock(&m_mutex);
			if(!request) continue;
			request->process();
		}
	}


private:
	int m_thread_number;
	int m_max_requests;
	pthread_t* m_threads;
	std::list<T*> m_request_queue;
	pthread_mutex_t m_mutex;
	sem_t m_sem;
	bool m_stop;
};


#endif