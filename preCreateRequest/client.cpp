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
  if (argc != 3) {
      cout << "Syntax: client <hostname> <num_of_buckets>\n" << endl;
      return 1;
  }
  const char *hostname = argv[1];
  int bucketNum = stoi(argv[2]);
  const char *port     = "12345";


  //Pre create the threads
  ctpl::thread_pool p(4000 /* 200 threads in the pool */);

  for(int i = 0; i < 4000; i++){
    p.push(threadFunc,hostname,port,bucketNum);    
  }
  return 0;
}
