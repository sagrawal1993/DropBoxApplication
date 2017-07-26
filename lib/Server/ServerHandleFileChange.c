/*****************************************************************************************************
 *
 *	This file contains the function that handle the change in the file, and Traverse the change to the user.
 *
 *
 *
 *****************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Server/ServerHandleFileChange.h"
#include "Common/FileTransfer.h"
#include "Common/TRIE.h"



//sendUser the command, while the command is set and give the name of the file for which 
//the command is and also the size of the Name string according to command.
static void sendUserCommand(HEADER *header, int connid, char *Name, int size){
	
	
	if(!header || !Name || connid<0 || size <0){
		perror("Error in sendUserCommand arguments\n");
		return;
	}

	header->size=size;
	if(writeBlock(connid,header,sizeof(HEADER))<0){
		perror("Not Able to sendUser the command for new file created\n");
		return;
	}
	//tell the user that the new file is being created.
	//set the size of the fileName.
//	Name[size]='\0';
	//sendUser the fileName
	if(writeBlock(connid,Name,size)<0){
		perror("Not able to sendUser the fileName\n");
		return;
	}

	return;
}



//tell that a particular file is being created.
void sendUserNewFileCreation(HEADER *header, int connid, char *fileName, DROPBOX *dropBox, int userId){ 	//new file is created.
	
	if(!header || !fileName||!dropBox || connid<0 || userId <0){
		perror("Error in sendUserNewFileCreation arguments\n");
		return;
	}


	char tempName[1024];
	char dropBoxName[32];
	FILEINFO *fileInfo;

	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	header->flag &=0;
	resetFlag(header);
	setNewFileCreated(header);	//macro
	sendUserCommand(header, connid, fileName, strlen(fileName));
	
	strcpy(tempName, dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName, fileName);
	
	fileInfo=searchInTrie(dropBox->trie, fileName);
	if(!fileInfo){
		perror("Error in SendUserNewFileCreated, but no entry in Trie\n");
		return;
	}
	(fileInfo->flag)[userId] &= 0;
	sendFile(connid, tempName);	//tempName contains the concatinated name of the file send to user.
	return;
}
	

//Tell that a particular file is being deleted.
void sendUserFileDeletion(HEADER *header,int connid, char *fileName, DROPBOX *dropBox, int userId){	//file is deleted.
	//tell the name of the file to the user which file is being deleted.
	
	if(!header || !fileName||!dropBox || connid<0 || userId <0){
		perror("Error in sendUserFileDeletion arguments\n");
		return;
	}


	FILEINFO *fileInfo;
	resetFlag(header);
	setFileDeleted(header);
	fileInfo=searchInTrie(dropBox->trie, fileName);
	if(!fileInfo){
		perror("Error in SendUserFileDeletion, but no entry in Trie\n");
		return;
	}

	(fileInfo->flag)[userId] &=0;
	sendUserCommand(header, connid, fileName, strlen(fileName));
	return;
}	
	
//Tell that the file is being modified.
void sendUserFileModify(HEADER *header,int connid, char *fileName, DROPBOX *dropBox, int userId){
	if(!header || !fileName||!dropBox || connid<0 || userId <0){
		perror("Error in sendUserFileModify arguments\n");
		return;
	}



	//tell the user that the file is modified.
	char tempName[1024];
	char dropBoxName[32];
	FILEINFO *fileInfo;

	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	resetFlag(header);
	setFileModified(header);
	sendUserCommand(header, connid, fileName, strlen(fileName));

	strcpy(tempName, dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName, fileName);

	fileInfo=searchInTrie(dropBox->trie, fileName);
	if(!fileInfo){
		perror("Error in SendUserFileModify, but no entry in Trie\n");
		return;
	}

	(fileInfo->flag)[userId]&=0;
	sendFile(connid,tempName);
	return;
}

/*

//Tell that the file has been moved.
//consider that fileInfo structure is stored at destination.
void sendUserFileMoved(HEADER *header, int connid, char *source, char *destination, DROPBOX *dropBox, int userId){
	
	//tell the user that the particualar file is being moved.
	//tell the user the source and the destination address of the file moved.
	
	int len1,size;
	FILEINFO *fileInfo;
	char temp[1024];
	len1=strlen(source);
	size = strlen(destination)+len1+1;
	strcpy(temp,source);
	temp[len1]='\0';
	strcpy(&(temp[len1+1]),destination);
	
	fileInfo = searchInTrie(dropBox->trie, destination);
	(fileInfo->flag)[userId]&=3;	//after move creation or deletion is only allowed.
	resetFlag(header);
	setFileMoved(header);
	sendUserCommand(header, connid, temp, size);
	return;

}

*/


