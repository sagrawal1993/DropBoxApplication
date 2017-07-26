/*****************************************************************************************************
*
*	@author:: Suraj Agrawal
*	@project:: DropBox For linux system.
*
*	Description::	This file contains the code for making server, and client.
*				A function is being passed to the server, that is 
*				being called after the new client get connected (@server side)
*				and after the client successfully connected to server,
*				the function return Socket id for client.
*
*
*
****************************************************************************************************/



#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <Common/Connection.h>

//start the server @arg1 port for listening the drop box request.
int serverConnection(int port, void* (*funForEachClient)(void *)){
	
	int serverid;
	int *connid;		//initialize the address of the server socket
	struct sockaddr_in servaddr;
	long unsigned int threadId;
	int optionValue=1;

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		
	//create the socket
	serverid=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(serverid<0){
		perror("Error in getting the serverid\n");
		return -1;
	}

	if(setsockopt(serverid, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue))<0){
		perror("Error in setsockopt server\n");
		return -1;
	}
	//bind the server address to the soket
	if(bind(serverid,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		printf("error in binding the socket");
		return -1;
	}
	
	//fix the length of queue.
	if(listen(serverid,10)<0){
		printf("will not able to run the listen sys call");
		return -1;
	}
	
	
	while(1){		//this while is used to iterate the server
		connid=(int *)malloc(sizeof(int));
		*connid=accept(serverid,NULL,NULL);
		if(*connid<0){//will generate id for accepting at server
			printf("will not able to run the accept");
			return -1;
		}
		if(pthread_create( &threadId, NULL, funForEachClient, (void *)(connid))!=0){
			printf("Error in thread creation\n");
			return -1;
		}
		//funForEachClient(connid);
	}	
	return 1;
}		



// This function will connect machine to particular ip and port.
// On success it will return the connection id;
// On Error return -1.
int clientConnection(int port,char *ip)
{
	struct sockaddr_in servaddr;
	int connid;	
	int optionValue=1;

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=inet_addr(ip);//htonl(INADDR_ANY);
	if(memset(servaddr.sin_zero,'\0',8)!=servaddr.sin_zero){
		printf("Error in memset in clientConnection\n");
		return -1;
	
	}
	
	connid=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(connid<=0){
		return -1;
	}

	if(setsockopt(connid, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue))<0){
		perror("Error in setsockopt server\n");
		return -1;
	}

	if(connect(connid,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		printf("error in connect");
		return -1;
	}
	
	return connid;
}
