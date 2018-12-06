#include <iostream>
#include <string>

using namespace std;

void reduceSign(string &input);
bool checkPrevious(char input);
void minusPlusHelper(string &input,unsigned long long &firstPos);
bool getInput(string &line);
void unaryHelper(string &exp);

int main(int argc, char const *argv[])
{
    string line ="";
    while(getInput(line))
    {
        cout <<line<<endl;
        reduceSign(line);
        cout << line<<endl;
        unaryHelper(line);
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

//call helper to reduce a streak of '-' or '+'
void reduceSign(string &input)
{
    int count = 0;
    unsigned long long pos =0;
    unsigned long long firstSize = input.size();        //the original length
    while(input[pos])
    {       
        if(input[pos] == '-' || input[pos] == '+')
            minusPlusHelper(input,pos);
        else pos++;
    }
}

// Return true if previous character is A-Z , 0-9, ')', or '!'
bool checkPrevious(char input)
{
    string temp = input +" ";
    return (temp.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789)! "));
}


//reduce a streak of '-' or '+' to a single operator
void minusPlusHelper(string &input,unsigned long long &pos)
{
    int sign=0;
    unsigned long long firstPos = pos;
    unsigned long long current;             //the position of pos 
                                            //when pointer hit number or operator 
                                            //is not unary or + - operator 
    //the beginning operator
    while(input[pos] == '-' || input[pos] == '+')
        if(input[pos++] == '-')
            sign++;
    //positive sign
    (sign%2 == 0) ?input[pos-1] = '+': input[pos-1] = '-'; //change sign of last operator
     //reduce extra operators after parsing   
    input.erase(firstPos,current=pos-firstPos-1);
    //move pointer to new pos
    pos = pos-current;
}


//Distinguish between unary (+,-) or a binary sign(-,+)
//remove unary operator if it's +, replace it by "#" if there is unary(-)
//otherwise,binary sign doesn't need to change.  
void unaryHelper(string &exp)
{
    unsigned long long i=0;     //index of a char in string exp
    char check ;          
    while(exp[i])
    {
        //unary case 
        if(exp[i] == '-' || exp[i] == '+')
            if(exp[i] == '-')
                !checkPrevious(exp[i-1]) ? exp[i++] = '#':exp[i++];     //replace unary by '#'
            else
            {    
                if(!checkPrevious(exp[i - 1])) 
                    exp.erase(i,1); //remove '+' unary
                i++;
            }                            
        else i++;                                                   //if it's not '+' or '-'
    } 
}