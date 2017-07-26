/*****************************************************************************************************
*
* 	this is the header file for the module handling the change in the file or directory in the 
*	DropBox directory.
*	
*	Options::
*		1- New file can be created.
*		2- Old file can be deleted.
*		3- File can be modified.
*		4- File can be moved in the directory.
*		5- Directory can be created.
*		6- Directory can be deleted.
*
*****************************************************************************************************/
#include "Common/CommandHeader.h"
#ifndef __HANDLE_FILE_CHANGE__
	#define __HANDLE_FILE_CHANGE__
	
	void sendServerNewFileCreation(HEADER *, int connid,char *fileName, char *rootDirectory); 	//new file is created.

	void sendServerFileDeletion(HEADER *, int connid, char *fileName);	//file is deleted.
	
	void sendServerFileModify(HEADER *, int connid, char *fileName, char *rootDirectory);

	//void sendServerFileMoved(HEADER *, int ,char *, char *);

	void sendServerDirectoryCreate(HEADER *, int connid,char *dirName);
	
	//void sendServerDirectoryMoved(HEADER * , int ,char *, char *);
	
	void sendServerDirectoryRemoved(HEADER *, int connid, char *dirName);
	
	
	void receiveServerNewFileCreation(int connid, char *fileName, char *rootDirectory); 	//new file is created.

	void receiveServerFileDeletion(int connid, char *fileName, char *rootDirectory);	//file is deleted.
	
	void receiveServerFileModify(int connid, char *fileName, char *rootDirectory);

	//void receiveServerFileMoved(int , char *, char *);

	void receiveServerDirectoryCreate(int connid, char *dirName, char *rootDirectory);
	
	//void receiveServerDirectoryMoved(int, char *, char *);
	
	void receiveServerDirectoryRemoved(int connid, char *dirName, char *rootDirectory);

#endif	
