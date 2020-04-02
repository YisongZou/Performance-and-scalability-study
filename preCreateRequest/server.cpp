#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/time.h>

//Thread Pool Library for c++
#include "ctpl_stl.h"

#include "server.hpp"

using namespace std;

//Store the buckets
vector<int> buckets;

//Protects buckets
std::mutex buckets_mutex;

void threadFunc(int id, int num,int client_connection_fd){
  char buffer[50];
  memset(buffer, '\0', sizeof(buffer));
  recv(client_connection_fd, buffer, 50, 0);

  int count;
  int position;
  string message(buffer);
  messageParser(count,position,message);
  cout <<"Number of Request:" << num << " Server received: " << count << " " << position << endl; 

  //delay loop
  struct timeval start, check;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do {
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec/1000000.0)) - (start.tv_sec + (start.tv_usec/1000000.0));
  } while (elapsed_seconds < count);

  const lock_guard<mutex> lock(buckets_mutex);//Lock guard for buckets
  buckets[position] += count;
  //  for(size_t i = 0 ; i < buckets.size(); i++){
  //  cout<< buckets[i] << " " ;
  // }
  string temp = to_string(buckets[position]);
  const char *result = temp.c_str();
  send(client_connection_fd, result, strlen(result), 0);
  close(client_connection_fd);
  //cout <<"Number of request:" << num << " Server received: " << count << " " << position << endl;
  //num ++;
}


int main(int argc, char *argv[])
{
  //Count the num of requests
 int num = 1;

  //Pre create the threads
  ctpl::thread_pool p(2000 /* 700 threads in the pool */);
  
  //Check if the number of command line arguments is correct
  if (argc != 2) {
    cerr << "Wrong number of command line arguments\n Usage: server <num_of_buckets>\n";
    exit(EXIT_FAILURE);
  }

  //Initialize the buckets
  int buckNum = stoi(argv[1]);
  for(int i = 0 ; i < buckNum; ++i){
    buckets.push_back(0);
  }

  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *hostname = NULL;
  const char *port     = "12345";

  memset(&host_info, 0, sizeof(host_info));

  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  status = listen(socket_fd, 100);
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl; 
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  } //if

  cout << "Waiting for connection on port " << port << endl;
  
  struct timeval start, check;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  
  do{
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec/1000000.0)) - (start.tv_sec + (start.tv_usec/1000000.0));
    //while(1){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
      cerr << "Error: cannot accept connection on socket" << endl;
      continue;
    } //if  
    p.push(threadFunc,num,client_connection_fd);
    //cout<< "Number of Requests:"<< num << endl;
    num++;
  }while (elapsed_seconds < 300);
    
  freeaddrinfo(host_info_list);
  close(socket_fd);
  
  return 0;
}
