sources = avltree.c daemonize.c epoll.c main.c socket.c visca.c worker.c
cflags = -Wall -Wextra -g -Wno-unused-function -Wno-unused-variable
ldflags =
binname = voproxyd

all:
	gcc $(sources) $(cflags) $(ldflags) -o $(binname)
	./$(binname)

