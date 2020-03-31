#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <thread>

//Thread Pool Library for c++ 
#include "ctpl_stl.h"


using namespace std;
void threadFunc(int id, const char * hostname, const char *port){
  while(1){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    // return -1;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    //return -1;
  } //if
  
  cout << "Connecting to " << hostname << " on port " << port << "..." << endl;

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    //return -1;
  } //if
  
  const char *message = "1,4\n";
  send(socket_fd, message, strlen(message), 0);

  char buffer[20];
  memset(buffer, '\0', sizeof(buffer));
  recv(socket_fd, buffer, 20, 0);
  string result(buffer);
  int res = stoi(result);
  cout <<"Id: " << id << " Result is: " << res <<endl;
  
  freeaddrinfo(host_info_list);
  close(socket_fd);
  }
}
