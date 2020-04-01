TARGETS=client server

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

client: client.cpp client.hpp
	g++ -pthread -o client client.cpp

server: server.cpp client.hpp
	g++ -pthread -o server server.cpp

