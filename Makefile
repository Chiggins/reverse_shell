CC=gcc
CFLAGS=-Wall
EXECUTABLE=reverse_shell

all:
	$(CC) $(CFLAGS) reverse_shell.c -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
