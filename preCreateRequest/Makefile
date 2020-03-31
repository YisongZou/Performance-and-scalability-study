TARGETS=client server

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

client: client.cpp
	g++ -pthread -o client client.cpp

server: server.cpp
	g++ -pthread -o server server.cpp

