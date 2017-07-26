/*****************************************************************************************************
 *
 *		This will contains the Data Structure for drop Box.
 * 		Drop Box Id are Continous.
 *
 *
 *
 *
 *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Common/TRIE.h"
#include "Server/ServerCommon.h"
#include "Server/OperationOnEachFile.h"
#include "Common/FileTransfer.h"


//Give the new DropBox for New User.
int getNewDropBoxID(){
//int i of first for loop is removed .	
	void *temp;
	int i,j;
	//first check if any Drop Box become invalid.
	if(dropBoxArrSize==0){
		temp=malloc(4*sizeof(DROPBOX));
		if(!temp){
			perror("error in malloc in getNewDropBoxId\n");
			return -1;
		}
		glbDropBox=(DROPBOX *)temp;
		dropBoxArrSize=4;
		for( i=1;i<4;i++){
			glbDropBox[i].dropBoxId=-1;
		}
		glbDropBox[0].dropBoxId = 0;
		glbDropBox[0].userId=(int *)malloc(4*sizeof(int));
		glbDropBox[0].userArrSize=4;
		for(j=0;j<4;j++){
			(glbDropBox[0]).userId[j]=-1;
		}
		glbDropBox[0].trie=getTrie();
		return 0;
	}

	for(i=0;i<dropBoxArrSize;i++){
		if(glbDropBox[i].dropBoxId==-1){
			glbDropBox[i].dropBoxId =i;
			glbDropBox[i].userId=(int *)malloc(4*sizeof(int));
			if(!glbDropBox[i].userId){
				perror("Error in melloc in getNewDropBoxId In For\n");
				return -1;
			}

			glbDropBox[i].userArrSize=4;
			for(j=0;j<4;j++){
				(glbDropBox[i]).userId[j]=-1;
			}
			glbDropBox[i].trie=getTrie();
			if(!glbDropBox[i].trie){
				perror("Error in getNewDropBox in getTrie\n");
				return -1;
			}
			return i;
		}
	}

	int tempPrevSize=dropBoxArrSize;

	temp=malloc(2*dropBoxArrSize*sizeof(DROPBOX));
	if(!temp){
		perror("Error in malloc in getNewDropBox :3rd Malloc\n");
		return -1;
	}
	memcpy(temp,glbDropBox,dropBoxArrSize*sizeof(DROPBOX));
	free(glbDropBox);
	glbDropBox=(DROPBOX *)temp;
	dropBoxArrSize=2*dropBoxArrSize;	

	//make all other dropBox invalid.
	for(i=tempPrevSize+1;i<dropBoxArrSize;i++){
		glbDropBox[i].dropBoxId=-1;
	}
	glbDropBox[tempPrevSize].dropBoxId =tempPrevSize;
	glbDropBox[tempPrevSize].userId=(int *)malloc(4*sizeof(int));
	glbDropBox[tempPrevSize].userArrSize=4;
	for(j=0;j<4;j++){
		(glbDropBox[tempPrevSize]).userId[j]=-1;
	}
	glbDropBox[tempPrevSize].trie=getTrie();
	if(!glbDropBox[tempPrevSize].trie){
		perror("error in getNewDropBox getTrie:2\n");
		return -1;
	}	
	return tempPrevSize;
}



//get new User of the DropBox denoted by DropBoxID.
int getNewUserOfDropBox(int DropBoxId){
	
	if(DropBoxId<0){
		perror("Error in getNewUserOfDropBox\n");
		return -1;
	}	

	void *temp;
	int userArrSize=glbDropBox[DropBoxId].userArrSize;
	
	if(DropBoxId>=dropBoxArrSize || glbDropBox[DropBoxId].dropBoxId!=DropBoxId){
		perror("Error in getNewUserOfDropBox \n");
		return -1;
	}

	/*	if(userArrSize==0){
		temp=malloc(4*sizeof(int));
		glbDropBox[DropBoxId].userId=(int *)temp;
		glbDropBox[DropBoxId].userArrSize=4;
		(glbDropBox[DropBoxId]).userId[0]=1;
		for(int i=1;i<4;i++){
			(glbDropBox[DropBoxId]).userId[i]=-1;
		}
		return 0;
	}
	
*/	int i;
	for( i=0;i<userArrSize;i++){
		if((glbDropBox[DropBoxId]).userId[i]==-1){
			(glbDropBox[DropBoxId]).userId[i]=1;	// make the user with i id valid.
			applyOperationOnAll(glbDropBox[DropBoxId].trie,ForEachFileNewUserAtEmpty, &i);
			return i;
		}	
	}
	int tempPrevSize=glbDropBox[DropBoxId].userArrSize;

	temp=malloc(2*tempPrevSize*sizeof(int));
	if(!temp){
		perror("error in malloc in getNewUserof DropBox\n");
		return -1;
	}
	memcpy(temp,glbDropBox,tempPrevSize*sizeof(int));
	free(glbDropBox[DropBoxId].userId);
	glbDropBox[DropBoxId].userId=(int *)temp;
	glbDropBox[DropBoxId].userArrSize=2*tempPrevSize;	

	//make all other dropBox invalid.
	for( i=tempPrevSize+1;i<glbDropBox[DropBoxId].userArrSize;i++){
		(glbDropBox[DropBoxId]).userId[i]=-1;
	}
	applyOperationOnAll(glbDropBox[DropBoxId].trie, ForEachFileNewUserNonEmpty, &tempPrevSize);

	return tempPrevSize;
}


//Handle the installation for  new User.
void* HandleNewUser(void * id){
	if(!id){
		perror("Error in HandleNewUser argument\n");
		return NULL;
	}
	int connid;
	connid=*((int *)id);
	printf("Connection Id is :%d\n",connid);
	int buff[2];
	int dropBoxId;
	char folderName[1024];
	if(read(connid, buff, sizeof(int)*2)!=(sizeof(int)*2)){
		perror("Error in HandleNewUser read for buffer\n");
		return NULL;
	}
	if(buff[0]==1){
		dropBoxId=buff[1];
	}else{
		//printf("Befor Connid:%d\n",connid);
		dropBoxId=getNewDropBoxID();
		//printf("1 Connid:%d\n",connid);
		glbDropBox[dropBoxId].currentUserId=-1;
		//printf("2 Connid:%d\n",connid);
		sprintf(folderName,"%s%d",dropBoxDir,dropBoxId);
		//printf("3 Connid:%d\n",connid);
		mkdir(folderName, S_IRWXU);
		//printf("After Connid:%d\n",connid);
	}
	//printf("ServerDropBoxID: %d\n",dropBoxId);
	buff[0]=dropBoxId;
	buff[1]=getNewUserOfDropBox(dropBoxId);
	printf("DropBoxId,ServerUserID:%d,%d\n",buff[0],buff[1]);	
	//printf("Connid:%d\n",connid);
	if(write(connid, buff, 2*sizeof(int))<(2*sizeof(int))){
		perror("Error in HandleNewUser write for buffer\n");
	}
	return NULL;
}
