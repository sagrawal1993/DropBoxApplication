

#include "Common/TRIE.h"
#include "Common/CommandHeader.h"

#ifndef __SERVER_COMMON__
	#define __SERVER_COMMON__

	typedef struct dropBox{
		int dropBoxId;		// Contains the dropBoxId, else -1 if this DropBox is not valid one
		//char *directory;
		int userArrSize;
		int *userId;	//This list contain -1 at any index, if that userId is empty.
		TRIE *trie;
		int currentUserId; 	//the user whom is connected at current time.
							//this is -1, if no user is connected.
	} DROPBOX;

	typedef struct perFileStruct{
		char *name;
		char *moved;		//name of the location form which the file moved here.
		//int *userToUpdate;		//list of machine id of the  user to update this file. -1 if userId is not valid.
		char *flag;		//tell the command associated with this file for the user associated with machineId at the index at userToUpdate.
					//	0th bit- created.
					//	1st bit - modify.
					// 	2nd bit -  moved.
					//	3rd bit -deleted.
		int isDirectory;	// 0 it is directory, 1 for file.
	} FILEINFO;
	//end with '/'

	typedef struct data{
		DROPBOX *dropBox;
		int connid;
		int machineId;
	} DATA;

	extern char *dropBoxDir;//[1024];	//this string contains the directory path which contains all the dropBox folder.
						//DropBox folder are named with its dropbox id, contains all the file in that dropBox. 
	extern DROPBOX *glbDropBox;	//array contains all the dropBox structure.
	extern int dropBoxArrSize;		//size of glbDropBox array.	


	#define isDeleted(flag) (flag &  8)
	#define isCreated(flag) (flag & 1)
	#define isModify(flag) (flag & 2 )
	#define isMoved(flag) (flag & 4)

#endif

