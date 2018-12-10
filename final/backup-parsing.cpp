#include "parsing.h"

// int main(int argc, char const *argv[])
// {
//     string line ="";
//     while(getInput(line))
//     {
//         // cout <<line<<endl;
//         // reduceSign(line);
//         // cout << line<<endl;
//         // unaryHelper(line);
//         parsing(line);
//         cout << line<<endl;
//         cout << "--------\n";
//     }
//     return 0;
// }


bool getInput(string &line)
{
    cout<<"Input: ";
    getline(cin, line);                           //Get infix expression
    fflush(stdin);                                //Clear input buffer
    for(unsigned int i = 0; i < line.size(); ++i) //standardize set names to uppercase
        line[i] = toupper(line[i]);
    return line != "";                            //see if the line was empty
}


//reduce the extra - or + and convert unary '-' '+'
void parsing(string &input)
{
    reduceSign(input);
    unaryHelper(input);
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
            if( (exp[i-1] == ' ' && exp[i+1] == ' ')                //binary case
                 || (exp[i-1] == ' ' && checkForward(exp[i+1]))
                 || (checkPrevious(exp[i-1]) && exp[i+1] == ' ')
                 || (checkPrevious(exp[i-1]) && checkForward(exp[i+1])
                 || (exp[i-1] == '#') && checkForward(exp[i+1]))
                )
            {
                i++;
            }
            else
            {
                if(exp[i] == '-')
                {
                    exp.erase(i,1);
                    while ((exp[i] >= 'A' && exp[i] <= 'B') || (exp[i] >= '0' && exp[i] <= '9')){i++;}
                    exp.insert(i++,1,'#');
                }
                else
                    exp.erase(i++, 1);  //remove '+' unary
                // exp[i] == '-' ? (void) (exp[i++] = '#') : (void)exp.erase(i++,1); //insert trailing "#" as '-' unary operator    
            }
        else i++;                       //otherwise, just skip

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



//remove trailing space
void removeTrailing(string& exp, unsigned long long pos)
{
    while(pos < exp.size() && exp[pos] == ' ')
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


