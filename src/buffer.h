#pragma once

#include <queue>
#include <pthread.h>
#include <map>
#include "server.h"

using namespace std;

#define NUM_THREADS 10

//------------------------------------------------------------------------------
class Buffer {

public:
	bool handling;
	map<int, string> client_cache;
	queue<int> buffer;

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t handle_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

	bool find(int c) {
		return client_cache.find(c) != client_cache.end();
	}
	void set_cache(int c, string s) {
		map<int, string>::iterator it;
		it = client_cache.find(c);
		if (it == client_cache.end()) {
			client_cache.insert(pair<int, string>(c, s));
		} else {
			it->second = s;
		}
	}
	string get_cache(int c) {
		map<int, string>::iterator it;
		it = client_cache.find(c);
		if (it != client_cache.end()) {
			string cache = it->second;
			return cache;
		}
		return NULL;
	}
	void erase(int c) {
		if (client_cache.find(c) != client_cache.end()) {
			client_cache.erase(c);
		}
		unlock_thread();
	}

	void lock_thread() {
		pthread_mutex_lock (&handle_lock);
	}
	void unlock_thread() {
		pthread_mutex_unlock (&handle_lock);
	}
	void append(int c) {
		//cout << pthread_self() << "    <BUFFER> append: " << c << endl;
		pthread_mutex_lock (&lock);
		buffer.push(c);
		pthread_cond_signal(&not_empty);
		pthread_mutex_unlock (&lock);
	}
	int take() {
		pthread_mutex_lock (&lock);
		while (buffer.empty()) {
			pthread_cond_wait(&not_empty, &lock);
		}
		int c = buffer.front();
		buffer.pop();
		pthread_mutex_unlock (&lock);
		return c;
	}
	int size() {
		return buffer.size();
	}

};
