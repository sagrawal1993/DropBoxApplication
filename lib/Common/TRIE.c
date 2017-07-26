/*
 *
 *	This is the TRIE DS implementation which uses  
 *
 *
 *
 *
 *	For using applyOnAllOperation() funciton need to define the function that will pass the first argument to be the data store at trie node.
 *	And the secod argument will take the argument pass by the user. 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Common/TRIE.h"
#include "Common/ContAllocat.h"



//This will give the TRIE ADT. //new instance of ADT.

TRIE* getTrie()
{
	TRIE *temp;
	int i;
	temp=(TRIE *)calloc(1,sizeof(TRIE));	//TRIE Structure stores the control information.
	if(!temp){
		perror("Error in getTrie after Calloc\n");
		return NULL;
	}
	temp->array=newAllocator(sizeof(TRIENODE));		//allocate the new node for root for trie.
	
	if(temp->array==NULL){
		perror("Error in getTrie temp->array is NULL\n");
		return NULL;
	}
	temp->array=getNewNode(temp->array, &(temp->root));
	if(temp->array==NULL){
		perror("Error in getNewNode in getTrie\n");
		return NULL;
	}
	for(i=0;i<100;i++){
		((*(TRIENODE *)(temp->array)).character)[i]=-1;
	}	
	(temp->array)[temp->root].value=NULL;		//set the value to be NULL for this node.
	return temp;
}

//Insert the string inside the TRIE.
void insertInTrie(TRIE *trie, char *string, void *value)
{
	if(!trie || !string){
		perror("Error in insert In Trie in passing argument\n");
		return;	
	}
	TRIENODE *temp=trie->array;		//get the array of TRIENODE structure.
	int index = trie->root;		//get the index of the root node.
	int i=0;		//the incrementer for string.
	
	//insert each character in TRIE.
	while(string[i]!='\0')
	{
		if( (temp[index].character)[string[i]-' ']==-1)		//when there is no node for this character.
		{
			int tempIndex;
			trie->array = getNewNode(trie->array, &tempIndex);
			temp=trie->array;
			(temp[index].character)[ string[i] - ' '] = tempIndex; 		//take the new node from the array.	
			int j,tst;
			tst=(temp[index].character)[string[i]-' '];
			for(j=0;j<100;j++){
				(temp[tst].character)[j]=-1;
			}
			temp[tempIndex].value = NULL;	//if there is no value.
			temp[tempIndex].count = 0;	//make the initial child count  to zero.
			(temp[index].count)++;		//increment the number of child of this node.	
		}
		index=(temp[index].character)[string[i]-' '];
		i++;
	}
	temp[index].value=value;
	return ;
}

//search if a string is present in the TRIE.
//return 1 if string is present 
//else return 0.
//check if count is zero and 
void* searchInTrie(TRIE *trie,char *string)
{
	if(!trie||!string){
		perror("Error in searchInTrie Argument\n");
		return NULL;
	}
	TRIENODE *temp=trie->array;	//array of the nodes.
	int i=0,flag=0;
	int index=trie->root;	//index in the array of root.
	while(string[i]!='\0')
	{
		if( temp[index].character[string[i]-' ']==-1)
		{
			flag=1;
			break;
		}
		index=(temp[index].character)[string[i]-' '];
		i++;
	}
	if(flag==1 || temp[index].value==NULL)
		return NULL;
	return  temp[index].value;
}

//delete a string from trie.
//For our current Purpose we will not delete the node for string from the trie, 
//We just make the value to be NULL to consider that the string is not in trie.

void deleteInTrie(TRIE *trie,char *string)
{
	if(!trie || !string){
		perror("Error in passing argument in deleteInTrie\n");
		return ;
	}
	TRIENODE *temp=trie->array;
	int 	index=trie->root;
	//int *prev,*array;
	
	int i=0;//,j;
	
	while(string[i]!='\0')
	{
		if( temp[index].character[string[i]-' ']==-1)
			return ;
		index=temp[index].character[string[i]-' '];	
		i++;
	}
	(temp[index]).value=NULL;
	
//	if(temp->count==0)
//		free(temp);
	

	// remove the trienode that are there only due to this string.
/*
	while(1)
	{
		temp=top(stack);
		if(temp==null)
//	int temp=trie->root;	//get the root node's indexdd.
		{
			trie->root=NULL;
			break;
		}
		pop(stack);
		if(temp->count>1)
		{
			(temp->character)[string[i]-' ']=NULL;
			temp->count--;
			break;
		}

	}
*/
}

//apply the dfs for this trie tree.
static void visit(TRIENODE *node,int index, void (*func)(void *data,void *user), void *userData){
	
	if(!node){
		perror("Error in visit\n");
		return;
	}
	int i;
	//if there is some value apply the operation.
	if(node[index].value){
		func((node[index].value),userData);
	}
	for(i=0;i<100;i++){	//visit all the child node of this node.
		if((node[index].character)[i]!=-1){
			visit(node,(node[index].character)[i],func, userData);
		}
	}
	return ;
}

// This function will apply the operation on all the string present in the Trie.
// The operation is given as the function as an argument.
// @arg1:: the trie on which we need to apply these.
// @arg2:: the function to apply. @arg1:: the value store at string . @arg2:: the store where the output will be stored.

void applyOperationOnAll(TRIE *trie, void (*func)(void *data,void *userData), void *udata){
	
	if(!trie){
		perror("Error in applyOperationOnAll\n");
		return;
	}
	TRIENODE *array = trie->array;
	int index = trie->root;
	visit(array, index, func ,udata);	
}


