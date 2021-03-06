/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  Final Project
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <bitset>
#include <string>
#include <array>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <utility>
#include <math.h>
#include "operation.h"
#include "parsing.h"

using namespace std;

bool illegalSet(string input,string validSet);
int whoIsFirst(const string &incoming);
bool precedence(const string &incoming, const string &tos);
bool convertToRPN(string input, string& output);
bool getInput(string &line);
bool process(string rpn, string sets[], int index);
bool commandInput(string &input, string sets[],bool& hasSaved,bool& checkEmpty);
bool commandMatching(string input,string commandName,unsigned int size);
bool commandHelper(string input,unsigned int& index);
void removeSpace(string& input);
bool helpCommand(string input);
bool checkFileName(string fileName);
bool saveHelper(string fileName, string sets[],bool& checkSaved);
bool letCommand(string &input, string sets[], bool &isEmpty, bool &checkSaved);
bool saveCommand(string sets[], string &input, bool &checkSaved, bool &isEmpty);
void loadHelper(string sets[],string fileName,bool& isEmpty,bool& hasSaved);
bool loadCommand(string sets[], string input,bool& isEmpty,bool& hasSaved);
bool showCommand(string *sets,string input);
void saveFile(string fileName,string sets[],bool& hasSaved);
void exitCommand(string sets[],bool hasSaved,bool isEmpty);
bool listCommand(string sets[], string input);
bool question(string title);
void leadingTrailing(string &input);
bool isFraction(string &input, bool noSpace, unsigned long long pos, unsigned long long &fracPos);
bool notNumOrLetter(char i);
void removeTrailing(string &exp, unsigned long long pos);
bool notLegalChar(char i,string invalidSet);
void readNum(string &input,string &operand);
void error(string message);
void trimZeros(string &input);
bool denomZero(string denom);
void wExitCommand(string sets[]);
bool editCommand(string sets[], string input, bool &isEmpty);
bool isLetter(string input);
void combinationCommand(string input);
void permutationCommand(string input);
void gCDCommand(string input);

