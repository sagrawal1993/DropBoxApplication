/*
 	@author:: Suraj Agrawal
	 
 */



#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Server/ServerCommon.h"
#include "Server/OperationOnEachFile.h"
#include "Server/ServerHandleFileChange.h"
#include "Common/FileTransfer.h"

void startListeningEventAtClient(HEADER *header, int connid, DROPBOX *dropBox, int machineId);
void updateClientFiles(HEADER *header, int connid, DROPBOX* dropBox, int machineId);


//get the drop box structure belongs to particular Drop Box id.
DROPBOX *getDropBox(int dropBoxId){
	if(dropBoxId<0){
		perror("Negative dropBoxId in getDropBox\n");
		return NULL;
	}
	int i;
	for(i=0;i<dropBoxArrSize;i++){
		if((glbDropBox[i]).dropBoxId==dropBoxId && (glbDropBox[i]).dropBoxId!=-1){
			return &(glbDropBox[i]);
		}
	}
	return NULL;
}



//When client or machine start to connect to his dropBox.
//check if it is initialize packet or not.
//if not make error and return.
void *serverConnectToDropBox(void *id){

	if(!id){
		perror("Error in serverConnectToDropBox argument\n");
		return NULL;
	}
	int connid; 
	HEADER header;
	DROPBOX *dropBox;
	int i;
	connid =*((int *)id);
	int machineId;

	printf("dropBoxArrSize: %d\n",dropBoxArrSize);
	for( i=0;i<dropBoxArrSize;i++){
		printf("%d,",glbDropBox[i].dropBoxId);
	}

	//read the header from the client.
	if(readBlock(connid,&header, sizeof(HEADER))<0){
		perror("Not able to read the initialization Header.");
		close(connid);
		return NULL;
	}
	
	//if it is not initialization packet then it is not drop box user.
	if(!(isInitialization(&header))){
		perror("First packet is not the initialization packet.");
		close(connid);
		return NULL;
	}
	
	printf("After Initialization\n");
	//extract the dropBox id and machine id from the header.
	machineId=header.machineId;

	dropBox=getDropBox(header.dropBoxId);

	//For Handling exclusive user access.
	if(!dropBox){
		perror("getDropBox gives the Null\n");
		return NULL;
	}
	if(dropBox->currentUserId!=-1){
		printf("In If\n");
		resetFlag(&header);
		setExitCommand(&header);	
		if(writeBlock(connid,&header,sizeof(HEADER))<0){
			perror("Error in writing the exit command\n");
			return NULL;
		}
		return NULL;
	}else{
		printf("IN Else\n");
		dropBox->currentUserId=machineId; 
		if(writeBlock(connid,&header,sizeof(HEADER))<0){
			perror("Error in writing the exit command\n");
			return NULL;
		}
	}


	if(dropBox==NULL){
		printf("Null Return;");
	}
	else{
	printf("DropBox: %d, MachineId: %d\n",dropBox->dropBoxId,machineId);
	}
	
	//this will update all the changed/creted/deleted file or directory to the client.
	
	updateClientFiles(&header, connid, dropBox, machineId);

	printf("After Update Client File\n");
	//after synchroizing the file at the client, server will start listing the event
	//generted by inotify to traverse the change at client to the server.
	startListeningEventAtClient(&header, connid, dropBox, machineId);
	return NULL;
}


// it will read the header then check the command.
// according to command it will send the changed file.
// untill there is not file to send to the client.
// check updated file in the Data Structure and send to the client.

void updateClientFiles(HEADER *header, int connid, DROPBOX* dropBox, int machineId){
	
	if(!header || !dropBox || connid<0 || machineId<0){
		perror("Error in the passing argument of updateClientFiles\n");
		return ;
	}
	//call operation on the all file stored in trie.
	//this will send all the file need to the client denote by machineId.
	DATA data;
	data.dropBox = dropBox;
	data.machineId = machineId;
	data.connid = connid;
	printf("Before apply all operation\n");
	applyOperationOnAll(dropBox->trie,OperationOnEachFile, &data);
	
	printf("After apply all operation\n");
	//this will tell the client that no more file is there now.
	resetFlag(header);
	setNoMoreFile(header);
	printf("Before set no more file\n");
	if(writeBlock(connid, header, sizeof(HEADER))<0){
		perror("error in telling last update file to clinet\n");
	}
	printf("After no more file\n");
	return ;
}


// it will check the command and update the file at the server changed by the client.
void startListeningEventAtClient(HEADER *header, int connid, DROPBOX *dropBox, int machineId){
	
	if(!header || !dropBox || machineId<0 || connid<0){
		perror("Error in argument of startListeningEventAtClint\n");
		return ;
	}

	char name[1024];
	
	printf("Before Read\n");
	if(readBlock(connid, header, sizeof(HEADER))<0){
		perror("Not able to read the first header from the client.\n");
		return ;
	}
	//check if there is no more file to get for update.
	while(!isExitCommand((header))){
		
		printf("before size\n");
		int size = header->size;	//get the size of the file.
		
		printf("In while\n");
		//read the control info for the file.
		//read the name of the file.
		if(readBlock(connid, name, size)<0){
			perror("Not able to read the file name in while");
			return ;
		}	
		
		name[size]='\0';
		printf("%s\n",name);
		printf("Before Command check\n");	
		//check what operation has been occured.
		if(isNewFileCreated(header)){
			printf("In File Created\n");
			receiveUserNewFileCreation(connid, name, dropBox, machineId);
		}else if(isFileDeleted(header)){
			printf("In File Deleted\n");
			receiveUserFileDeletion(connid, name, dropBox, machineId);
		}/*else if(isFileMoved(header)){

			char *temp=&(name[strlen(name)+1]);
			receiveUserFileMoved(connid,name, temp, dropBox, machineId);	
		}*/else if(isFileModified(header)){
			printf("In file modified\n");
			receiveUserFileModify(connid, name, dropBox, machineId);
		}else if(isDirectoryCreated(header)){
			printf("In Directory Created\n");
			receiveUserDirectoryCreate(connid, name, dropBox, machineId);
		}/*else if(isDirectoryMoved(header)){
		   	printf("moved\n");
			char *temp=&(name[strlen(name)+1]);
			receiveServerDirectoryMoved(connid, name, temp, dropBox);
		}*/else if(isDirectoryDeleted(header)){
			printf("Directory Deleted\n");
			receiveUserDirectoryRemoved(connid, name, dropBox, machineId);
		}else{
			perror("Something went wrong in file command.\n");
		}


		printf("After command check\n");
		//after receiving the whole file, check if more file is there.
		if(readBlock(connid, header, sizeof(HEADER))<0){
			perror("Not to able to read the first header from the server.\n");
			return ;
		}
	}
	printf("Setting CurrentUserId to -1\n");
	//when the user Exit from Drop Box other user can enter in it.
	dropBox->currentUserId=-1;
	return;
}
