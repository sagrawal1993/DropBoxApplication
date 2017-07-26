/******************************************************************************************************
*
*	Header file for RegisterNotifier for all the directory under the root directory of the dropBox.
*	
*
*
************************************************************************************************************************/

#ifndef __REGISTER_NOTIFIER__
	#define __REGISTER_NOTIFIER__
	

	#define MAX_FOLDER_ON_SYSTEM 1024
	#define TO_WATCH ( IN_ISDIR | IN_MOVE | IN_MOVE_SELF | IN_MOVED_TO | IN_MOVED_FROM  | IN_MODIFY | IN_DELETE | IN_CREATE ) 



	extern char *wdControlMapping[ MAX_FOLDER_ON_SYSTEM ];

	int add_notifier(char *rootDirectory);
	int addWatchOnDirectory(int fdNotify, char *name, char *rootDirectory);
	int removeWatchOnDirectory(int fdNotify,char *name, char *rootDirectory);

#endif
