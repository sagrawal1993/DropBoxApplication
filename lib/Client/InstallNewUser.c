/*****************************************************************************************************
 *
 *
 *	
 *
 *	Description:: This file contains the code for installation of dropBox.
 *				dropBox.conf file will contains all the user's details on this machine.
 *
 *
 *
 *
 ***************************************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Common/Connection.h"
#include "Common/FileTransfer.h"

//check if the directory is empty or not.
static int isDirectoryEmpty(char *fileName){
	DIR *dir= opendir(fileName);
	struct dirent *entry;

	if(dir!=NULL){
		entry=readdir(dir);
		while(entry){
			if( strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
				printf("DIRECTORY IS NOT EMPTY\nEXECUTION ABORTED\n");
				return 0;
			}
			entry=readdir(dir);
		}
		return 1;
 	}else if(errno==ENOTDIR){
		printf("PATH IS NOT DIRECTORY\n");
		return 0;
	}
	return 0;
}

//this function will run only once and will get the userId, dropBox Id and other information and store them in a file.

int install(int flag, int dropBox, int port ,char *ip){
	
	//FILE *infoFile;
	int infoFile;
	char directoryPath[1024];
	if((flag!=0 && flag!=1)|| !ip){
		perror("Error in argument of install\n");
		return -1;
	}
	int newConnId;
	//int machineId;
	
	int buff[3];	
	printf("Enter the Directory with absolute Path for DropBox\n");
	scanf("%s",directoryPath);
	
	if(!isDirectoryEmpty(directoryPath)){
		return -1;
	}	
	newConnId = clientConnection( port,ip); 

	if(newConnId<0){
		perror("Error in getting connection Id\n");
		return -1;
	}
	buff[0]=flag;
	if(flag==1){
		buff[1]=dropBox;
	}
	//send the command accordingly
	if(write(newConnId, buff, 2*sizeof(int))!=(2*sizeof(int))){
		perror("Error in writing \n");
	}	

	//read the dropBox id and machine id, for this installation.
	if(read(newConnId, buff,sizeof(int)*2)!=(2*sizeof(int))){
		perror("Error in Reading the installation time DropBox id and Machind Id\n");
	}

	if(buff[0]==-1){
		printf("Wrong DropBox Id is provided\n");
		return -1;
	}
	
	int index = strlen(directoryPath);		// store  the directory path with '/' at the end.
	//directoryPath[index]='/';
	directoryPath[index]='\0';
	//dropBox=buff[0];
	//machineId=buff[1];
	buff[2]=index+1;
	//infoFile=fopen("dropBox.conf","a+");	//the new dropBox user is added just after old entry.
	infoFile = open("dropBox.conf", O_CREAT |O_WRONLY, S_IRWXU|S_IRGRP);
	if(infoFile<0){
		printf("Not able to open new file\n");
		return -1;
	}
	if(write(infoFile, buff, 3*sizeof(int))!=(3*sizeof(int))){
		printf("Not able to write\n");
		return -1;
	}
	if(writeBlock(infoFile, directoryPath,buff[2]*sizeof(char))<0){
		printf("Not able to write\n");
		return -1;
	}
	//fprintf(infoFile,"%d%d%s",dropBox,machineId,directoryPath);
	printf("Till End\n");
	return 0;
}

