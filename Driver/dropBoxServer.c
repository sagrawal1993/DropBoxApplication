/*****************************************************************************************************
*
*	@author:: Suraj Agrawal
*	@Project:: Drop Box for linux.
*
*	Description:: This file contains code for driver, for Server for DropBox.
*				
*
*
*
*
****************************************************************************************************/
#include <unistd.h>
#include "Common/Connection.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "Server/ServerCommon.h"

char *dropBoxDir; 	//this vaiable contains the path to folder that contains all dropBox.
int dropBoxArrSize=0;	//number of dropBox in the server.
DROPBOX *glbDropBox;	//the array containing the DROPBOX structure for each DropBox.

void* serverConnectToDropBox(void *id);	
void *HandleNewUser(void *id);


//To handle installation of the Drop Box to new User.
void *startNewClientServer(void *data){

	if(!data){
		perror("Error in startNewClientServer\n");
		return NULL;
	}
	int portForNewUser = *(int *)data;
	printf("New User Server Started\n");
	if(serverConnection(portForNewUser, HandleNewUser)<0){
		perror("Error in startNewClient in serverConnection\n");
		return NULL;
	}
	return NULL;	
}

int main(){

	int portForNewUser, portForDropBox;
	pthread_t threadId;
	dropBoxDir = (char *)malloc(1024 *sizeof(char));	

	if(!dropBoxDir){
		perror("Error in allocationg space for dropBoxDir\n");
		return -1;
	}
	FILE *confFile;
	int size;
	char *strPortDropBox, *strPortNewUser, *strRootDir, *confStr;

	confFile=fopen("server.conf","r");
	
	if(!confFile){
		perror("Error in Opening the server.conf file\n");
		return -1;
	}

	if(fseek(confFile,0,SEEK_END)<0){
		perror("Error in fseek_END in Driver of Server\n");
		return -1;
	}

	size=ftell(confFile);
	if(size<0){
		perror("Error in ftell in Driver of server\n");
		return -1;
	}
	if(fseek(confFile,0,SEEK_SET)<0){
		perror("Error in fseek_SET in Driver of Server\n");
		return -1;
	}

	confStr=malloc(size*sizeof(char));
	if(!confStr){
		perror("Error in malloc confStr\n");
		return -1;

	}

	if(fread(confStr, 1, size, confFile)<=0){
		perror("Error in malloc confStr\n");
		return -1;
	}

	//read the root directory.
	strtok(confStr,"\n ");
	strRootDir=strtok(NULL,"\n ");
	//read the port to connect.
	strtok(NULL,"\n ");
	strPortDropBox=strtok(NULL,"\n ");
	strtok(NULL,"\n ");
	strPortNewUser=strtok(NULL,"\n ");
	if(!strRootDir || !strPortDropBox || !strPortNewUser){
		perror("any of the configuration is not given in sever.conf\n");
		return -1;
	}


	if(strPortDropBox==NULL||strPortNewUser==NULL||strRootDir==NULL){
		printf("Please provide Port for New User Server and Drop Box Server, with root directory of the server conf.\n");
		return 0;
	}

	portForNewUser = atoi(strPortNewUser);
	portForDropBox = atoi(strPortDropBox);
	
	if(portForNewUser<=0 || portForDropBox<=0 || portForNewUser>65535 || portForDropBox>65535 ){
		perror("Please give the valid Port in server.conf file\n");
		return -1;
	}

	strcpy(dropBoxDir, strRootDir);
	//create the child of this process. 
	//For simultaneous execution of server.

	//pid = fork();
	if(pthread_create( &threadId, NULL, startNewClientServer, &portForNewUser)!=0){
		printf("Error in thread creation\n");
		return -1;
	}
	printf("DropBox Connection Server Started\n");
	if(serverConnection(portForDropBox, serverConnectToDropBox)<0){
		perror("Error in server Connection of connecting user to DropBox");
		return -1;
	}
	return 0;
}