int main(int argc, char *argv[])
{

    string line, output="";            //Create input (line) and output (output) variables for functions to use
    string sets[26] = {};              //Create a 26 element array of sets
    bool hasSaved = false;          //Check SAVE before exiting program
    bool isEmpty = true;            //Check if the set is empty 
    // for(unsigned int i=0;i<26;++i)
    //     for(unsigned j=0;j<2;j++)             //initialize sets' value
    //         sets[i][j] = "^"
    for(unsigned int i=0;i<26;++i)
        sets[i] = "^";
    while(getInput(line))
    {
        // parsing(line);
        // if(convertToRPN(line,output))
        // {
        //     process(output, sets, 1);
        //     cout<<"Result: "<<sets[1]<<endl;
        // }   
        if(commandInput(line, sets,hasSaved,isEmpty)) //See if we can convert infix to postfix notation
                cout << "--------------" << endl; 
        else
        {
            cout << "Invalid command!!!. Please type HELP for instructions" << endl;
            cout << "--------\n";
        }
    }
    // cout << "Illegal command! Please type HELP for the instructions." << endl;

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

int whoIsFirst(const string &incoming) //Convert operator to its precedence value
{
    int value = 0;
    switch(incoming[0])
    {
        case '!' : value = 5;          //factorial has highest precedence
                   break;
        case '$' : value = 4;          //mixed number
                   break;
        case '~' : value = 3;          //fraction
                   break;
        case '*' : 
        case '/' : value = 2;
                   break;
        case '+' :
        case '-' : value = 1;          //Union and set difference are the lowest
    }
    return value;
}

bool illegalSet(string input,string validSet)          //See if the user entered a double comma or something
{                                      //like {, or ;}
    unsigned long long pos, size = input.size();
    // while((pos = input.find(' ')) < size) //Find spaces
    //     input.erase(pos,1);
    return (input.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/!~# ") < size);
}

bool precedence(const string &incoming, const string &tos) //Return TRUE is incoming operator
{
     return whoIsFirst(incoming) < whoIsFirst(tos);  //is less than what is on the top of the operator stack
}

bool convertToRPN(string input, string &output)
{
    vector<string>  operatorStack;                      //Holds operators
    unsigned int numOp = 0;                             //number of operator
    unsigned long long fracPos = 0;                             
    bool number,letter = false;
    string  op, operand, parens, getNum;                //Holds the current operator and operand
    output = "";                                        //Initialize output to empty   

    while(input.size() > 0)                       //As long as there is still input
    {
        if(input[0]>='A' && input[0] <= 'Z')      //Did we read the name of a set?
        {                                         //If so, move it directly to the output
            operand = input[0];
            output += operand + " ";
            input.erase(0,1);                     //Remove the operand from the input
        }
        else if(input[0]>='0' && input[0] <= '9')        
        {
            // getNum(input,operand);
            operand = "";
            while (input[0] >= '0' && input[0] <= '9')
            {
                operand += input[0];
                input.erase(0, 1);
            }
            output += operand + " ";
            number = true;
        }        
        else
        {
            switch(input[0])                        //See what the operator "could" be
            {
                case '#' :  operand = input[0];
                            output += operand+ ' ';    //unary operator
                            input.erase(0,1);
                            break;
                case ' ' :  if(!isFraction(input,false,0,fracPos))
                                return false;                       //invalid fraction format
                            else
                            {
                                input[fracPos] = '$';               //mixed number appear
                                cout<<"Exp changed: "<<input<<endl;
                            }
                            break;
                case '$' :
                case '~' :
                case '!' :
                case '*' :                          //If it is any valid operator
                case '/' :                         //we either immediately push it onto the operand stack
                case '+' :                          //or push higher precedence operators currently in the stack
                case '-' :  op = input[0];           //onto the output
                            numOp++;
                            // if(numOp++ > 1 && number) return false; //only one operator accept if expression is all numbers
                            //handle fraction
                            if(op == "/")
                            {                           //v-start after oprator '/'
                                if(!isFraction(input,true,1,fracPos)) return false; //invalid fraction input
                                input[fracPos] = '~';
                                break;
                            }                        
                            while((operatorStack.size() > 0) && precedence(op, operatorStack[operatorStack.size()-1]))
                            {
                                output += operatorStack.back() + " ";
                                operatorStack.pop_back();
                            }
                            operatorStack.push_back(op); //Insert current operator onto operator stack
                            input.erase(0,1);            //Remove the current operator from input
                            break;
                case '(' :  operatorStack.push_back("("); //Parenthesis are a "special case"
                            input.erase(0,1);             //Push the opening onto the operand stack and wait till
                            break;                        //a closing parentheses is found
                case ')' :  while(operatorStack.size() > 0 && operatorStack.back() != "(")
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

bool process(string rpn, string sets[], int index) //Process the RPN on sets
{
    vector<string> operandStack;        //Create an operand and operator stack
    vector<char> operatorStack;
    string set, x, y, z, output ="", result = "";           //Create some temporary variables
    cout<<"Translated to RPN: "<< rpn <<endl;
    while(rpn.size() > 0)               //As long as there are inputs available
    {
        if(rpn[0] >= 'A' && rpn[0] <= 'Z') //If a named set, push onto the operand stack
        {
            string operand = sets[rpn[0]-'A'] + "";
            operandStack.push_back(operand);
            // result = sets[((int)(rpn[0])) -65];
            rpn.erase(0,1);
        }
        else if(rpn[0] >= '0' && rpn[0] <= '9')
        {
            string num = "";
            while (rpn[0] >= '0' && rpn[0] <= '9')
            {
                num += rpn[0];
                rpn.erase(0, 1);
            }
            operandStack.push_back(num);
        }
        else                                //Otherwise
        {
            switch(rpn[0])                  //See what the operator is
            {
                case ' ' :  rpn.erase(0,1); //Get rid of spaces
                            break;

                case '#' :  x = operandStack.back();
                            operandStack.pop_back();
                            if(!isLetter(x))
                            {
                                x = sets[x[0]-'A'];
                                x = '-' + x; //add '-' front of number
                            }
                            else
                                x = '-' + x;                
                            operandStack.push_back(x);
                            rpn.erase(0,1);
                            break;             

                case '!' :  x = operandStack.back();    //factorial
                            operandStack.pop_back();
                            if(!isLetter(x))
                                x = sets[x[0]-'A'];
                            if(!factorial(x,result))    //process and put value to result
                            {
                                error("Factorial can't be negative or fraction");
                                return false; 
                            }    
                            operandStack.push_back(result);   
                            rpn.erase(0,1);
                            break; 

                case '+' :  x = operandStack.back();    //If it is Union, two operands are required
                            operandStack.pop_back();    //Pop them, then perform the union
                            if (!isLetter(x))
                                x = sets[x[0] - 'A'];
                            y = operandStack.back();
                            operandStack.pop_back();
                            if (!isLetter(y))
                                y = sets[y[0] - 'A'];
                            result = checkFrac(x) || checkFrac(y) ?
                                        addingFrac(y,x) : addition(y, x);

                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;

                case '-' :  x = operandStack.back();    //If it is Union, two operands are required
                            operandStack.pop_back();    //Pop them, then perform the union
                            if (!isLetter(x))
                                x = sets[x[0] - 'A'];
                            y = operandStack.back();
                            operandStack.pop_back();
                            if (!isLetter(y))
                                y = sets[y[0] - 'A'];
                            result = checkFrac(x) || checkFrac(y) ?
                                        subtractFrac(y,x) :subtract(y, x);
                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;

                case '~':   x = operandStack.back();        //denominator
                            operandStack.pop_back();
                            if (!isLetter(x))
                                x = sets[x[0] - 'A'];
                            y = operandStack.back();        //numerator
                            operandStack.pop_back();
                            if (!isLetter(y))
                                y = sets[y[0] - 'A'];
                            if(denomZero(x))
                                return false;               //denominator is zero
                            result = y + "/" + x;
                            result = reduceFraction(result);
                            operandStack.push_back(result); //Push the result back onto the operand stack
                            rpn.erase(0, 1);
                            break;

                case '$':   x = operandStack.back();        //denominator
                            operandStack.pop_back();
                            if (!isLetter(x))
                                x = sets[x[0] - 'A'];
                            y = operandStack.back();        //numerator
                            operandStack.pop_back();
                            if (!isLetter(y))
                                y = sets[y[0] - 'A'];
                            z = operandStack.back();        //mixed number
                            operandStack.pop_back();
                            if (!isLetter(z))
                                z = sets[z[0] - 'A'];
                            y = addition(multiply(z,x),y);  //z*x+y convert to numerator
                            if(denomZero(x))
                                return false;               //denominator is zero
                            result = y + "/" + x;
                            result = reduceFraction(result);
                            operandStack.push_back(result);
                            rpn.erase(0,1);
                            break;

                case '*' :  x = operandStack.back();        
                            operandStack.pop_back();
                            if (!isLetter(x))
                                x = sets[x[0] - 'A'];
                            y = operandStack.back();
                            operandStack.pop_back();
                            if (!isLetter(y))
                                y = sets[y[0] - 'A'];
                            result = checkFrac(x) || checkFrac(y) ?
                                        multiplyFrac(y,x) : multiply(y,x);;
                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;
            }
        }
    }
    //assign the elements to a set with index
    sets[index] = result;
}//process

/*  This function will read the command(SET)
    from the user's input. If it's SET command, get the right expression
    assign to string array. Then get the first letter to determine the index of a set.
    If there is not "SET" command or "equal" sign, print an error message as an invalid input
    @index : index of a set in the sets array sets[26]
    @equalPos: return the position of "=" sign if there is "SET" command
*/

bool letCommand(string &input, string sets[],bool& isEmpty,bool& checkSaved)
{
    unsigned int posSet = input.find("LET");
    unsigned int posEqual = input.find("=");
    int index = -1; //index of a set
    string output = "";                                 //result after process RPN expression
    string strSet = "";
    string newStrSet;
    string expression = input.substr(posEqual+1);       //get an expression
    //  get a letter of a LET command
    if(posSet < input.size() && posEqual < input.size())
    {
        // get a string between LET command and "=" sign
        strSet = input.substr(posSet + 3, posEqual + 1);
        newStrSet = strSet.substr(posSet, posEqual);
        // if(newStrSet[1] != '=')
        // {
        //     return false;
        // }
        removeSpace(strSet); //remove leading spaces

        //find letter index
        if((int)strSet[0] >= 65 && (int)strSet[0] <= 90)
        {
            index = (int(strSet[0]) - 65); //get an index of a set
        }
        else
            return false;
    }
    else
        return false;   //invalid command

    unsigned int check1 = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    unsigned int check2 = expression.find_first_of("0123456789");
    if(check1 < expression.size() && check2 < expression.size())
        return false; //can not mixed between letter and number
    
    input = input.substr(posEqual+1); // get the right expression
    //parsing expression to reduce the streak '-' and '+'
    parsing(input);
    if(!convertToRPN(input,output))
        return false;
    process(output,sets,index);
    // {
    //     cout<<"WRONG HERE"<<endl;
    //     return false;
    // }
    isEmpty = false;                //turn on Check Empty set
    checkSaved = false;//need to save before exiting
    return true;  //valid command
}


//remove all spaces in input string
void removeSpace(string& input)
{
    int pos = 0;
    while((pos = input.find(' ')) < input.size())
    {
        input.erase(pos,1);
    }
}

//compare user's command input to default command
//return true if matching
bool commandMatching(string input,string commandName,unsigned int size)
{
    // cout << "Substring is: "<<input.substr(0,size)<<endl;
    return (input.substr(0,size) == commandName);
}

/*  Find a first letter matches to first letter from one of commands in the list(LIST,HELP,SET,LOAD,SAVE)
    Check the rest of letters. If it matches return true
    otherwise return false
*/
bool commandHelper(string input,unsigned int& index)
{
    while(input.size()>0)
    {
        switch(input[0])
        {
            case ' ' :  input.erase(0,1);
                        break;
            case 'L' :  if(commandMatching(input,"LET",3))
                        {
                            index = 0;          //NEW
                            return true;
                        }
                        else if(commandMatching(input,"LOAD",4))
                        {
                            index = 4;          //LOAD
                            return true;
                        }
                        else if(commandMatching(input, "LIST", 4))
                        {
                            index = 8; //LIST
                            return true;
                        }
            case 'S' :  if(commandMatching(input,"SAVE",4))
                        {
                            index = 1;          //SAVE
                            return true;
                        }
                        else if(commandMatching(input, "SHOW", 4))
                        {
                          index = 3; //LIST
                          return true;
                        }
                        else return false;
            case 'H' :  if(commandMatching(input,"HELP",4))
                        {
                            index = 2;          //HELP
                            return true;
                        }
                        else return false;
            case 'Q' :  if(commandMatching(input,"QUIT",4))
                        {
                            index = 5;          //QUIT
                            return true;                           
                        }
                        else return false;                        
            case 'E' :  if(commandMatching(input,"EDIT",4))
                        {
                            index = 6;          //EDIT
                            return true;                           
                        }
                        else if(commandMatching(input,"EXIT",4))
                        {
                            index = 5;          //EXIT
                            return true;                           
                        }
                        return false;
            case 'W' :  if(commandMatching(input,"WEXIT",5) || commandMatching(input,"WQUIT",5))
                        {
                            index = 7;          //WEXIT WQUIT
                            return true;                           
                        }
                        else return false;
            case 'C' :  if(commandMatching(input,"C(",2))
                        {
                            index = 9; //Combination
                            return true;
                        }
                        else return false;
            
            case 'P':   if (commandMatching(input, "P(", 2))
                        {
                            index = 10; //Permutation
                            return true;
                        }
                        else
                            return false;
            case 'G': if (commandMatching(input, "GCD", 3))
                        {
                            index = 11; //Greatest common divisor
                            return true;
                        }
                        else
                            return false;

            default:    return false;   //invalid command
        }
    }
}

/*  Call commandHelper to parse the command
    If the command doesn't match the listed command keyword
    return false.
 */
bool commandInput(string &input, string sets[],bool& hasSaved,bool& isEmpty)
{
    unsigned int index = -1;        //index of command
    //check valid command
    if(!commandHelper(input,index))
        return false;


    switch (index)
    {
        case 0:
            return letCommand(input,sets,isEmpty,hasSaved);
            break;
        case 1:
            return saveCommand(sets,input,hasSaved,isEmpty);
            break;
        case 2:
            return helpCommand(input);
            break;
        case 3:
            return showCommand(sets,input);
            break;
        case 4:
            return loadCommand(sets,input,isEmpty,hasSaved);
            break;
        case 5:
            exitCommand(sets,hasSaved,isEmpty);
            return true;
            break;
        // case 6:
        //     return editCommand(sets, input,isEmpty);
        //     break;
        case 7:
            wExitCommand(sets);
            return true;
            break;
        case 8:
            return listCommand(sets, input);
            break;
        case 9:
            combinationCommand(input);
            return true;
            break;
        case 10:
            permutationCommand(input);
            return true;
            break;
        case 11:
            gCDCommand(input);
            return true;
            break;
        default:
            return false;   //invalid command
    }
}




bool helpCommand(string input)
{
    string fileName = "final.help";
    if(checkFileName(fileName))
    {
        ifstream opFile(fileName);
        unsigned int pos = 0;
        string token = "";
        if(opFile.is_open())
        {
            while(getline(opFile,token,';'))
                cout << token;
            cout << endl;
            opFile.close();
        }
        return true;
    }
    return false;//invalid command
}


//check if File Name already exist
//return true if exists ; otherwise false
bool checkFileName(string fileName)
{
    ifstream file(fileName);
    return (bool)file;
}


//store all expressions if file doesn't exist
//otherwise rename or remove that file
bool saveCommand(string sets[],string& input,bool& checkSaved,bool& isEmpty)
{
    unsigned int pos = input.find("SAVE");
    string fileName = input.substr(pos+4);          //get fileName
    removeSpace(fileName);
    if(isEmpty)
    {
        cout << "The list of expressions is empty." << endl;
        return true;
    }
    else if(fileName == "")
    {
        cout << "Missing the file name." <<endl;
        return true;
    }
    return saveHelper(fileName,sets,checkSaved);
    // return true;
}



bool question(string title)
{
    string line;
    cout << title;
    getline(cin, line);
    transform(line.begin(), line.end(), line.begin(), ::toupper);
    return (line == "Y" || line == "YES");
}


//Create a new file
//if file already exists, overwrite it or give another file name
//return true if checkFileName
bool saveHelper(string fileName, string sets[],bool& hasSaved)
{
    string ans = "";
    removeSpace(fileName);
    //add extension to file if missing
    //if exist lowercase extensions
    //if fileName exists
    if(checkFileName(fileName))
    {
        cout << "File already exists" << endl;
        //erase file
        if(question("Would you like to overwrite file(Y/N): "))
        {
            remove(fileName.c_str());                                   //remove the file
            cout << "Old file is overwritten succesfully." << endl;
            saveFile(fileName,sets,hasSaved);                                    //write a new file
            return true;
        }
        else if(question("Would you like to rename file(Y/N): "))
        {
            cout << "Enter new file name: " << endl;
            getline(cin, ans);
            removeSpace(ans);
            transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
            //check extension missing
            while(checkFileName(ans))
            {
                cout << "File already exists. Please enter another name:" << endl;
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
            }
            rename(fileName.c_str(), ans.c_str());
            cout << "File successfully renamed." << endl;
            return true;
          }
          else if(question("Would you like to create new file(Y/N): "))
          {
              cout << "Enter new file name: " << endl;
              getline(cin, ans);
              removeSpace(ans);
              transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
              //check extension missing
              while(checkFileName(ans))
              {
                  cout << "File already exists. Please enter another name:" << endl;
                  getline(cin, ans);
                  transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
              }
              saveFile(ans,sets,hasSaved);
              cout << "File successfully created." << endl;

          }
          else if(question("Would you like to erase the current file(Y/N): "))
          {
            remove(fileName.c_str());
            cout << "File successfully erased." << endl;
            return true;
          }
          return true;
    }
    else
    {
        saveFile(fileName,sets,hasSaved);
        return true;
    }
}


//LOAD command
bool loadCommand(string sets[], string input,bool& isEmpty,bool& hasSaved)
{
    unsigned int pos = input.find("LOAD");
    string fileName = input.substr(pos + 4); //get fileName
    removeSpace(fileName);
    if(fileName == "")
    {
        cout << "Missing the file name." << endl;
        return true;//invalid command
    }
    loadHelper(sets,fileName,isEmpty,hasSaved);
    return true;
}

//check the file name
//if  the file exist read through the text file.
//token it by ";" delimeter and assign each number to an array element of sets[]
void loadHelper(string sets[],string fileName,bool& isEmpty,bool& hasSaved)
{
    string ans = "";
    removeSpace(fileName);
    //add extension to file if missing
    //if exist lowercase extensions

    //check file exist
    if(!checkFileName(fileName))
        cout << "The file name doesn't exist." << endl;
    else if(fileName.size() == 0)
        cout << "Missing file name." <<endl;
    else
    {
        string line;
        ifstream opFile(fileName);
        unsigned int index = 0;
        string rpn = "";
        if (opFile.is_open())
        {
            while (getline(opFile, line,';'))
            {
                sets[index++] = line;
            }
            isEmpty = false;                        //Turn off isEmpty
            hasSaved = true;                        //Saved the current state
            cout<<"File already is already loaded."<<endl;
            opFile.close();
        }
        else
            cout << "Unable to open file" << endl;
    }
}


//print a particular set
bool showCommand(string *sets, string input)
{
    int pos = 0;                           //get LIST index in the input string
    pos = input.find("SHOW");
    string command = input.substr(pos,4);   //get "SHOW" string
    string setIndex = input.substr(pos+4);  //get set number want to see the content
    removeSpace(setIndex);
    if(setIndex == "")
    {
        cout<<"Missing the name of a set."<<endl;
        return true;
    }
    else
    {
        char setName = setIndex[0]; //set name in char
        if(setName<'A' || setName>'Z')
            cout<<"Index of the set is invalid or out of range (A-Z)."<<endl;
        else
        {
            if(sets[setName-'A'] != "^")
            {
                cout << setName << " = " <<sets[setName-'A']<<endl;
            }
            else
                cout<<"The set "<<setName<<" is empty."<<endl;
        }
        return true;
    }
    return false;    //command valid
}

//print list of expressions
bool listCommand(string sets[], string input)
{
    int pos = 0; //get LIST index in the input string
    removeSpace(input);
    pos = input.find("LIST");
    string getList = input.substr(pos);
    //no other parameter after or before LIST
    if (getList != "LIST" && getList.size() != 4)
    {
        cout << "Command LIST is ambiguous." << endl;
        return true;
    }
    else
    {
        unsigned int setName = 65; //set name in char
        for (int i = 0; i < 26; ++i)
        {
            string setValue = sets[i]+"";
            if (setValue != "^")
            {
                cout << char(setName + i) << " = " << sets[i]<<endl;
            }
        }
        return true;
    }
    return false; //command valid
}




/*
    Create a file with fileName,
    Loop through the sets[], write each element into the file, delimited by ";"
*/
void saveFile(string fileName,string sets[],bool& hasSaved)
{
    ofstream myFile;
    myFile.open(fileName,ios::trunc); //append to the expression to the end
    for(int i=0;i<26;++i)
    {
        string num= sets[i] + ";";
        myFile << num ;
    }
    hasSaved = true;    //turn SAVED flag on to detect EXIT WITHOUT SAVE
    myFile.close(); //close the file
}

void exitCommand(string sets[],bool hasSaved,bool isEmpty)
{
    string ans = "";

    //ask you to save before exit
    if(!hasSaved && !isEmpty)
    {
        cout << "Would you like to save your changes(Y/N): ";
        getline(cin,ans);
        transform(ans.begin(),ans.end(),ans.begin(),::toupper);         //convert to upper case
        if(ans == "Y" || ans == "YES")
        {
            string fileName = "";
            cout << "Please enter your file name: ";
            getline(cin,fileName);
            transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
            //call saveHelper to process save command
            saveHelper(fileName,sets,hasSaved);
        }
    }
    else
        exit(0);    //totalExpression is empty
    exit(0);//close program
}


//remove leading and trailing space before convert RPN
void leadingTrailing(string &input)
{
    int count = 0;
    while(input.size() > 0 && input[count++] == ' ')
        input.erase(0,1);
    while(input.size() > 0 && input[input.size()-1] == ' ')
        input.erase(input.size()-1);
}

// /*  Mixed number format is A B/C. If after space isn't a number, return false
//     Convert mixed number to fraction, and reduce it
//     If it a fraction number, the last digit must follow by A-Z,0-9,+,-,*,/,),(
//     @noSpace: not a mixed number
//     @pos: position at a space or '/'
//     Return true if a fraction
//  */
bool isFraction(string &input, bool noSpace, unsigned long long pos,unsigned long long &fracPos)
{
    //mixed number
    if(!noSpace) 
    {
        input.erase(0,1);                           //remove space after mixed number
        //check numerator (no expression included)
        if(notNumOrLetter(input[pos]))
            return false; //not a mixed number
        //keep reading digits until see a space
        while((input[pos] >= '0' && input[pos] <= '9') || (input[pos] >= 'A' && input[pos] <= 'B')){pos++;}
        removeTrailing(input,pos);              //remove extra spaces following numerator
        //Must be a '/' before first digits of denominator -- A    /B
        if(input[pos++] != '/')   
            return false;                       //it's not fraction. following numerator must be '/'
        else
            return isFraction(input,true,pos,fracPos);  //same pattern as fraction A/B
    }
    else    //regular fraction, identify denominator
    {
        //denominator is a expression
        if(input[pos] == '(')
        {
            unsigned long long closeBracket = input.find_first_of(")",pos+1);    
            if(closeBracket > input.size()) return false;                   //not found
            cout<<"Fall in bracket\n";
            // removeTrailing(input,closeBracket+1);                       //remove space after ')'
            // return !notNumOrLetter(input[closeBracket+1]);               //return false if it's a number or letter
        }
        else //denominator NOT an expression
        {
            removeTrailing(input, pos);     //remove trailing space after '/' A/    B
            if (notNumOrLetter(input[pos])) //invalid fraction A/   +-/*(
                return false;               //must be a letter or number follow after
            while ((input[pos] >= '0' && input[pos] <= '9') || (input[pos] >= 'A' && input[pos] <= 'B'))
            {
                pos++;
            } //otherwise find the last digit of denominator
            //after last digit of denominator must be '!'23  or and operator
            if (input[pos] == '!' && input[pos + 1] == '(')
                return false;            //imbigous input A/B!(
            else if (pos < input.size()) //invalid fraction if following denominator is not an operator
            {
                if (notLegalChar(input[pos], "+-*/#! "))
                    return false;
            }
        }
        fracPos = input.find_first_of("/");
        return true;
    }
}


bool notNumOrLetter(char i)
{
    string input;
    input += i;
    return input.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") < input.size();
}

//remove trailing space
void removeTrailing(string &exp, unsigned long long pos)
{
    while (pos < exp.size() && exp[pos] == ' ')
        exp.erase(pos++, 1);
}


//identify a char appear or not in a specific set characters
bool notLegalChar(char i,string invalidSet)
{
    string input;
    input += i;
    return input.find_first_not_of(invalidSet) < input.size();
}


//read and get a number from string digits
void readNum(string &input, string &num)
{
    num = "";
    while(input.size()>0 && (input[0] >= '0' && input[0] <= '9'))
    {
        num += input[0];
        input.erase(0,1);
    }
    num+= " ";
}

//inform error operation or invalid expression
void error(string message)
{
    cout<<message<<endl;
}

//trim zero from left to right
void trimZeros(string &input)
{
    while(input.size() > 1 && input[0] == '0')
        input.erase(0,1);
}

//return true if denominator is zero
bool denomZero(string denom)
{
    trimZeros(denom);
    if(denom == "0") 
    {
        error("The denominator can't be zero.");
        return false;
    }
}

//write a file and exit
void wExitCommand(string sets[])
{
    string fileName = "UNTITLED.truth";
    //check file exits
    if(!checkFileName(fileName))
    {
        ofstream myFile;
        myFile.open(fileName,ios::app); //append to the expression to the end
        for(int i=0;i<26;++i)
        {
            string temp = sets[i] + ";" ;
            myFile << temp ;
        }
        myFile.close(); //close the file
    }
    else
    {
        unsigned int hashNum = (rand()%100)+99;
        string newFile = "UNTITLED"+to_string(hashNum)+".truth";
        while(checkFileName(newFile))                  //create new file name 
        {
            hashNum = (rand()%100)+99;
            newFile = "UNTITLED"+to_string(hashNum)+".truth";
        }
        ofstream myFile;
        myFile.open(newFile,ios::app); //append to the expression to the end
        for(int i=0;i<26;++i)
        {
            string temp = sets[i] + ";" ;
            myFile << temp ;
        }
        myFile.close(); //close the file
    }
    exit(0);    //close program
}


bool editCommand(string input,string sets[],bool& isEmpty)
{
    unsigned int pos = input.find("EDIT");
    string indexExp = input.substr(pos + 4); //get index of expression need to be deleted
    removeSpace(indexExp);
    if (isEmpty)
    {
        cout << "There doesn't have any expression to delete." << endl;
        return true;
    }
    if ((indexExp.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ")) < indexExp.size())
    {
        cout << "The index expression is invalid." << endl;
        return true;
    }
    else
    {
        string answer = "";
        string output = "";
        cout << "Please enter new expression: " ;
        getline(cin,answer);
        transform(answer.begin(),answer.end(),answer.begin(),::toupper);    //convert to upper case
        while(!convertToRPN(answer,output))
        {
            cout << "Invalid expression. Please enter again: ";
            getline(cin, answer);
            transform(answer.begin(),answer.end(),answer.begin(),::toupper);
        }
        // //clear old RPN and old expression
        sets[indexExp[0]-'A'] = "";
        // totalExpression[index-1][0] = "";
        // totalExpression[index-1][1] = "";
        // //insert new RPN and new expression
        // totalExpression[index-1][0] += answer;
        // totalExpression[index-1][1] += output;
        sets[indexExp[0]-'A'] = answer;
        return true;
    }
    // else
    // {
    //     cout << "The number of expressions is less than index." << endl;
    //     return true;
    // }
    return false; //invalid input
}

bool isLetter(string input)
{
    return input.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") < input.size();
}

void combinationCommand(string input)
{
    int pos1 = input.find_first_of("(");
    unsigned long long pos2 = input.find_first_of(")");
    unsigned long long comma = input.find_first_of(",");
    string firstNum = input.substr(pos1+1,comma);
    string secondNum = input.substr(comma+1,pos2-1);
    cout <<"Firstnum "<<firstNum<<" SecondNum "<<secondNum<<endl;
    string result = permutation(firstNum,secondNum);
    cout<<firstNum<<"C"<<secondNum<<": "<<result;
}
void permutationCommand(string input)
{
    int pos1 = input.find_first_of("(");
    unsigned long long pos2 = input.find_first_of(")");
    unsigned long long comma = input.find_first_of(",");
    string firstNum = input.substr(pos1+1, comma);
    string secondNum = input.substr(comma + 1, pos2-1);
    cout << "Firstnum " << firstNum << " SecondNum " << secondNum << endl;
    string result = combination(firstNum, secondNum);
    cout << firstNum << "P" << secondNum << ": " << result;
}

void gCDCommand(string input)
{
    unsigned int gcdKey = input.find("GCD");
    int pos1 = input.find_first_of("(");
    unsigned long long pos2 = input.find_first_of(")");
    unsigned long long comma = input.find_first_of(",");
    string firstNum = input.substr(pos1 + 1, comma);
    string secondNum = input.substr(comma + 1, pos2-1);
    cout << "Firstnum " << firstNum << " SecondNum " << secondNum << endl;
    string result = gcd(firstNum,secondNum);
    cout << "GCD is: "<<result;
}