void sendUserDirectoryCreate(HEADER *header,int connid, char *dirPath, DROPBOX *dropBox, int userId){

	if(!header || !dirPath||!dropBox || connid<0 || userId <0){
		perror("Error in sendUserDirectoryCreate arguments\n");
		return;
	}


	FILEINFO *fileInfo;
	
	//tell the name of the directory created.
	resetFlag(header);
	setDirectoryCreated(header);
	sendUserCommand(header,connid, dirPath, strlen(dirPath));
	fileInfo = searchInTrie(dropBox->trie, dirPath);
	if(!fileInfo){
		perror("Error in SendUserDirectoryCreate, but no entry in Trie\n");
		return;
	}

	(fileInfo->flag)[userId]&=0;
	return;
}
	

/*
//Directory Move need to be handle.
//Need to be handle.
void sendUserDirectoryMoved(HEADER *header, int connid, char *source, char *destination){

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
	sendUserCommand(header, connid, temp, size);
	return;	
}
*/	
	

void sendUserDirectoryRemoved(HEADER *header, int connid, char *dirPath, DROPBOX *dropBox, int userId){
	
	if(!header || !dirPath||!dropBox || connid<0 || userId <0){
		perror("Error in sendUserDirectoryRemoved arguments\n");
		return;
	}


	FILEINFO *fileInfo;
	//tell the User the name of directory that is being deleted.
	resetFlag(header);
	setDirectoryDeleted(header);	
	sendUserCommand(header, connid, dirPath, strlen(dirPath));
	fileInfo = searchInTrie(dropBox->trie, dirPath);
	if(!fileInfo){
		perror("Error in SendUserDirectoryRemoved, but no entry in Trie\n");
		return;
	}

	(fileInfo->flag)[userId]&=0;
	return;
}
	
	
void receiveUserNewFileCreation(int connid, char *fileName, DROPBOX *dropBox, int userId){ 	//new file is created.

	if(!fileName||!dropBox || connid<0 || userId <0){
		perror("Error in receiveUserFileCreation arguments\n");
		return;
	}
	char tempName[1024];
	char dropBoxName[32];
	int i;

	FILEINFO *fileInfo = (FILEINFO *)malloc(sizeof(FILEINFO));
	fileInfo->name = (char *)malloc(sizeof(char)*(strlen(fileName)+1));
	
	if(!fileInfo->name){
		perror("Error in allocating memory receiveUserNewFileCreation\n");
		return;
	}
	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	
	strcpy(tempName,dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName,fileName);
	
	strcpy(fileInfo->name,fileName);
	fileInfo->isDirectory=1;
	fileInfo->flag=(char *)malloc(sizeof(char)*dropBox->userArrSize);
	if(!fileInfo->flag){
		perror("Error in malloc in receiveUserNewFileCreation\n");
		return;
	}
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userId){
			(fileInfo->flag)[i]&=0;
			(fileInfo->flag)[i]|=1;	//set the LSB for ith user.
		}
	}
	
	insertInTrie(dropBox->trie,fileName,fileInfo );	//Enter New Created file into TRIE. 

	receiveFile(connid, tempName);
}
	

void receiveUserFileDeletion(int connid, char *fileName, DROPBOX *dropBox, int userId){	//file is deleted.
	//tell the name of the file to the User which file is being deleted.
	if(!fileName||!dropBox || connid<0 || userId <0){
		perror("Error in receiveUserFileDeletion arguments\n");
		return;
	}
	
	char tempName[1024];
	char dropBoxName[32];
	FILEINFO *fileInfo;
	int i;

	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	
	strcpy(tempName,dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName, "/");
	strcat(tempName,fileName);

	if(access(tempName,F_OK)==-1){
		return;
	}
	fileInfo=(FILEINFO *)searchInTrie(dropBox->trie,fileName);

	if(!fileInfo){
		perror("Error in searchInTrie in receiveUserFileDeletion\n");
		return;
	}
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userId){
			(fileInfo->flag)[i]|=8;	//set the 3rd for ith user that tell that file is being deleted.
		}
	}
	/*
	 *	Will not delete the file Name from the Trie so that other users can delete that file too. 
	 */
	if(remove(tempName)<0){
		perror("Error in removing the file\n");
	}
	return;
}	
	

