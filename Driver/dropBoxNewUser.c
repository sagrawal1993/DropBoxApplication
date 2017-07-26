/*****************************************************************************************************
*
*
*
*		Description:: This file contains the main for installation of new DropBox User.
*
*
*
*
*
*****************************************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int install(int flag, int dropBoxId, int port, char *ip);

int main(int argc, char **argv){

	int port;
	int flag, dropBoxId;

	FILE *confFile;
	int size;
	char *strPort, *strIp, *confStr;

	confFile=fopen("client.conf","r");
	if(confFile==NULL){
		perror("Error in opening client.conf File\n");
		return -1;
	}
	if(fseek(confFile,0,SEEK_END)==-1){
		perror("error in Fseek_END\n");
		return -1;
	}
	size=ftell(confFile);
	if(size<0){
		perror("Error in ftell\n");
		return -1;
	}
	if(fseek(confFile,0,SEEK_SET)==-1){
		perror("Error in Fseek_SET\n");
		return -1;
	}
	confStr=malloc(size*sizeof(char));
	if(confStr==NULL){
		perror("Error in malloc\n");
		return -1;
	}
	if(fread(confStr, 1, size, confFile)<=0){
		perror("Error in reading from file\n");
		return -1;
	
	}
	
	//read the IP.
	strtok(confStr,"\n ");
	strIp=strtok(NULL,"\n ");
	//read the port to connect.
	strtok(NULL,"\n ");
	strtok(NULL,"\n ");
	strtok(NULL,"\n ");
	strPort=strtok(NULL,"\n ");


	if(strPort==NULL || strIp==NULL){
		printf("Provide the port and ip in configuration File\n");
		return -1;
	}
	
	port = atoi(strPort);
	
	if(port<=0||port>65535){
		perror("Error in the port number given\n");
		return -1;
	}
	printf("Want to make new DropBox[y/n] ");
	if(getchar()=='y'){
		flag=0;
		dropBoxId = -1;
	}else{
		printf("Enter DropBox Id: ");
		scanf("%d",&dropBoxId);
		flag=1;
	}

	printf("%d,%d,%d,%s",flag,dropBoxId,port,strIp);	
	if(install(flag, dropBoxId, port, strIp)<0){
		perror("Error in Install\n");
		return -1;
	}
	return 0;
}
