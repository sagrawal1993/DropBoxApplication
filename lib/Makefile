DIR:=.
#DIR:=~suraj/workplace/osProject/DropBox/lib

#Makefile for lib
VPATH:=Common Server Client
export

#include $(DIR)/Common/Makefile
#include $(DIR)/Server/Makefile
#include $(DIR)/Client/Makefile

lib:
	cd Common && $(MAKE) Common
	cd Server && $(MAKE) Server
	cd Client && $(MAKE) Client
