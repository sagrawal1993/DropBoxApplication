/*****************************************************************************************************
 *
 *	This file contains the function that handle the change at the client side and Traverse the change to the server.
 *	
 *	Define the function for send or receive the files and directory with the following changes to the server.
 * 	Consider only one drop box at a machine.
 *	
 *	
 *
 *****************************************************************************************************/

#include "Common/FileTransfer.h"
#include "Common/CommandHeader.h"
#include "Client/HandleFileChange.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//sendServer the command, while the command is set and give the name of the file for which 
//the command is and also the size of the Name string according to command.
static void sendServerCommand(HEADER *header, int connid, char *Name, int size){
	
	
	header->size=size;
	if(writeBlock(connid,header,sizeof(HEADER))<0){
		perror("Not Able to sendServer the command for new file created\n");
		return;
	}
	//tell the server that the new file is being created.
	//set the size of the fileName.
	//Name[size]='\0';
	//sendServer the fileName
	printf("In send command: %s\n",Name);
	if(writeBlock(connid,Name,size)<0){
		perror("Not able to sendServer the fileName\n");
		return;
	}

	return;
}



//tell that a particular file is being created.
void sendServerNewFileCreation(HEADER *header, int connid, char *fileName, char *rootDirectory){ 	//new file is created.

	char tempName[1024];
	resetFlag(header);
	setNewFileCreated(header);	//macro
	sendServerCommand(header, connid, fileName, strlen(fileName));

	strcpy(tempName, rootDirectory);
	strcat(tempName,"/");
	strcat(tempName, fileName);
	sendFile(connid, tempName);	//tempName contains the concatinated name of the file send to server.
	return;
}
	

//Tell that a particular file is being deleted.
void sendServerFileDeletion(HEADER *header,int connid, char *fileName){	//file is deleted.
	//tell the name of the file to the server which file is being deleted.
	resetFlag(header);
	setFileDeleted(header);
	sendServerCommand(header, connid, fileName, strlen(fileName));
	return;
}	
	
//Tell that the file is being modified.
void sendServerFileModify(HEADER *header,int connid, char *fileName, char *rootDirectory ){
	//tell the server that the file is modified.
	char tempName[1024];
	
	resetFlag(header);
	setFileModified(header);
	sendServerCommand(header, connid, fileName, strlen(fileName));

	strcpy(tempName, rootDirectory);
	strcat(tempName,"/");
	strcat(tempName, fileName);
	
	sendFile(connid,tempName);
	return;
}

/*

//Tell that the file has been moved.
void sendServerFileMoved(HEADER *header, int connid, char *source, char *destination){
	//tell the server that the particualar file is being moved.
	//tell the server the source and the destination address of the file moved.
	
	int len1,size;
	char temp[1024];
	len1=strlen(source);
	size = strlen(destination)+len1+1;
	strcpy(temp,source);
	temp[len1]='\0';
	strcpy(&(temp[len1+1]),destination);
	resetFlag(header);
	setFileMoved(header);
	sendServerCommand(header, connid, temp, size);
	return;
}
*/
	
void sendServerDirectoryCreate(HEADER *header,int connid, char *dirPath){

	//tell the name of the directory created.
	resetFlag(header);
	setDirectoryCreated(header);
	sendServerCommand(header,connid, dirPath, strlen(dirPath));
	return;
}

/*
	
void sendServerDirectoryMoved(HEADER *header, int connid, char *source, char *destination){

	//tell the moved directory source and destination.
	int len1,size;
	char temp[1024];
	len1=strlen(source);
	size = strlen(destination)+len1+1;
	strcpy(temp,source);
	temp[len1]='\0';
	strcpy(&(temp[len1+1]),destination);
	resetFlag(header);
	setDirectoryMoved(header);
	sendServerCommand(header, connid, temp, size);
	return;	
}
	
*/

void sendServerDirectoryRemoved(HEADER *header, int connid, char *dirPath){
	
	//tell the server the name of directory that is being deleted.
	resetFlag(header);
	setDirectoryDeleted(header);	
	sendServerCommand(header, connid, dirPath, strlen(dirPath));
	return;
}
	
	
void receiveServerNewFileCreation(int connid, char *fileName, char *rootDirectory){ 	//new file is created.

	char tempName[1024];
	strcpy(tempName,rootDirectory);
	strcat(tempName, "/");
	strcat(tempName,fileName);

	//tell the server that the new file is being created.
	if(receiveFile(connid, tempName)<0){
		perror("Error in receiveServerNewFileCreation\n");
		return;
	}
}
	

void receiveServerFileDeletion(int connid, char *fileName, char *rootDirectory){	//file is deleted.
	//tell the name of the file to the server which file is being deleted.
	char temp[1024];
	strcpy(temp, rootDirectory);
	strcat(temp,"/");
	strcat(temp, fileName);
	if(remove(temp)<0){
		perror("Not able to delete file\n");
	}
	return;
}	
	

void receiveServerFileModify(int connid, char *fileName, char *rootDirectory){
	//tell the server that the file is modified.
	char temp[1024];
	strcpy(temp, rootDirectory);
	strcat(temp, "/");
	strcat(temp, fileName);
	receiveFile(connid,temp);
	return;
}

/*
	
void receiveServerFileMoved(int connid, char *source, char *destination){
	//tell the server that the particualar file is being moved.
	//tell the server the source and the destination address of the file moved.
	char temp1[1024],temp2[1024];
	strcpy(temp1,rootDirectory);
	strcat(temp1,source);
	strcpy(temp2,rootDirectory);
	strcat(temp2,destination);
	rename(source, temp2);
	return;
}
*/

void receiveServerDirectoryCreate(int connid, char *dirPath, char *rootDirectory){

	//tell the name of the directory created.
	char temp[1024];
	strcpy(temp, rootDirectory);
	strcat(temp, "/");
	strcat(temp, dirPath);
	if(mkdir(temp,S_IFDIR | S_IRWXU )<0){
		perror("Error in making directory\n");
	}	
	return;
}

/*
// 	move the directory specified.	
void receiveServerDirectoryMoved(int connid, char *source, char *destination){
	char temp1[1024],temp2[1024];
	strcpy(temp1,rootDirectory);
	strcat(temp1,source);
	strcpy(temp2,rootDirectory);
	strcat(temp2,destination);
	rename(source, destination);
	
	//tell the moved directory source and destination.
	return;
}
	
*/

//	remove the directory specified.
void receiveServerDirectoryRemoved(int connid, char *dirPath, char *rootDirectory){
		
	//tell the server the name of directory that is being deleted.
	char temp[1024];
	strcpy(temp, rootDirectory);
	strcat(temp,"/");
	strcat(temp, dirPath);
	if(rmdir(temp)<0){
		perror("error in removing Directory\n");
	}	
	return;

}

