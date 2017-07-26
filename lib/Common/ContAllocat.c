/*****************************************************************************************************
*
*	This will give the continous allocated sapace for node to be implement for different data-structure.	
*	So instead of dealing with pointer, we deal with the index of the array.
* 	This tries to get the advantage of localily of refrence. That might reduce the page fault for our 
* 	Data Structure access.
*
*
*	This file for implement the function for getting and managing the space for the nodes of Data Structure.
*	This will give the space in such a manner that all the node are allocated in the continous manner, 
* 	So that the page fault occurs in less number of time.
*
*	Note:: size of the array's space, the number of allocated node and the size of each node is stored in 
* 		the itself, before the first four position of the array.
*	
*	Requirement for using it:::
*		Each node should have the size atleast the size of the integer.
* 		
*	First one is for size of each node.
* 	Second one is for size of array.
*	Thired one denote the index of the node which will be allocated if there is no node in the array.
* 	Fourth one contains the index to the first free node.
*
*
*
*
*	How it works:::::
*		For deletion :: each free node contains the index of the previous free node .
*					third one will denote the last node's index free on this array.
*
*
*
*
*
*
*
*
*****************************************************************************************************/

#include "Common/ContAllocat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//this will give the pointer to the array containing the nodes.

void *newAllocator(int nodeSize){
	if(nodeSize<=sizeof(int) || INIT_ALLOC<=0){
		perror("Need to have some size of array or the node.");
		return NULL;
	}
	int * array = (int *)malloc( (nodeSize*INIT_ALLOC) + (sizeof(int)*4) );	
	if(!array){
		perror("Malloc Can't allocate this much of memory");
		return NULL;
	}
	array[0]=nodeSize;
	array[1]=INIT_ALLOC;
	array[2]=0;
	array[3]=-1;	//initiale no node on free list. 
	return &(array[4]);
}



//fill the index of the node allocated at the address given as second argument.
//first argument is the starting address of the array given by newAllocator.

void *getNewNode(void *array,int *index){	
	
	if(!array||!index){
		perror("You call newAllocate Method to get the array.");
		return NULL;
	}
	int *conf=(int *)(array-sizeof(int)*4);
	
	//if all the array node has been allocated.
	if(conf[3]!=-1){	//there is free node in array.
		*index=conf[2];
		conf[2]=*((int *)(array+conf[2]*conf[0]));	//the previos free node index is stored in this nodes' start.
		return array;
	}
	else if(conf[1]==conf[2]){
		//allocate the new array of double size and copy the array to other array.
		int * newArray=(int *)malloc(conf[0]*2*conf[1]+sizeof(int)*4);
		
		if(!newArray || !memcpy(newArray,conf,conf[0]*conf[1]+sizeof(int)*4)){
			perror("Malloc didn't allocate or copy isn't happened");
			return NULL;
		}
		newArray[1]=2*conf[1];	//set the new size of the array of nodes.
		*index = conf[2];	//the index that need to be return to the array.
		newArray[2]=conf[2]+1;	
		free(conf);	//free the previously allocated space
		return &(newArray[4]);
	}else{
		*index=conf[2];
		conf[2]++;
		return array;
	}
	return NULL;
}


//delete the node allocated at a particular index.
int deleteNode(void *array, int index){
	if(!array){
		return -1;
	}
	int *conf=(int *)(array-sizeof(int)*4);
	
	if(conf[2]<=index) //index to free is out of the last node to allocate.
		return -1;
	int *temp=(int *)(array+(conf[0]*index));
	*temp=conf[3];
	conf[3]=index;
	return 1;
}
