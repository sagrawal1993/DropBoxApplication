/*****************************************************************************************************
*	
* 	This is header file for the function for getting and managing the space for the nodes of Data Structure.
*	This will give the space in such a manner that all the node are allocated in the continous manner, 
* 	So that the page fault occurs in less number of time.
*
*****************************************************************************************************/


#ifndef __CONT_ALLOCATE__
	#define __CONT_ALLOCATE__
	#define INIT_ALLOC 4
	void *newAllocator(int);  //this will give the pointer to the array containing the nodes.

	//first argument is the starting address of the array given by newAllocator.
	void *getNewNode(void * ,int *);	//fill the index of the node allocated at the address given as second argument.
								
	//delete the node from the array of allocated node.
	int deleteNode(void *, int);
#endif
