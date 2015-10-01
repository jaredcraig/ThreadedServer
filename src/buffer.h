#pragma once

#include <queue>
#include <pthread.h>
#include "inet-server.h"

using namespace std;

#define NUM_THREADS 10

//------------------------------------------------------------------------------
class Buffer {
	private:
		queue<int> buffer;
		Server server;
		pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
		pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
		pthread_cond_t not_handling = PTHREAD_COND_INITIALIZER;

	public:
		Buffer() {
		}
		void append(int c) {
			pthread_mutex_lock(&lock);
			while ( buffer.size() >= NUM_THREADS ) {
				pthread_cond_wait(&not_full, &lock);
			}
			buffer.push(c);
			pthread_cond_signal(&not_empty);
			pthread_mutex_unlock(&lock);
		}
		int take() {
			pthread_mutex_lock(&lock);
			while(buffer.empty()) {
				cout << pthread_self() << "    <take> waiting for condition(not_empty)" << endl;
				pthread_cond_wait(&not_empty, &lock);
				pthread_cond_wait(&not_handling, &lock);
			}
			int c = buffer.front();
			cout << pthread_self() << "    <take> client: " << c << endl;
			pthread_cond_signal(&not_full);
			pthread_mutex_unlock(&lock);
			buffer.pop();
			return c;
		}
		int size() {return buffer.size();}
		
};
