CC = gcc        # Compiler
CFLAGS = -Wall  # Compiler flags

# For Make all
all: server.out client.out

# For client: echo PORT
echos: server.out
	./server.out $(PORT)

#For server: echos
# Pending task: Client must accept IP address as an input
echo: client.out
	./client.out $(PORT) 

server.out: server.c
	$(CC) $(CFLAGS) -o server.out server.c

client.out: client.c
	$(CC) $(CFLAGS) -o client.out client.c

# for Make clean
clean:
	rm -f server.out client.out