/*
 * This is the Header file containg Declaration for Trie's function that is being used for storing 
 * the changed file at the server.
 *
 *
 *
 */

#ifndef __TRIE__
 
 #define __TRIE__

	//structure for each Trie node.
	typedef struct trienode{
		int character[100];
		void *value;		//pointer to the value store corresponding to this string(key).
		int count;	//count the number of substring rooted through this node.
		} TRIENODE;

	
	//structure of the TRIE.
	typedef struct trie{
		int root;
		struct trienode *array;	//the array of the struct trienode's starting address.
	} TRIE;


	//This will give the TRIE ADT.
	TRIE* getTrie();


	//Insert the string inside the TRIE.
	void insertInTrie(TRIE *,char *,void *);


	//search if a string is present in the TRIE.
	//return 1 if string is present 
	//else return 0.
	void* searchInTrie(TRIE *,char *);

	//delete a string from trie.
	void deleteInTrie(TRIE *,char *);
	
	
	//static void visit(TRIENODE *,int , void (*)(void *,void *), void *);
	void applyOperationOnAll(TRIE *, void (*)(void *,void *), void *);
#endif
