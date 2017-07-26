/*****************************************************************************************************
 *
 *
 *		
 *
 *
 *		Description:: Driver for starting Backend process for all the dropBox on this machine.
 *
 *
 *
 *
 ****************************************************************************************************/ 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include "Common/Connection.h"
#include "Common/CommandHeader.h"

int ClientConnectToDropBox(int dropBoxId, int machineId, int connid, char *rootDirectory);
int id;

void exitDropBox(int sig){

	HEADER header;
	resetFlag(&header);
	if((sig && SIGTSTP)||(sig && SIGABRT )||(sig && SIGTERM)){
		setExitCommand(&header);
		if(write(id, &header, sizeof(HEADER))<0){
			perror("Error in send the command \n");
		}
		printf("Disconnecting to DropBox\n");
		exit(1);
	}
	return;
}

int main(int argc, char **argv){

	int fp;
	int port, connid;
	char rootDirectory[1024];
	char *confStr;
	int buff[3];

	FILE *confFile;
	int size;
	char *strPort, *strIp;
	//long unsigned int thread_id;

	confFile=fopen("client.conf","r");
	if(confFile==NULL){
		perror("error in opening client.conf\n");
		return -1;
	}
	if(fseek(confFile,0,SEEK_END)<0){
		perror("Error in fseek_end\n");
		return -1;
	}
	size=ftell(confFile);
	if(size<0){
		perror("Error in ftell");
		return -1;
	}
	if(fseek(confFile,0,SEEK_SET)<0){
		perror("Error in fseek_set");
		return -1;
	}
	confStr=malloc(size*sizeof(char));
	if(confStr==NULL){
		perror("error in allocating memory Malloc\n");
		return -1;
	}
	if(fread(confStr, 1, size, confFile)<0){
		perror("error in Reading \n");
		return -1;
	}
	
	//read the IP.
	strtok(confStr,"\n ");
	strIp=strtok(NULL,"\n ");
	//read the port to connect.
	strtok(NULL,"\n ");
	strPort=strtok(NULL,"\n ");

	if(strPort==NULL || strIp==NULL){
		printf("Provide the ip and port to connect to the server in the Conf file\n");
		return 0;
	}
	port = atoi(strPort);

	if(port<=0||port>65535){
		perror("Not a valid port\n");
		return -1;
	}

	fp = open("dropBox.conf",O_RDONLY);
	if(fp<0){
		perror("error in opening file dropBox.conf\n");
		return -1;
	}
	if(read(fp,buff,sizeof(int) *3)!=sizeof(int)*3){
		perror("Error in reading from file \n");
		return -1;
	}
	if(read(fp,rootDirectory,buff[2])!=buff[2]){
		perror("Error in reading the directory of the clientfrom dropBox.conf");
		return -1;
	}
	printf("dropBoxId: %d, MachineId: %d, rootDirectory: %s",buff[0], buff[1], rootDirectory);
	close(fp);
	connid = clientConnection(port, strIp);
	if(connid<=0){
		printf("Error in getting client connection\n");
		return -1;
	}
	id=connid;
	if(signal(SIGINT,exitDropBox)==SIG_ERR){
		perror("Error in applying the signal\n");
		return -1;
	}

	if(ClientConnectToDropBox(buff[0], buff[1], connid, rootDirectory)==-1){
		return -1;
	}	
	return 1;
}
