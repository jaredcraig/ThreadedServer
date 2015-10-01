#pragma once

#include <pthread.h>
#include <queue>

#define CAPACITY 10
#define THREADS 10

using namespace std;
//------------------------------------------------------------------------------
class Buffer {
	private:
		queue<int> buffer;
		pthread_mutex_t lock;
		pthread_cond_t not_full;
		pthread_cond_t not_empty;

	public:
		Buffer() {
			pthread_mutex_init(&lock, NULL);
			pthread_cond_init(&not_full, NULL);
			pthread_cond_init(&not_empty, NULL);
		}
		void append(int c) {
			pthread_mutex_lock(&lock);
			while ( buffer.size() >= CAPACITY ) {
				pthread_cond_wait(&not_full, &lock);
			}
			buffer.push(c);
			pthread_cond_signal(&not_empty);
			pthread_mutex_unlock(&lock);
		}
		int take() {
			pthread_mutex_lock(&lock);
			while(buffer.empty()) {
				pthread_cond_wait(&not_full, &lock);
			}
			int c = buffer.front();
			buffer.pop();
			pthread_cond_signal(&not_full);
			pthread_mutex_unlock(&lock);
			return c;
		}
};
