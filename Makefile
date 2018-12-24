# -*- MakeFile -*-

#target: dependencies
#	action

all: pargrep

pargrep: pargrep.o
	gcc pargrep.o -o pargrep -pthread

sysinfo.o: pargrep.c
	gcc -c pargrep.c

clean: 
	rm *.o pargrep