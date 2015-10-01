#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "inet-server.h"
#include "buffer.h"
#include <pthread.h>

using namespace std;

void *threadHandle(void*);

int main(int argc, char **argv) {
	int option, port;

	// setup default arguments
	port = 5000;

	// process command line options using getopt()
	// see "man 3 getopt"
	while ((option = getopt(argc, argv, "p:")) != -1) {
		switch (option) {
		case 'p':
			port = atoi(optarg);
			break;
		default:
			cout << "server [-p port]" << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	InetServer server = InetServer(port);
	
	
	pthread_t tidA;
	Buffer b;
	
	//pthread_t* tid = new pthread_t[THREADS];
	
	pthread_create(&tidA, NULL, &threadHandle, &b);
	
 	//for (long id = 1; id <= THREADS; id++) {
	//	pthread_create(&tid[id], NULL, &Server::threadHandle, (void*)id);
	//}

	server.run(b);
}

//-----------------------------------------------------------------------------
void *threadHandle(void *vptr) {
	Buffer *b;
	
	b = (Buffer*) vptr;
	
//	while(true) {
//		int client = b->take();
//		handle(client);
//		b->append(client);
//	}

}
