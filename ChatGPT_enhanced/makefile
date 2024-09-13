CC = gcc        # Compiler
CFLAGS = -Wall  # Compiler flags

# For Make all
all: server.out client.out

# For client: required PORT
echos: server.out
	./server.out $(PORT)

#For server: required IP address and PORT
echo: client.out
	./client.out $(SERVER_ADDR) $(PORT) 

#for gcc server.c -o server.out
server.out: GPT_enh_server.c
	$(CC) $(CFLAGS) -o server.out GPT_enh_server.c

#for gcc client.c -o client.out
client.out: GPT_enh_client.c
	$(CC) $(CFLAGS) -o client.out GPT_enh_client.c

# for Make clean
clean:
	rm -f server.out client.out