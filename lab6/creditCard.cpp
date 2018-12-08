/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  Creditcard Generator
*/
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;
bool getInput(string &line);
bool validateCredit();
bool checkDigit(string creditNumber);
char toChar(int num);
string ccHelper(string& input);
void generatorCC(string storage[100000]);

int main(int argc, char const *argv[])
{
    string input;
    int total = 0;
    srand(time(NULL));
    string storage[100000];
    generatorCC(storage);
    return 0;
}

bool getInput(string &line)
{
    cout<<"Credit card numbers: ";
    getline(cin,line);
    fflush(stdin);                                //Clear input buffer
    return !line.empty();
}

bool checkDigit(string num)
{
    int nonCheck = 0;
    string inverse = "";
    for(int i=0;i<num.size();++i)
        inverse+= num[num.size()-1-i];
    for(int i=0;i<inverse.size();++i)
    {
        int temp = 0; 
        if(i%2==0)
            (temp = ((int)inverse[i]-'0')*2)> 9 ? nonCheck+= temp-9 : nonCheck+= temp;
        else
            nonCheck+= ((int)inverse[i]-'0');
    }
    return ((nonCheck*9)%10)== ((int)num[num.size()-1]-'0');
}

char toChar(int num)
{
    return static_cast<char>(48+num);
}

/*  Create a 16-digit number
    First digit is between 3-6 represent CreditCard provider
 */
string ccHelper(string& input)
{
    input ="";
    for(int i=0;i<16;++i)
        (i==0)? input+=toChar((rand()%4+3)) : input+=toChar((rand()%10));
    return input;
}

//check if digit valid add to the array
void generatorCC(string storage[100000])
{
    int count =0;
    while(count < 100000)
    {
        string temp="";
        if(checkDigit(ccHelper(temp)))
        {
            storage[count] = (ccHelper(temp));
            count++;
        }
    }
}