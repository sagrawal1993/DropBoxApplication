
INCLUDE:=../../include

Common: Connection.o TRIE.o ContAllocat.o FileTransfer.o


ContAllocat.o: ContAllocat.c $(INCLUDE)/Common/ContAllocat.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c ContAllocat.c -o $(OBJ)/ContAllocat.o

TRIE.o: TRIE.c $(INCLUDE)/Common/TRIE.h $(INCLUDE)/Common/ContAllocat.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c TRIE.c -o $(OBJ)/TRIE.o

Connection.o: Connection.c $(INCLUDE)/Common/Connection.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c Connection.c -o $(OBJ)/Connection.o

FileTransfer.o: FileTransfer.c $(INCLUDE)/Common/FileTransfer.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c FileTransfer.c -o $(OBJ)/FileTransfer.o


