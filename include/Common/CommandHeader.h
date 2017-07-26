#ifndef __COMMAND_HEADER__
	#define __COMMAND_HEADER__
	//send by client.		//initialization means the client side will update its file that has been changed.
typedef struct header{
	
	int dropBoxId;		//drop box id to which it is connecting.
	int machineId;		//machine id of itself.
	char flag;	// LSB -1st bit.- if 1 then it is initialization, if 0 then 	
				// 2nd bit -  tell wheater it is a command or data packet. 0- command, 1- data packet.
				// 3rd bit - tell wheater it is last data packet or not. 0- not, 1-last data packet for a file.
				// 4th bit - tell wheater there is no more file to be send for initialization. 
				// 876 's bit will tell the command.
				//Options 
					// 1 - new file is created.
					// 2- file is deleted.
					// 3- file is moved.
					// 4 - directory is created.
					// 5- directory is deleted.
					// 6 - directory is moved.
					// 7 - file is modified.
	short int size; //if command then show the size of file name //if data packet it will show the size of the data send.
				
} HEADER;

//if it is the command then first 2 byte will show the lenth of the name of the file 
//after it the name of the file will be there.

	#define resetFlag(header) ((header)->flag &=0)

	#define isInitialization(header) (!(1 ^ ( (header)->flag)))
	#define isData(header) (!(2 ^ ( (header)->flag)))
	#define isLastPacket(header) (!(3 ^ ((header)->flag)))
	#define isNoMoreFile(header) (!(4 ^ (header)->flag))
	#define isExitCommand(header) (!(5 ^ (header)->flag))


	#define isNewFileCreated(header) (!(6 ^ ((header)->flag)))
	#define isFileDeleted(header) (!(7 ^ ((header)->flag)))
	#define isFileMoved(header) (!(8 ^ ((header)->flag)))
	#define isDirectoryCreated(header) (!(9 ^ ((header)->flag)))
	#define isDirectoryDeleted(header) (!(10 ^ ((header)->flag)))
	#define isDirectoryMoved(header) (!(11 ^ ((header)->flag)))
	#define isFileModified(header) (!(12 ^ ((header)->flag) ))

	#define setInitialization(header) (((header)->flag)|=1)
	#define setData(header) (((header)->flag)|=2)
	#define setLastPacket(header) (((header)->flag)|=3)
	#define setNoMoreFile(header) (((header)->flag)|=4)
	#define setExitCommand(header) (((header)->flag)|=5)


		
		
	#define setNewFileCreated(header) (((header)->flag)|=6)
	#define setFileDeleted(header) (((header)->flag)|=7)
	#define setFileMoved(header) (((header)->flag)|=8)
	#define setDirectoryCreated(header) ( ((header)->flag)|=9)
	#define setDirectoryDeleted(header) (((header)->flag)|=10)
	#define setDirectoryMoved(header) (((header)->flag)|=11)
	#define setFileModified(header) (((header)->flag)|=12 )

/*
	#define resetInitialization(header) (((header)->flag) &=254)
	#define resetData(header) (((header)->flag)&=253)
	#define resetLastPacket(header) (((header)->flag)&=251)
	#define resetNoMoreFile(header) (((header)->flag)&=247)
	#define resetExitCommand(header) (((header)->flag)&=239)

	#define resetNewFileCreated(header) (((header)->flag)&=223)
	#define resetFileDeleted(header) (((header)->flag)&=191)
	#define resetFileMoved(header) (((header)->flag)&=159)
	#define resetDirectoryCreated(header) ( ((header)->flag)&=127)
	#define resetDirectoryDeleted(header) (((header)->flag)&=95)
	#define resetDirectoryMoved(header) (((header)->flag)&=63)
	#define resetFileModified(header) (((header)->flag)&=31 )
*/
#endif
