#pragma once

#include <queue>
#include <pthread.h>
#include "server.h"

using namespace std;

#define NUM_THREADS 10

//------------------------------------------------------------------------------
class Buffer {

	public:

		queue<int> buffer;
		pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t handle_lock = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
		pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
		pthread_cond_t not_handling = PTHREAD_COND_INITIALIZER;

		Buffer() {}
		void start_handling() {
			pthread_mutex_lock(&handle_lock);
			pthread_cond_wait(&not_handling, &handle_lock);
		}
		void done_handling() {
			pthread_cond_signal(&not_handling);
			pthread_mutex_unlock(&handle_lock);
			cout << pthread_self() << "    <buffer> done handling\n---------------------------------------------\n";
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
			}
			int c = buffer.front();
			cout << "\n---------------------------------------------\n" << pthread_self() << "    <take> client: " << c << endl;
			buffer.pop();
			pthread_cond_signal(&not_full);
			pthread_mutex_unlock(&lock);

			return c;
		}
		int size() {return buffer.size();}
		
};
