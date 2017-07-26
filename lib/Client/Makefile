INCLUDE:=../../include


Client: $(patsubst %.c,%.o,$(wildcard *.c))

HandleFileChange.o: HandleFileChange.c $(INCLUDE)/Common/FileTransfer.h $(INCLUDE)/Common/CommandHeader.h $(INCLUDE)/Client/HandleFileChange.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c HandleFileChange.c -o $(OBJ)/HandleFileChange.o

HandleGeneratedEvent.o: HandleGeneratedEvent.c $(INCLUDE)/Client/RegisterNotifier.h $(INCLUDE)/Client/HandleFileChange.h $(INCLUDE)/Common/CommandHeader.h 
	$(CC) $(CFLAGS) -I $(INCLUDE) -c HandleGeneratedEvent.c -o $(OBJ)/HandleGeneratedEvent.o

InstallNewUser.o: InstallNewUser.c $(INCLUDE)/Common/Connection.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c InstallNewUser.c -o $(OBJ)/InstallNewUser.o

RegisterNotifier.o: RegisterNotifier.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -c RegisterNotifier.c -o $(OBJ)/RegisterNotifier.o

ClientConnectToDropBox.o: ClientConnectToDropBox.c $(INCLUDE)/Common/CommandHeader.h $(INCLUDE)/Client/HandleFileChange.h $(INCLUDE)/Client/HandleNotifyEvent.h $(INCLUDE)/Client/RegisterNotifier.h
	$(CC) $(CFLAGS) -I $(INCLUDE) -c ClientConnectToDropBox.c -o $(OBJ)/ClientConnectToDropBox.o

