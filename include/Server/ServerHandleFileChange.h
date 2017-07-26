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

#include "Server/ServerCommon.h"
#include "Common/CommandHeader.h"

#ifndef __SERVER_FILE_CHANGE__
	#define __SERVER_FILE_CHANGE__
	
		
	void sendUserNewFileCreation(HEADER *header, int connid, char *fileName, DROPBOX *dropBox, int userId); 	//new file is created.

	void sendUserFileDeletion(HEADER *header, int connid, char *fileName, DROPBOX *dropBox, int userId);	//file is deleted.
	
	void sendUserFileModify(HEADER *header, int connid, char *fileName, DROPBOX *dropBox, int userId);

	//void sendUserFileMoved(char *source, char *destination);

	void sendUserDirectoryCreate(HEADER *header, int connid, char *dirPath, DROPBOX *dropBox, int userId);
	
	//void sendUserDirectoryMoved(char *source, char *destination);
	
	void sendUserDirectoryRemoved(HEADER *header, int connid, char *dirPath, DROPBOX *dropBox, int userId);
	
	
	void receiveUserNewFileCreation(int connid,char *fileName, DROPBOX *dropBox, int userId); 	//new file is created.

	void receiveUserFileDeletion(int connid, char *fileName, DROPBOX *dropBox, int userId);	//file is deleted.
	
	void receiveUserFileModify(int connid, char *fileName, DROPBOX *dropBox, int userId);

	//void receiveUserFileMoved(char *source, char *destination);

	void receiveUserDirectoryCreate(int connid, char *dirPath, DROPBOX *dropBox, int userId);
	
	//void receiveUserDirectoryMoved(char *source, char *destination);
	
	void receiveUserDirectoryRemoved(int connid, char *dirPath, DROPBOX *dropBox, int userId);

#endif	
