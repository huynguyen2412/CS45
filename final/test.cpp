#include <iostream>
#include <string>

using namespace std;

void trimZeros(string &input)
{
    while(input.size() > 0 && input[0] == '0')
        input.erase(0,1);
}

int main(int argc, char const *argv[])
{
  string zero = "00000";
  trimZeros(zero);
  cout<<"Result :"<<zero<<endl;
  return 0;
}
