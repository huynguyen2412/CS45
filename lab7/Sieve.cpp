/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  Sieve of Eratosthenes 
*/
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

bool getLine(string &input);
void primeFinder(int number);

int main(int argc, char const *argv[])
{
    string line ="";
    while(getLine(line))
    {
        if(stoi(line)>1)
            primeFinder(stoi(line));
        else
            cout<<"The number must be 2 or greater.\n";
    }
    return 0;
}

bool getLine(string &line)
{
    cout<<"Enter a number: ";
    getline(cin,line);
    return !line.empty();
}

void primeFinder(int num)
{
    bool prime[num] = {};
    int root = sqrt(num);
    for(int i=2;i<root+1;++i)
    {
        if(!prime[i])   //the number is prime
            for(int j=0;j<num;++j)
                if ((i*i + j*i)<= num) prime[i*i + j*i] = true;
    }
    for(int i=2;i<num+1;++i)
        if(!prime[i]) cout<<i<<" ";
    cout<<endl;
}