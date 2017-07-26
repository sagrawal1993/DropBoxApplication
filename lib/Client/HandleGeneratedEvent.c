





#include <sys/inotify.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Client/RegisterNotifier.h"
#include "Common/CommandHeader.h"
#include "Client/HandleFileChange.h"
 
void printDetails(struct inotify_event *inotifyBuf){
	if(!inotifyBuf){
		perror("error in printDetails\n");
		return;
	}

	printf("mask-%u\n",inotifyBuf->mask);
	printf("cookie-%u\n",inotifyBuf->cookie);
	printf("len-%u\n",inotifyBuf->len);
	printf("buf-%p,,%p,name-%s\n",inotifyBuf,inotifyBuf->name, inotifyBuf->name);
//	printf("some event occurs\n");
	
	return;
}


//Handle Event Genenerated for inotify.
  int HandleEvent(int fdNotify,int connid, char *rootDirectory, int dropBoxId, int machineId){

  
  	struct pollfd fds[1];
	char buf[1024];
	struct inotify_event *inotifyBuf;
	char fileName[1024];
	HEADER header;

	header.dropBoxId=dropBoxId;
	header.machineId=machineId;

	fds[0].fd = fdNotify;
	fds[0].events = POLLIN;
	while(1){
		#ifdef _DEBUG_
			printf("before poll\n");
		#endif
		if(poll(fds,1,-1)<=0){
			perror("in poll");
			exit(EXIT_FAILURE);
		}
		//printf("%d",fds[0].revents);
		#ifdef _DEBUG_
			printf("before read\n");
		#endif
		//read the event from the notify file heaving the fdNodify id.
		while( read(fdNotify, buf, sizeof(buf) ) > 0 ){

			sleep(1);
			inotifyBuf=(struct inotify_event *)buf;
			//printf("wd-%s\n",wdControlMapping[inotifyBuf->wd]);
			
			uint32_t mask = inotifyBuf->mask;
			
			if(wdControlMapping[inotifyBuf->wd]==NULL){
				strcpy(fileName, inotifyBuf->name);
			}else{
				strcpy(fileName, wdControlMapping[inotifyBuf->wd]);
				strcat(fileName, "/");
				strcat(fileName, inotifyBuf->name);
			}
			
			if((mask & IN_ISDIR ) && (inotifyBuf->name)[0]!='.' && (inotifyBuf->name[strlen(inotifyBuf->name)-1]!='~')){
				//printf("File is Directory\n");
				if(mask & IN_CREATE){
					if(addWatchOnDirectory(fdNotify,fileName, rootDirectory )==-1){
						printf("ERROR\n");
					}
					printf("Directory Created: %s\n",fileName);			
					sendServerDirectoryCreate(&header, connid, fileName);	
					//printDetails(inotifyBuf);	
				}
				if( mask & IN_DELETE){
					removeWatchOnDirectory(fdNotify,fileName, rootDirectory );
					
					printf("Directory Deleted: %s\n",fileName);	
					sendServerDirectoryRemoved(&header, connid, fileName);
					//printDetails(inotifyBuf);	
				}
				//move is not working fine, so we didn't handle it, right now.
				
				if(mask & IN_MOVE){
					printf("Directory Move: %s\n",fileName);	
					//printDetails(inotifyBuf);
				}
				if( mask & IN_MOVE_SELF){
					printf("Directory IN_MOVE_SELF: %s\n",fileName);
				}
				if( mask & IN_MOVED_TO){
					printf("Directory IN_MOVED_TO: %s\n",fileName);
				}
				if( mask & IN_MOVED_FROM){
					printf("Directory IN_MOVED_FROM: %s\n",fileName);
				}
			}else if((inotifyBuf->name)[0]!='.'){
				if(mask & IN_CREATE){
					printf("File Created: %s\n",fileName);
					sendServerNewFileCreation(&header, connid, fileName, rootDirectory);
					//printDetails(inotifyBuf);
				}
				if( mask & IN_DELETE){
					printf("File Deleted: %s\n",fileName);
					sendServerFileDeletion(&header, connid, fileName);
					//printDetails(inotifyBuf);
				}
				if( mask & IN_MODIFY){
					printf("File Modify: %s\n",fileName);
					sendServerFileModify(&header, connid, fileName, rootDirectory);
					//printDetails(inotifyBuf);
				}
				if(mask & IN_MOVE){
					printf("File IN_MOVE: %s\n",fileName);
					//printDetails(inotifyBuf);
				}
				if( mask & IN_MOVE_SELF){
					printf("File IN_MOVE_SELF: %s\n",fileName);
				}
				if( mask & IN_MOVED_TO){
					printf("file IN_MOVED_TO: %s\n",fileName);
				}
				if( mask & IN_MOVED_FROM){
					printf("IN_MOVED_FROM: %s\n",fileName);
				}
			}
		}
	}
  }	
