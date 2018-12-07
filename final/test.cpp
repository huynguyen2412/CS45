#include <iostream>
#include <string>

using namespace std;

void reduceSign(string &input);
bool checkPrevious(char input);
bool checkForward(char input);
void minusPlusHelper(string &input,unsigned long long &firstPos);
bool getInput(string &line);
void unaryHelper(string &exp);
bool isLetter(char i);
bool isNumber(char i);


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
        cout << "--------\n";
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
        {
            while(input[pos+1] == ' ')
                input.erase(pos+1,1);       //remove extra space after binary - +
            minusPlusHelper(input,pos);
        }
        else if(input[pos] == '*' || input[pos] == '/')
        {
            while(input[pos+1] == ' ')
                input.erase(pos+1,1);       //remove extra space after binary - +
            pos++;
        }
        else pos++;
    }
}

// Return true if previous character is A-Z , 0-9, ')', '!', or ' '
bool checkPrevious(char input)
{
    return  (input >= 'A' && input <= 'Z') 
            || (input == '!' || input == ')' ) 
            || (input >= '0' && input <= '9');
}

// Return true if previous character is A-Z , 0-9, '(', or ' '
bool checkForward(char input)
{
    return  (input >= 'A' && input <= 'Z') 
            || (input == '(' ) 
            || (input >= '0' && input <= '9');
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
            if( (exp[i-1] == ' ' && exp[i+1] == ' ')
                 || (exp[i-1] == ' ' && checkForward(exp[i+1]))
                 || (checkPrevious(exp[i-1]) && exp[i+1] == ' ')
                 || (checkPrevious(exp[i-1]) && checkForward(exp[i+1]))
                )
            {
                i++;
            }
            else 
                exp[i] == '-' ? (void) (exp[i++] = '#') : (void)exp.erase(i++,1);
        else i++;
        /* original code (Not Removed) */
        //     if(exp[i] == '-')
        //     {
        //         if(!checkPrevious(exp[i-1]) && !checkForward(exp[i+1]))
        //             exp[i++] = '#';
        //         else i++;
        //     }
        //         // !checkPrevious(exp[i-1]) || !checkForward(exp[i-1]) ? exp[i++] = '#': i++;     //replace unary by '#'
        //     else  
        //         if(!checkPrevious(exp[i - 1]) && !checkForward(exp[i+1])) 
        //             exp.erase(i++,1); //remove '+' unary
        //         else
        //             i++;                    
        // else i++;                                                   //if it's not '+' or '-'
        /* original code (Not Removed) */

    }
}


/*  Detect the fraction 
    When the pointer read a space, check substring next to it.
    If it form A/B and a char after denominator B is one of following 
    chars '+-/*! ', it's a fraction.
    Remove all extra space between denominator and numerator
    otherwise, return false;
    @pos: the current index where starting to find fraction
    @noSpace: to distinguish between mixed number and fraction
*/
bool isFraction(string& exp,bool noSpace,int pos)
{
    //search until finding the list of operator 
    if(noSpace)
    {
        //remove leading spaces 32/          ( 2 +5)
        while(pos < exp.size() && exp[pos+1] == ' ')
            exp.erase(pos++,1);
        //Denominator is an expression
        while((exp[pos++] >= '0' && exp[pos++] <= '9') || (exp[pos++] >= 'A' && exp[pos++] <= 'B'))
    }
}

//remove trailing space
void removeTrailing(string& exp)
{
    while(exp.peek() == ' ')
        exp.erase(pos++,1);
}

//Check a char is number
bool isNumber(char i)
{
    return (i >= '0' && i <= '9');
}

//Check a char is letter
bool isLetter(char i)
{
    return (i >= 'A' && i <= 'B');
}