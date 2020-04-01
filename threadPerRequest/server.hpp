#include <cstdlib>
#include <string>

using namespace std;
void messageParser(int & count, int & position, string message){
  string sCount;
  string sPosition;
  size_t i = 0 ;
  while(i < message.size()){
    if(message[i] != ','){
      sCount.push_back(message[i]);
      i++;
    }
    else{
    break;
    }
  }
  i++;
  while(i < message.size()){
      sPosition.push_back(message[i]);
      i++;
  }
  count = stoi(sCount);
  position = stoi(sPosition);
}
