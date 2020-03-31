#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <thread>

//Thread Pool Library for c++ 
#include "ctpl_stl.h"
#include "client.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  const char *hostname = argv[1];
  //const char *message = argv[2];
  const char *port     = "12345";
  
  if (argc != 2) {
      cout << "Syntax: client <hostname>\n" << endl;
      return 1;
  }

  //Pre create the threads
  ctpl::thread_pool p(200 /* 200 threads in the pool */);

  for(int i = 0; i < 200; i++){
    p.push(threadFunc,hostname,port);    
  }
  return 0;
}
