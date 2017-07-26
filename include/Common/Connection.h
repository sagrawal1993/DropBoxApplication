#ifndef __CONNECTION__
	#define __CONNECTION__
	
	int serverConnection(int port, void* (*funForEachClient)(void *));	//at each client connected the function funForEachClient will be executed.

	int clientConnection(int port, char *ip);	//connect to server @ip,@port and return the socket id for the connection. 

#endif
