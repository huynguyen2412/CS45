/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  The prime decomposition
*/
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

bool getLine(string &input);
void primeFinder(int number,bool prime[]);
void decomposition(int num);


int main(int argc, char const *argv[])
{
    string line ="";
    while(getLine(line))
    {
        if(stoi(line)>1)
            decomposition(stoi(line));
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

//prime decomposition
void decomposition(int num)
{
    bool prime[num] = {};
    string result= "";
    int index = 2;   
    int quot = num;      //remainder
    primeFinder(num,prime);
    while(quot != 1)
    {
        if(!prime[index] && quot%index == 0)
        {
            quot = quot/index;
            result += "x"+ to_string(index);
        }
        else
            index++;
    }
    result.erase(0,1);
    cout <<"Number "<<num<<"'s prime decomposition: "<< result<<endl;
}


// Get array of prime numbers
void primeFinder(int num,bool prime[])
{
    // prime[num] = {};
    int root = sqrt(num);
    for(int i=2;i<root+1;++i)
    {
        if(!prime[i])   //the number is prime
            for(int j=0;j<num;++j)
                if ((i*i + j*i)<= num) prime[i*i + j*i] = true;
    }
    // for(int i=2;i<num+1;++i)
    //     if(!prime[i]) cout<<i<<" ";
    // cout<<endl;
}