void receiveUserFileModify(int connid, char *fileName, DROPBOX *dropBox, int userId){
	//tell the User that the file is modified.
	if(!fileName || connid<0 || !dropBox || userId<0){
		perror("error in receiveUserFileModify Argument\n");
		return ;
	}

	char tempName[1024];
	char dropBoxName[32];
	FILEINFO *fileInfo;
	int i;

	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	
	strcpy(tempName,dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName,fileName);
	
	fileInfo=(FILEINFO *)searchInTrie(dropBox->trie,fileName);

	if(!fileInfo){
		perror("error in search in Trie in receiveUserFileModify\n");
		return;
	}
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userId){
			(fileInfo->flag)[i]|=2;	//set the 3rd for ith user that tell that file is being deleted.
		}
	}

	receiveFile(connid,tempName);
	return;
}
/*
//this has the problem when a user make two move of the same file while some user in between connect to the dropBox while some not.
//this will lead to inconsistency.
	
void receiveUserFileMoved(int connid, char *source, char *destination, DROPBOX *dropBox, int userId){
	//tell the User that the particualar file is being moved.
	//tell the User the source and the destination address of the file moved.
	char temp1[1024],temp2[1024];
	char dropBoxName[32];
	int i;
	FILEINFO *fileInfo;

	sprintf(dropBoxName,"%d",dropBox->dropBoxId);
	
	strcpy(temp1,rootDirectory);
	strcat(temp1,dropBoxName);
	strcat(temp1,source);

	strcpy(temp2,rootDirectory);
	strcat(temp2,dropBoxName);
	strcat(temp2,destination);

	
	fileInfo=(FILEINFO *)searchInTrie(dropBox->trie,source);
	fileInfo->moved=name;
	fileInfo->name=(char *)malloc(sizeof(char)*(strlen(destination)+1));
	strcpy(fileInfo->name, destination);
	deleteInTrie(dropBox->trie, source);
	insertInTrie(dropBox->trie, destination, fileInfo);
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userid){
			(fileInfo->flag)[i]|=4;	//set the 3rd for ith user that tell that file is being deleted.
		}
	}

	rename(source, destination);
	return;
}

*/
void receiveUserDirectoryCreate(int connid, char *dirPath, DROPBOX *dropBox, int userId){

	if(connid<0||!dirPath||!dropBox||userId<0){
		perror("Error in receiveUserDirectoryCreate arguments\n");
		return;
	}
	//tell the name of the directory created.
	char tempName[1024];
	char dropBoxName[32];
	int i;

	if(!dropBox){
		printf("Error DropBox NULL\n");
		return;
	}
	printf("DropBOx ID: %d\n",dropBox->dropBoxId);
	printf("Before\n");
	FILEINFO *fileInfo = (FILEINFO *)malloc(sizeof(FILEINFO));
	printf("In Recive IN btween\n");
	fileInfo->name = (char *)malloc(sizeof(char)*(strlen(dirPath)+1));
	//printf("After\n");

	if(!fileInfo || ! fileInfo->name){
		perror("Error of malloc in receiveUserDirectoryCreate\n");
		return;
	}
	printf("DropBox Dir: %s", dropBoxDir);
	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);

	printf("DropBox Name: %s",dropBoxName);
	printf("DropBox Dir: %s", dropBoxDir);
	strcpy(tempName,dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName,dirPath);
	
	strcpy(fileInfo->name,dirPath);
	fileInfo->isDirectory=0;
	fileInfo->flag= (char *)malloc(sizeof(char)* dropBox->userArrSize);
	if(!fileInfo->flag){
		perror("error malloc in receiveUserDirectory:2\n ");
		return;
	}
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userId){
			(fileInfo->flag)[i]&=0;
			(fileInfo->flag)[i]|=1;	//set the LSB for ith user.
		}
	}
	
	insertInTrie(dropBox->trie,dirPath,fileInfo );	//Enter New Created file into TRIE. 
	
	if(mkdir(tempName,S_IFDIR| S_IRWXU)<0){
		perror("Error in creating directory\n");
	}
	return;
}
/*	
// 	move the directory specified.	
//	still Left to be implementation.

void receiveUserDirectoryMoved(int connid, char *source, char *destination, DROPBOX *dropBox, int userId){
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
void receiveUserDirectoryRemoved(int connid, char *dirPath, DROPBOX *dropBox, int userId){
	if(connid<0||!dirPath||!dropBox||userId<0){
		perror("Error in receiveUserDirectoryCreate arguments\n");
		return;
	}

	//tell the User the name of directory that is being deleted.
	char tempName[1024];
	char dropBoxName[32];
	FILEINFO *fileInfo;
	int i;

	sprintf(dropBoxName,"%d/",dropBox->dropBoxId);
	
	strcpy(tempName,dropBoxDir);
	strcat(tempName,dropBoxName);
	//strcat(tempName,"/");
	strcat(tempName,dirPath);
	
	fileInfo=(FILEINFO *)searchInTrie(dropBox->trie,dirPath);
	if(!fileInfo){
		perror("Error in search in tree in directory removed\n");
		return;
	}
	for(i=0;i<dropBox->userArrSize;i++){
		if(i!=userId){
			(fileInfo->flag)[i]|=8;	//set the 3rd for ith user that tell that file is being deleted.
		}
	}
	/*
	 *	Will not delete the file Name from the Trie so that other users can delete that file too. 
	 */

	if(rmdir(tempName)<0){
		perror("error in Removing Directory\n");
	}	
	return;

}
