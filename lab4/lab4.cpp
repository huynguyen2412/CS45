#include <iostream>
#include <vector>
#include <cstdlib>
 
using namespace std;
 
 
bool illegalSet(string input);
int whoIsFirst(const string &incoming);
bool precedence(const string &incoming, const string &tos);
bool convertToRPN(string input, string& output);
bool getInput(string &line);
void process(string rpn, int sets[], int index);
unsigned int unionOfTwoSets(string x, string y, string &result);
unsigned int intersectionOfTwoSets(string x, string y, string &result);
unsigned int differenceOfTwoSets(string x, string y, string &result);
unsigned int setCompliment(string x, string &result);
int userHelper(string &input, int &equalPos);

int main()
{
    string line, output;            //Create input (line) and output (output) variables for functions to use
    int sets[26];                   //Create a 26 element array of sets
    int equalPos = -1;              //pos of equal sign
    int index = -1;                 //index of a set

    while(getInput(line))           //As long as there is input from the keyboard
    {
        index = userHelper(line,equalPos); // get index of a set
        cout << "The input after eliminate the l-expression:" << line 
             << "the index is " << index << " " << endl;

        //check if the input is invalid or not
        if(convertToRPN(line, output) && index != -1) //See if we can convert infix to postfix notation
            process(output,sets, index);      //If we can, process the input
        else                           //If not, tell the user that there was bad input
            cout<<"Illegal expression!"<<endl;
    }
    return 0;
}
 
bool getInput(string &line)
{
    cout<<"In-fix expression: ";
    getline(cin, line);                           //Get infix expression
    fflush(stdin);                                //Clear input buffer
    for(unsigned int i = 0; i < line.size(); ++i) //standardize set names to uppercase
        line[i] = toupper(line[i]);
    return line != "";                            //see if the line was empty
}
 
int whoIsFirst(const string &incoming) //Convert operator to its precedence value
{
    int value = 0;
    switch(incoming[0])
    {
        case '!' : value = 3;          //Compliment is the highest
                   break;
        case '*' : value = 2;
                   break;
        case '+' :
        case '\\' : value = 1;          //Union and set difference are the lowest
    }
    return value;
}
 
bool illegalSet(string input)          //See if the user entered a double comma or something
{                                      //like {, or ;}
    unsigned int pos, size = input.size();
    while((pos = input.find(' ')) < size) //Find spaces
        input.erase(pos,1);
    return (input.find(",,") < size ||
            input.find("{,")  < size ||
            input.find(",}") < size ||
            input.find_first_not_of("0123456789{,}") < size);
}
 
bool precedence(const string &incoming, const string &tos) //Return TRUE is incoming operator
{
     return whoIsFirst(incoming) < whoIsFirst(tos);  //is less than what is on the top of the operator stack
}
 
