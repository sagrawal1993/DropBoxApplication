INCLUDE:=../include

VPATH:= $(SRC)/lib

Driver: $(patsubst %.c,%.o,$(wildcard *.c))

dropBoxNewUser.o: dropBoxNewUser.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -c dropBoxNewUser.c -o $(OBJ)/dropBoxNewUser.o

dropBoxServer.o: dropBoxServer.c $(INCLUDE)/Server/ServerCommon.h $(INCLUDE)/Common/Connection.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c dropBoxServer.c -o $(OBJ)/dropBoxServer.o

dropBoxUser.o: dropBoxUser.c $(INCLUDE)/Common/Connection.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c dropBoxUser.c -o $(OBJ)/dropBoxUser.o

