/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  Add N-Digit
*/
#include <iostream>
#include <string>
#include <math.h>

using namespace std;
int charToInt(char letter);
string addDigit(string num1, string num2);
void leadingZero(string &num,int count);
int sumTwoChar(char n1,char n2);
bool getLine(string &input1,string &input2);

int main()
{
    string result ="";
    string num1 ="";
    string num2 ="";
    while(getLine(num1,num2))
    {
        result = addDigit(num1,num2);
        cout<<"Result is: "<<result<<endl;
        cout<<"-------------"<<endl;
    }
    return 0;
}

int charToInt(char letter)
{
    return letter - 48;
}

int sumTwoChar(char n1,char n2)
{
    return charToInt(n1)+charToInt(n2);
}

//fill up leading 0 for small number
void leadingZero(string &num,int count)
{
    for(int i=0;i<count;++i)
        num = "0"+num;
}

string addDigit(string n1, string n2)
{
    int carry =0;
    string result = "";
    (n1.size()>n2.size()) ? leadingZero(n2,n1.size()-n2.size()) : leadingZero(n1,n2.size()-n1.size());
    for(int i=n1.size()-1;i>-1;--i)
    {        
        if((sumTwoChar(n1[i],n2[i]) + carry) >= 10)
        {
            result = to_string(sumTwoChar(n1[i],n2[i]) + carry++ -10) + result;                //added 1 to carry-over
            // carry++;
            carry>1 ? carry-- : carry;                                                        //only 1 carry-over           
        }
        else
        {
            result = to_string(sumTwoChar(n1[i],n2[i]) + carry) + result;
            carry>=1 ? carry--:carry; 
        }
    }
    carry>0 ? result = to_string(carry)+result:result;
    return result;
}

bool getLine(string &num1,string &num2)
{
    cout<<"Enter first number: ";
    getline(cin,num1);
    cout<<"Enter second number: ";
    getline(cin,num2);
    return !num1.empty() && !num2.empty();
}