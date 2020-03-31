TARGETS=client server

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

client: client.cpp
	g++ -g -o $@ $<

server: server.cpp
	g++ -pthread -o server server.cpp

