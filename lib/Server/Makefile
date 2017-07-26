INCLUDE:=../../include

Server: $(patsubst %.c,%.o,$(wildcard *.c))

ServerHandleFileChange.o: ServerHandleFileChange.c $(INCLUDE)/Common/TRIE.h $(INCLUDE)/Server/ServerHandleFileChange.h $(INCLUDE)/Common/FileTransfer.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c ServerHandleFileChange.c -o $(OBJ)/ServerHandleFileChange.o

OperationOnEachFile.o: OperationOnEachFile.c $(INCLUDE)/Server/ServerCommon.h $(INCLUDE)/Server/ServerHandleFileChange.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c OperationOnEachFile.c -o $(OBJ)/OperationOnEachFile.o

ServerConnectToDropBox.o: ServerConnectToDropBox.c $(INCLUDE)/Server/ServerCommon.h $(INCLUDE)/Server/ServerHandleFileChange.h $(INCLUDE)/Server/OperationOnEachFile.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c ServerConnectToDropBox.c -o $(OBJ)/ServerConnectToDropBox.o

ServerHandleNewUser.o: ServerHandleNewUser.c $(INCLUDE)/Common/TRIE.h $(INCLUDE)/Server/OperationOnEachFile.h $(INCLUDE)/Server/ServerCommon.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c ServerHandleNewUser.c -o $(OBJ)/ServerHandleNewUser.o

