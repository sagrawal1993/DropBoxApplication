/***************************************************************************************************** 
*
*
*	@author:: Suraj Agrawal.
*	@Project:: DropBox for linux system.
*
*	Descriptioin:: This file contains the functions for transfering the file.
*				
*				SendFile() and ReceiveFile() are the function responsible for Sending and Receving the
*				File respectively. 
*
*				Current State: Transfer whole file.
*				Future Scope:	Can implement so that only the change in the file need to be transfer,
*							If the file modified.
* *****************************************************************************************************/


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "Common/FileTransfer.h"

extern int errno;


//To read data. 
int readBlock(int id, void *buffPtr, int byteToRead ){
	int byteRead=0;
	while(byteToRead>0){
			
		int flag=0;
		flag=read(id, buffPtr+byteRead, byteToRead);
	//	printf("flag:%d::::%s",flag,buffPtr+byteRead);

		if(flag<0){
			perror("Error in WriteBlock\n");
			return -1;
		}
		byteRead+=flag;
		byteToRead-=flag;
	}
	if(byteToRead<0){
		perror("Error byteTo Read < 0\n");
		return -1;
	}
	return byteRead;
}


//To write the data.
int writeBlock(int id, void *buffPtr, int byteToWrite ){
	int byteWritten=0;
	while(byteToWrite>0){
		int flag=0;
	//	printf("flag:%d:::%s",flag,buffPtr+byteWritten);
		flag=write(id, buffPtr+byteWritten, byteToWrite);
		if(flag<0){
			printf("Error in WriteBlock\n");
			return -1;
		}
		byteWritten+=flag;
		byteToWrite-=flag;
	}
	if(byteToWrite<0){
		perror("Error byte To Write <0\n");
		return -1;
	}
	return byteWritten;
}

// This will send the changed or newly created file to the user.
// take the absolute path of the file to be sendUser. 
// @arg1:: Socket Id of the connection.
// @arg1:: Name of the file to be send.
// return:: 	1	on success.
//			-1	on failure.

int sendFile(int connid, char *fileName){
	

	void *buff;
	int fd=open(fileName, O_RDONLY);
	struct stat fileStat;

	short int byteRead;
	int sizeOfFile;

	if(fd<0){
		perror("Error in opening the file In SendFile\n");
		return -1;
	}

	if(fstat(fd, &fileStat)<0){
		perror("Error in fstat\n");
		return -1;
	}
	sizeOfFile=fileStat.st_size;	//get the size of the file.

	buff=malloc(1024+sizeof(short int));	//each data packet contains the header that tells that it is header 
	if(!buff){
		perror("Error in getting the memory in Malloc\n");
		return -1;
	}
	while(sizeOfFile!=0){
	
		byteRead=sizeOfFile>1024?1024:sizeOfFile;
		sizeOfFile-=byteRead;

		if(readBlock(fd,buff,byteRead)<0){
		//if(byteRead<0){
			printf("Error in Reading the file in senduserFile\n");
			return -1;
		}
		//set the size at the first of the data stream.
	//	printf("\nByteRead:%d\n",byteRead);
	//	if(memcpy(buff,&byteRead,sizeof(short int))==NULL){
	//		perror("Error in memcpy in readFile Funciton\n");
	//		return -1;
	//	}	
		//printf("%s",(char *)(buff+sizeof(short int)));
	//	printf("in Write: %d\n",(short int*)buff);	
		if(write(connid,&byteRead, sizeof(short int))<0){
			printf("Error in write\n");
			return -1;
		}
		if(writeBlock(connid,buff, byteRead)<0){
			printf("Error in Writing the filo to the client\n");
			return -1;
		}
	}
	byteRead=0;	
	if(write(connid,&byteRead,sizeof(short int))!=sizeof(short int)){
		perror("Error in Writing last\n");
		return -1;
	}	
	free(buff);
	close(fd);
	return 1;
}


//this function will recive the file from the user.
//take the absolute path as an argument.
// @arg1:: Socket Id of the connection.
// @arg1:: Name of the file to be send.
// return:: 	1	on success.
//			-1	on failure.


int receiveFile(int connid, char *fileName){

	void *buff;
	char tempFileName[1024];
	printf("File Name: %s\n",fileName);
	strcpy(tempFileName,fileName);
	strcat(tempFileName,".tmp");
	int fd=open(tempFileName, O_CREAT | O_WRONLY, S_IRWXU | S_IRGRP | S_IXGRP  );
	//struct stat fileInfo;
	short int byteRead;

	//fstat(fd, &fileInfo);
	if(fd<0){
		perror("Error in opening file in Receive File \n");
		return -1;
	}
	buff=malloc(1024);	//each data packet contains the header that tells that it is header 
	if(buff==NULL){
		perror("Error in buff\n");
		return -1;
	}

	if(read(connid,&byteRead,sizeof(short int))!=sizeof(short int)){	//read the size of the containt send.
		perror("error in reading in readFile\n");
		return -1;
	}
	//printf("Recive File ByteRead:%d\n",byteRead);
	while(byteRead!=0){

		if(readBlock(connid,buff,byteRead)<0){
			printf("Error in Reading from socket in recive file.\n");
			return -1;
		}

		//printf("%s",(char *) buff);
		//write the file content into buffer.
		if(writeBlock(fd,buff,byteRead)<0){
			printf("Error in Writing the file to the client\n");
			printf("%d\n",errno);
			return -1;
		}	
		if(read(connid,&byteRead,sizeof(short int))!=sizeof(short int)){
			perror("Error in short int Read\n");
			return -1;
		}
	//	printf("Recive File ByteRead:%d\n",byteRead);

	}
	close(fd);
	free(buff);
	rename(tempFileName,fileName);
	return 1;

}