bool convertToRPN(string input, string &output)
{
      vector<string>  operatorStack;                //Holds operators
      string  op, operand, parens;                  //Holds the current operator and operand
      output = "";                                  //Initialize output to empty
      unsigned int pos, pos2;                       //Used to find position of substrings within a string
      while(input.size() > 0)                       //As long as there is still input
      {
          if(input[0]>='A' && input[0] <= 'Z')      //Did we read the name of a set?
          {                                         //If so, move it directly to the output
              operand = input[0];
              output += operand + " ";
              input.erase(0,1);                     //Remove the operand from the input
          }
          else                                      //Otherwise
          {
            switch(input[0])                        //See what the operator "could" be
            {
                case ' ' : input.erase(0,1);        //Dump any space that has been read from the input
                           break;
                case '{' : pos = input.find('}');   //If it is a open curly brace, we have an unnamed set
                           pos2 = input.find('{', pos + 1);  //So see if it is a valid unnamed set
                           if(pos > input.size() || pos2 < pos)
                               return false;        //If not, signify that an invalid unnamed set was read in
                           parens = (input.substr(0,pos + 1)) + " "; //It is is valid, pull it out of the input
                           if(illegalSet(parens))   //Did they enter a double comma?
                               return false;
                           output += parens;        //Bad input!!
                           input.erase(0,pos + 1);  //and remove it from the input
                           break;
 
                case '*' :                          //If it is any valid operator
                case '\\' :                         //we either immediately push it onto the operand stack
                case '+' :                          //or push higher precedence operators currently in the stack
                case '!' : op = input[0];           //onto the output
                           while((operatorStack.size() > 0) && precedence(op, operatorStack[operatorStack.size()-1]))
                           {
                                output += operatorStack.back() + " ";
                                operatorStack.pop_back();
                           }
                           operatorStack.push_back(op); //Insert current operator onto operator stack
                           input.erase(0,1);            //Remove the current operator from input
                           break;
                case '(' : operatorStack.push_back("("); //Parenthesis are a "special case"
                           input.erase(0,1);             //Push the opening onto the operand stack and wait till
                           break;                        //a closing parentheses is found
                case ')' : while(operatorStack.size() > 0 && operatorStack.back() != "(")
                           {                             //Once found, keep pushing operators onto output
                              output += operatorStack.back() + " ";
                              operatorStack.pop_back();  //Until we either empty the stack or find a opening paren
                           }
                           if(operatorStack.size() == 0)
                               return false;
                           else
                               operatorStack.pop_back();
                           input.erase(0,1);
                           break;
                default  : return false;
            }
          }
        }
        while(operatorStack.size() > 0)  //If there are any additional operators left on the stack
        {                                //we push them onto output unless we find a mis-matched paren
            string op = operatorStack.back();
            if(op[0] == '(')
                return false;
            output += op + " ";
            operatorStack.pop_back();
        }
        return true;                      //Signify a successful conversion to RPN
}
 
 
void process(string rpn, int sets[], int index)    //Process the RPN on sets
{
    unsigned int result = 0, pos;       //Initialize result to 0 (or create a bitset to be your result holder)
    vector<string> operandStack;        //Create an operand and operator stack
    vector<char> operatorStack;
    string set, x, y, output;           //Create some temporary variables
    cout<<"Translated to RPN: "<<rpn<<endl;
    while(rpn.size() > 0)               //As long as there are inputs available
    {
        if(rpn[0] >= 'A' && rpn[0] <= 'Z') //If a named set, push onto the operand stack
        {
            operandStack.push_back(output = rpn[0]);
            rpn.erase(0,1);
        }
        else                                //Otherwise
        {
            switch(rpn[0])                  //See what the operator is
            {
                case ' ' :  rpn.erase(0,1); //Get rid of spaces
                            break;
                case '{' :  pos = rpn.find('}');//If curly braces, get the unnamed set
                            set = (rpn.substr(0, pos + 1));
                            operandStack.push_back(set); //and push it onto the operand stack
                            rpn.erase(0, pos+1);         //Then remove it from the RPN input
                            break;
                case '!' :  x = operandStack.back();     //If compliment operator
                            operandStack.pop_back();     //Pop an operand and
                            result = setCompliment(x, output); //compliment it
                            operandStack.push_back(output); //Push the result back onto the operand stack
                            rpn.erase(0,1);
                            break;
                case '+' :  x = operandStack.back();    //If it is Union, two operands are required
                            operandStack.pop_back();    //Pop them, then perform the union
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = unionOfTwoSets(x, y, output);
                            operandStack.push_back(output); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;
                case '*' :  x = operandStack.back();        //If it is Intersection, two operands are required
                            operandStack.pop_back();        //Pop them, then perform the intersection
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = intersectionOfTwoSets(x, y, output);//The place the result onto the operand stack
                            operandStack.push_back(output); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;
               case '\\' :  x = operandStack.back();        //If it is Set Difference, two operands are required
                            operandStack.pop_back();        //Pop them, then perform the set difference
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = differenceOfTwoSets(y, x, output);//The place the result onto the operand stack
                            operandStack.push_back(output); //Delete from input the operand
                            rpn.erase(0,1);
                            break;
            }
        }
    }

    //assign the elements to a set with index
    sets[index] = result;
}
 
 
//The functions below are for you to complete. You can use bitset or ints, but you will have to
//adjust the function above to work correctly with bitsets
unsigned int unionOfTwoSets(string x, string y, string &result)
{
 
}
 
unsigned int intersectionOfTwoSets(string x, string y, string &result)
{
 
}
 
unsigned int differenceOfTwoSets(string x, string y, string &result)
{
 
}
 
unsigned int setCompliment(string x, string &result)
{
 
}


/* This function will read the command(SET) 
from the user's input. If it's SET command, get the right expression
assign to string array. Then get the first letter to determine the index of a set 
If there is not "SET" command or "equal" sign, print an error message as an invalid input
    @index : index of a set in the sets array sets[26]
    @equalPos: return the position of "=" sign if there is "SET" command
*/

int userHelper(string &input, int &equalPos)
{
    unsigned int posSet = input.find("SET");
    // unsigned int posHelp = input.find("HELP");
    // unsigned int posList = input.find("LIST");
    unsigned int posEqual = input.find("=");
    string strSet = "";
    int index = 0; //index of a set

    // // HELP command
    // if (posHelp < input.size())
    //     index = 0;
    // // LIST command
    // else if (posList < input.size())
    //     index = 1;

    //  SET command
    if (posSet < input.size() && posEqual < input.size())
    {
        // get a string between SET command and "=" sign
        strSet = input.substr(posSet + 3, posEqual + 1);
        while (strSet[0] == ' ')
            strSet.erase(0, 1); //remove leading spaces

        //find letter index
        if (strSet[0] >= 'A' || strSet[0] <= 'Z')
        {
            index = (int(strSet[0]) - 65); //get an index of a set
            equalPos = posEqual;
            input = input.substr(posEqual+1, input.size()); // get the right expression
            cout << "index of the set is " << index 
                 << "\n char is " << strSet[0] << ". "
                 << "Equal sign at pos is " << equalPos << endl;
        }
    }
    else
        return -1;//invalid input command

    return index; //not found SET, HELP, LIST
}

/* Helper will convert the set of numbers to int and assign to a bit set.
    Each of index in the bit set represents for that number.
*/

int setHelper
{
    
}