#include <iostream>
#include <string>

using namespace std;

void reduceToUnary(string &input);
bool checkPrevious(char input);
void unaryHelper(string &input,unsigned long long &firstPos,unsigned long long firstSize,string &second);
bool getInput(string &line);

int main(int argc, char const *argv[])
{
    string line ="";
    while(getInput(line))
    {
        cout <<line<<endl;
        reduceToUnary(line);
        cout << line<<endl;
    }
    return 0;
}

bool getInput(string &line)
{
    cout<<"Input: ";
    getline(cin, line);                           //Get infix expression
    fflush(stdin);                                //Clear input buffer
    for(unsigned int i = 0; i < line.size(); ++i) //standardize set names to uppercase
        line[i] = toupper(line[i]);
    return line != "";                            //see if the line was empty
}


void reduceToUnary(string &input)
{
    int count = 0;
    unsigned long long i =0;
    unsigned long long firstSize = input.size();        //the original length
    input += '#';
    string second = "";
    while(input[i] != '#')
    {       
        if(input[i] == '-' || input[i] == '+')
            unaryHelper(input,i,firstSize,second);
        else i++;
    }
}

// Return true if previous character is A-Z , 0-9, ')', or '!'
bool checkPrevious(char input)
{
    string temp = input +" ";
    return (temp.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789)! "));
}

void unaryHelper(string &input,unsigned long long &pos,unsigned long long firstSize,string &second)
{
    int sign=0;
    unsigned long long firstPos = pos;
    //the beginning operator
    while(input[pos] == '-' || input[pos] == '+')
        if(input[pos++] == '-')
            sign++;

    //positive sign
    (sign%2 == 0) ?input[pos-1] = '+': input[pos-1] = '-'; //change sign of last operator
    //remove extra operators after parsing   
    input.erase(firstPos,pos-firstPos-1);
    pos=0;
}