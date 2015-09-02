CC=gcc
CFLAGS=-Wall
EXECUTABLE=reverse_shell
LIBS=-lssl

all:
	$(CC) $(CFLAGS) $(LIBS) reverse_shell.c -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
