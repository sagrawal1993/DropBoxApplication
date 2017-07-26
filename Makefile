##DropBox parenent Makefile.


SRC:=$(shell pwd)
OBJ:=$(SRC)/Object
BIN:=$(SRC)/bin
CC:=gcc
CFLAGS:=-Wall -g
INCLUDE:=$(SRC)/include 
VPATH:=$(INCLUDE):$(SRC)/lib:$(SRC)/Driver $(OBj)
export 

#include lib/Makefile
#include Driver/Makefile

Default: subsystem ClientDropBox.out InstallNewUser.out ServerDropBox.out


subsystem:
	$(MAKE) -C lib lib
	$(MAKE) -C Driver Driver



ClientDropBox.out: $(OBJ)/dropBoxUser.o $(OBJ)/Connection.o $(OBJ)/FileTransfer.o $(OBJ)/ContAllocat.o $(OBJ)/TRIE.o $(OBJ)/ClientConnectToDropBox.o $(OBJ)/HandleFileChange.o $(OBJ)/RegisterNotifier.o $(OBJ)/HandleGeneratedEvent.o 
	$(CC) $(CFLAGS) $(OBJ)/dropBoxUser.o $(OBJ)/Connection.o $(OBJ)/FileTransfer.o $(OBJ)/ContAllocat.o $(OBJ)/TRIE.o $(OBJ)/ClientConnectToDropBox.o $(OBJ)/HandleFileChange.o $(OBJ)/RegisterNotifier.o $(OBJ)/HandleGeneratedEvent.o -o $(BIN)/ClientDropBox.out -lpthread

InstallNewUser.out: $(OBJ)/InstallNewUser.o $(OBJ)/dropBoxNewUser.o $(OBJ)/Connection.o
	$(CC) $(CFLAGS) $(OBJ)/InstallNewUser.o $(OBJ)/dropBoxNewUser.o $(OBJ)/Connection.o $(OBJ)/FileTransfer.o $(OBJ)/ContAllocat.o $(OBJ)/TRIE.o $(OBJ)/ClientConnectToDropBox.o $(OBJ)/HandleFileChange.o $(OBJ)/RegisterNotifier.o $(OBJ)/HandleGeneratedEvent.o -o $(BIN)/InstallNewUser.out -lpthread
 
ServerDropBox.out: $(OBJ)/dropBoxServer.o $(OBJ)/Connection.o $(OBJ)/FileTransfer.o $(OBJ)/ContAllocat.o $(OBJ)/TRIE.o $(OBJ)/ServerConnectToDropBox.o $(OBJ)/ServerHandleFileChange.o $(OBJ)/OperationOnEachFile.o $(OBJ)/ServerHandleNewUser.o
	$(CC) $(CFLAGS) $(OBJ)/dropBoxServer.o $(OBJ)/Connection.o $(OBJ)/FileTransfer.o $(OBJ)/ContAllocat.o $(OBJ)/TRIE.o $(OBJ)/ServerConnectToDropBox.o $(OBJ)/ServerHandleFileChange.o $(OBJ)/OperationOnEachFile.o $(OBJ)/ServerHandleNewUser.o -o $(BIN)/ServerDropBox.out -lpthread

