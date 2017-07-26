/*****************************************************************************************************
*
*	@author:: Suraj Agrawal
*	@Project:: DropBox
*
*	Description:: Header file for File Transfer.
*
*
*
*
*
*****************************************************************************************************/

#ifndef __FILE_TRANSFER__
	#define __FILE_TRANSFER__

	int sendFile(int connid, char *fileName);	//send the file over connection with socket id connid.
	
	int receiveFile(int connid, char *fileName);		//receive the file over connection with socket id connid.
	int writeBlock(int connid, void *buff, int byteToWrite);
	int readBlock(int connid, void *buff, int byteToRead);
#endif
