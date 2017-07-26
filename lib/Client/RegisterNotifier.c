/*************************************************************************************************************************
*
*	Store all the fileName inside the Trie structure.
*	Each file has some value associate with it.
*	Value will be the structure store intermediate control information, As weather the file has been modified or not.
*	
*	Will update the file on server when file is being closed.
*	
*
*************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_FOLDER_ON_SYSTEM 1024
#define TO_WATCH ( IN_ISDIR | IN_MOVE | IN_MOVE_SELF | IN_MOVED_TO | IN_MOVED_FROM  | IN_MODIFY | IN_DELETE | IN_CREATE ) 



char *wdControlMapping[ MAX_FOLDER_ON_SYSTEM ];


 // 	If it is a directory:--
 //	ADD the watch notifier for the directory.
 //	Make the entry of the wdControlMapping.
 //	For each entry in the directory structure, call add_subdir_watch().

 int add_subdir_watch(int fdNotify, char *dirName, char *rootDirectory){
 	DIR *fileDir;
	struct dirent *newDirectory;
 	char *newName, *tempName;
	int wd;
	
	if(!rootDirectory || fdNotify<=0){
		perror("fdNotify is nil or root directory is not given\n");
		return -1;
	}

	if(dirName==NULL){
		tempName=rootDirectory;	
	}else{
		tempName = malloc(sizeof(char)*(strlen(dirName)+strlen(rootDirectory)+5));
		if(!tempName){
			perror("Error in malloc add_subdir_watch\n");
			return -1;
		}
		strcpy(tempName,rootDirectory);
		strcat(tempName,"/");
		strcat(tempName, dirName);
	}
	fileDir = opendir(tempName);
	//check if it is a file or directory.
 	if(fileDir==NULL){
		//if it is a file.
		if(errno==ENOTDIR){
			//infoControl->flag|=2;				
			free(dirName);
			return 0;
		}
		else{
			perror("other error");
			printf("errno-%d,%s\n",errno,dirName);
			return -1;
		}
	}
	//add the notifier for this directory.
	wd=inotify_add_watch(fdNotify, tempName, TO_WATCH);
	//printf("%d\n",wd);
	if(wd<0){
		perror("error in inotify_add_watch\n");
		return -1;
	}
	wdControlMapping[wd]=dirName;

	//read the directory and traverse through all the file of it.
	while((newDirectory=readdir(fileDir))!=NULL){
		if((newDirectory->d_name)[0]!='.'){
			int flag;
			int strLen;
			if(dirName==NULL){
				strLen=strlen(newDirectory->d_name)+2;
				newName = (char *)malloc(strLen *sizeof(char));
				strcpy(newName,newDirectory->d_name);
			}else{
				strLen = strlen(dirName) + strlen( newDirectory->d_name )+3;
				newName = (char *)malloc(strLen * sizeof(char));
				strcpy(newName, dirName);
				strcat(newName, "/");
				strcat(newName, newDirectory->d_name);
			}
			flag = add_subdir_watch(fdNotify, newName, rootDirectory);
			if(flag==-1){
				perror("error in add_subdir_watch\n");
				return -1;
			}
		}
	}
	closedir(fileDir);
 	return 1;
 }
 


// Add the Notifier for all the file under  this directory.
int add_notifier(char *directName){

	if(!directName){
		perror("Error in add_notifier argument\n");
		return -1;
	}
	int fdNotify;
	fdNotify = inotify_init1(IN_NONBLOCK);
	//printf("%d\n",fdNotify);
 	if( fdNotify == -1){
		perror("inotify_init");
		exit(EXIT_FAILURE);
	}
	
	if( add_subdir_watch(fdNotify, NULL, directName) == -1 ){
		perror("add_subdir_watch");
		exit(EXIT_FAILURE);
	}
	return fdNotify;
 }

//name is the path form rootdirectory to the directory created.
//add the watch for newly created directory.
int addWatchOnDirectory(int fdNotify, char *name, char *rootDirectory){
	int newWd;
	if(fdNotify<=0 || !rootDirectory || !name){
		perror("error in addWatchOnDirectory Arguments\n");
		return -1;
	}

	char *strName, temp[1024];

	strName= (char *)malloc(sizeof(char)*(strlen(name)+2));
	if(!strName){
		perror("Error of malloc in addWatchOnDirectory\n");
		return -1;
	}
	strcpy(temp, rootDirectory);
	strcat(temp, "/");
	strcat(temp,name);	
	printf("\n%s\n",temp);
	newWd = inotify_add_watch(fdNotify, temp, TO_WATCH ); 
	if(newWd<0){
		perror("Error in inotify_add_watch in addWatchOnDirectory\n");
		return -1;
	}
	strcpy(strName, name);
	wdControlMapping[newWd] = strName;
	//printf("fdNotify-%d, newWd-%d\n",fdNotify,newWd);
	return 0;	
 }
 
 
//delete watch descriptor entry for deleted directory.
 int removeWatchOnDirectory(int fdNotify, char *name, char *rootDirectory){
	if(fdNotify<=0 || !name || !rootDirectory){
		perror("error in arguments of remove WatchOnDirectory\n");
		return -1;
	}
	
	char strName[1024];
	int newWd;

	strcpy(strName, rootDirectory);
	strcat(strName,"/");
	strcat(strName, name);
	//get the watch discriptor corresponds to this directory.
	newWd = inotify_add_watch(fdNotify, strName, TO_WATCH ); 
	if(inotify_rm_watch(fdNotify, newWd)<0){
		//not successful rm watch.
		return -1;
	}
	free(wdControlMapping[newWd]);
	return 0;
 }


