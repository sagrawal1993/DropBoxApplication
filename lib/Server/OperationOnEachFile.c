/*
 	@author: Suraj Agrawal
	@Desciption: This file is the part of server's code.
	It contains the all the function that are apply for each data store in Trie.
 */

#include "Server/ServerCommon.h"
#include "Server/ServerHandleFileChange.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ForEachFileNewUserAtEmpty(void *trieData, void *myData){
	if(!trieData||!myData){
		perror("Error in ForEachFileNewUserAtEmpty\n");
		return;
	}
	FILEINFO *fileInfo = (FILEINFO *)trieData;
	int index=*(int *)myData;
	(fileInfo->flag)[index]|=1;
	return;
}
/*
void ForEachFileInfoBackStore(void *trieData, void *myData){
	FILEINFO *fileInfo = (FILEINFO *)trieData;
	FILE * fp = (FILE *)myData;

	if(fwrite((void *)fileInfo, sizeof(FILEINFO), )<0){
		printf("Error in writing\n");
		return 0;
	}

	return;
}
*/
void ForEachFileNewUserNonEmpty(void *trieData, void *myData){
	if(!trieData||!myData){
		perror("Error in ForEachFileNewUserNonEmpty\n");
		return;
	}

	FILEINFO *fileInfo = (FILEINFO *)trieData;
	void *temp;
	int tempPrevSize=*(int *)myData;
	int i;
	temp=malloc(2*tempPrevSize*sizeof(int));
	if(!temp){
		perror("Error in malloc in ForEachFileNewUserNonEmpty\n");
		return;
	}
	memcpy(temp,fileInfo->flag,tempPrevSize*sizeof(int));
	free(fileInfo->flag);
	fileInfo->flag=(char *)temp;

	//make all other dropBox invalid.
	for( i=tempPrevSize+1;i<2*tempPrevSize;i++){
		fileInfo->flag[i]=0;
	}
	(fileInfo->flag)[tempPrevSize]|=1;
	return;
}


void OperationOnEachFile(void *trieData, void *myData){
	if(!trieData||!myData){
		perror("Error in OperationOnEachFile\n");
		return;
	}

	//file info stored in the TRIE associated with the file.
	FILEINFO *fileInfo = (FILEINFO *)trieData;
	DATA*  data=(DATA *)myData;
	int index=data->machineId;
	char *flag=fileInfo->flag;
	HEADER header;
	
	header.dropBoxId = (data->dropBox)->dropBoxId;
	header.machineId= data->machineId;
	header.flag &=0;

	//find the machine need to change this file or not.
	//for(i=0;i<fileInfo->userUpdateArrSize;i++){
	//no change is require
	if(!((fileInfo->flag)[index]&&15) || ((data->dropBox)->userId)[index]==-1 ){
		return;
	}

	if(fileInfo->isDirectory==1){	//this is a file.
	
		if(isDeleted(flag[index])){
			sendUserFileDeletion(&header, data->connid, fileInfo->name, data->dropBox, index );
		}
		/*if(isMoved(flag[index])){	//for further scope
		
		//	sendUserFileMoved(&header, data->connid, fileInfo->moved, fileInfo->name, data->dropBox, index  );
		}*/
		if(isCreated(flag[index])){
		
			sendUserNewFileCreation(&header, data->connid, fileInfo->name, data->dropBox, index);
		}else if(isModify(flag[index])){
		
			sendUserFileModify(&header, data->connid, fileInfo->name, data->dropBox, index);
		}
	}else{
		if(isCreated(flag[index])){
	
			sendUserDirectoryCreate(&header, data->connid, fileInfo->name, data->dropBox, index);
		}else if(isDeleted(flag[index])){
	
			sendUserDirectoryRemoved(&header, data->connid, fileInfo->name, data->dropBox, index);
		}/*else if(isMoved(flag[index])){
			//need to be implemented.	
			//sendDirectoryMoved();
		}*/
	}


	return;	
}


