CC = gcc
CFLAGS = -Wall -lm -lncurses -lprocps
all: servs  exec
servs: server.c
	$(CC) -o server server.c
exec: inter.c
	$(CC) -o adem inter.c $(CFLAGS)
clean:
	rm -f *.o
serv:
	./server
interface:    
	./adem
