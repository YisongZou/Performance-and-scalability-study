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
void threadFunc(int id, const char * hostname, const char *port, int bucketNum){
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
     freeaddrinfo(host_info_list);
     continue;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    freeaddrinfo(host_info_list);
    close(socket_fd);
    continue;
  } //if
  
  //  cout << "Connecting to " << hostname << " on port " << port << "..." << endl;

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    freeaddrinfo(host_info_list);
    close(socket_fd);
    continue;
  } //if
  
  int delay = rand() % (3 - 1 + 1) + 1;
  int bucket = rand() % ((bucketNum-1) - 0 + 1) + 0;
  string temp = to_string(delay) + "," + to_string(bucket) + "\n";
  const char *message = temp.c_str();
  send(socket_fd, message, strlen(message), 0);

  char buffer[50];
  memset(buffer, '\0', sizeof(buffer));
  recv(socket_fd, buffer, 50, 0);
  string result(buffer);
  int res = stoi(result);
  cout <<"Bucket: " << bucket << " Result is: " << res <<endl;
  
  freeaddrinfo(host_info_list);
  close(socket_fd);
  }
}
