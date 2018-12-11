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

bool illegalSet(string input);
int whoIsFirst(const string &incoming);
bool precedence(const string &incoming, const string &tos);
bool convertToRPN(string input, string& output);
bool getInput(string &line);
bool process(string rpn, string sets[], int index);
bool commandInput(string &input, int sets[],bool& hasSaved,bool& checkEmpty);
bool commandMatching(string input,string commandName,unsigned int size);
bool commandHelper(string input,unsigned int& index);
void removeSpace(string& input);
bool helpCommand(string input);
bool checkFileName(string fileName);
bool saveHelper(string fileName, int sets[],bool& checkSaved);
bool setHelper(string& input,unsigned int& setNum);
bool setCommand(string &input, int sets[], bool &isEmpty, bool &checkSaved) ;
bool saveCommand(int sets[], string &input, bool &checkSaved, bool &isEmpty);
void loadHelper(int sets[],string fileName,bool& isEmpty,bool& hasSaved);
bool loadCommand(int sets[], string input,bool& isEmpty,bool& hasSaved);
void showHelper(const int &num);
bool showCommand(int *sets,string input);
void saveFile(string fileName,int sets[],bool& hasSaved);
void exitCommand(int sets[],bool hasSaved,bool isEmpty);
bool listCommand(int *sets, string input);
bool question(string title);
void leadingTrailing(string &input);
bool isFraction(string &input, bool noSpace, unsigned long long pos, unsigned long long &fracPos);
bool notNumOrLetter(char i);
void removeTrailing(string &exp, unsigned long long pos);
bool notLegalChar(char i,string invalidSet);
void getNum(string &input,string &operand);
void error(string message);
void trimZeros(string &input);

int main(int argc, char *argv[])
{

    string line, output="";            //Create input (line) and output (output) variables for functions to use
    string sets[26] = {};              //Create a 26 element array of sets
    bool hasSaved = false;          //Check SAVE before exiting program
    bool isEmpty = true;            //Check if the set is empty 
    bool inputCheck = false;
    for(int i=0;i<26;++i)          //intial value for each setset
        sets[i]= -1;
        
    while(getInput(line))
    {
        parsing(line);
        cout<<line<<endl;
        if(convertToRPN(line,output))
        {
            process(output, sets, 1);
            cout<<"Result: "<<sets[1]<<endl;
        }    
        else
            cout<<"Invalid expression\n";
        cout << "--------\n";
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

bool illegalSet(string input)          //See if the user entered a double comma or something
{                                      //like {, or ;}
    unsigned int pos, size = input.size();
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
            operandStack.push_back(output = rpn[0]);
            result = sets[((int)(rpn[0])) -65];
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
                            x = '-' + x;                //add '-' front of number
                            operandStack.push_back(x);
                            rpn.erase(0,1);
                            break;             

                case '!' :  x = operandStack.back();    //factorial
                            operandStack.pop_back();
                            if(!factorial(x,result))    //process and put value to result
                            {
                                error("Factorial can't be negative");
                                return false; 
                            }    
                            operandStack.push_back(result);   
                            rpn.erase(0,1);
                            break; 

                case '+' :  x = operandStack.back();    //If it is Union, two operands are required
                            operandStack.pop_back();    //Pop them, then perform the union
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = addition(y, x);
                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;

                case '-' :  x = operandStack.back();    //If it is Union, two operands are required
                            operandStack.pop_back();    //Pop them, then perform the union
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = subtract(y, x);
                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;

                case '~':   x = operandStack.back();        //denominator
                            operandStack.pop_back(); 
                            y = operandStack.back();        //numerator
                            operandStack.pop_back();
                            trimZeros(x);                   //remove leading zeros
                            if(x == "0") 
                            {
                                error("The denominator can't be zero.");
                                return false;
                            }
                            result = y + "/" + x;
                            result = reduceFraction(result);
                            operandStack.push_back(result); //Push the result back onto the operand stack
                            rpn.erase(0, 1);
                            break;

                case '$':   x = operandStack.back();        //denominator
                            operandStack.pop_back();
                            y = operandStack.back();        //numerator
                            operandStack.pop_back();
                            z = operandStack.back();        //mixed number
                            operandStack.pop_back();
                            y = addition(multiply(z,x),y);  //z*x+y convert to numerator
                            result = y + "/" + x;
                            result = reduceFraction(result);
                            operandStack.push_back(result);
                            rpn.erase(0,1);
                            break;

                case '*' :  x = operandStack.back();        
                            operandStack.pop_back();        
                            y = operandStack.back();
                            operandStack.pop_back();
                            result = multiply(y,x);
                            operandStack.push_back(result); //Then place the result onto the operand stack
                            rpn.erase(0,1);                 //Delete from input the operand
                            break;

            }
        }
    }
    //assign the elements to a set with index
    sets[index] = result;
}



// /*  This function will read the command(SET)
//     from the user's input. If it's SET command, get the right expression
//     assign to string array. Then get the first letter to determine the index of a set.
//     If there is not "SET" command or "equal" sign, print an error message as an invalid input
//     @index : index of a set in the sets array sets[26]
//     @equalPos: return the position of "=" sign if there is "SET" command
// */

// bool setCommand(string &input, int sets[],bool& isEmpty,bool& checkSaved)
// {
//     //remove spaces trailing leading space
//     removeSpace(input);

//     unsigned int posSet = input.find("SET");
//     unsigned int posEqual = input.find("=");
//     unsigned int openBracket = input.find("{");
//     unsigned int closeBracket = input.find("}");
//     unsigned int cAlternative;                       //check if aternative entry appear
//     int index = -1; //index of a set
//     string output = ""; //result after process RPN expression
//     string strSet = "";
//     string newStrSet;
//     string expression = input.substr(posEqual+1);
//     removeSpace(expression);

//     //  get a letter of a SET command
//     if(posSet < input.size() && posEqual < input.size())
//     {

//         // get a string between SET command and "=" sign
//         strSet = input.substr(posSet + 3, posEqual + 1);
//         newStrSet = strSet.substr(posSet, posEqual);

//         if(newStrSet[1] != '=')
//         {
//             return false;
//         }

//         removeSpace(strSet); //remove leading spaces

//         //find letter index
//         if((int)strSet[0] >= 65 && (int)strSet[0] <= 90)
//         {
//             index = (int(strSet[0]) - 65); //get an index of a set
//             /*if(index<=0 || (index >= 26 && index <= 70) || index >=97)
//             {
//                 cout << "The name of a set is invalid;" <<endl;
//                 return false;
//             }*/
//         }
//         else
//             return false;
//     }
//     else
//         return false;   //invalid command

//     //Aternative entry of sets, NOT include "{"
//     if ((openBracket > input.size() || closeBracket > input.size()))
//     {
//         unsigned int check1 = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ+*\\");
//         unsigned int check2 = expression.find_first_of("0123456789");
//         unsigned int setNum;
//         if(check1 < expression.size() && check2 < expression.size())
//             return false;       //invalid command, numbers and letters can be mixed

//         //if entry is an alternative of sets
//         //only numbers are allowed
//         if(check1 > expression.size() && check2 < expression.size())
//         {
//             try
//             {
//                 setNum = stoi(expression);
//             }
//             catch (const std::exception &e)
//             {
//                 cout<< "The number is out of range. Only accept up to 16-bit." << '\n';
//             }
//             // if (setNum > pow(2, 16))
//             //     return false; //not beyond 16-bit
//             // else
//             sets[index] = setNum;
//             isEmpty = false; //at least 1 set existed
//             checkSaved = false;//need to save before exiting
//             return true;     //valid alternative entry of a set
//         }
//     }

//     input = input.substr(posEqual+1); // get the right expression
//     if(!convertToRPN(input,output))
//         return false;
//     process(output,sets,index);
//     isEmpty = false;                //turn on Check Empty set
//     checkSaved = false;//need to save before exiting
//     return true;  //valid command
// }



// /*  Helper will convert the set of numbers to int
//     seperate each number by comma and turn on the bit
//     at index where appears in the set.
// */
// bool setHelper(string& input,unsigned int& setNum)
// {
//     bitset<16> num;
//     int firstBracket = input.find_first_of("{");
//     int lastBracket = input.find_first_of("}");
//     string subSet = input.substr(firstBracket + 1, lastBracket - 1); // get sub set of the numbers

//     while (subSet.size() > 0)
//     {
//         //comma(s) position
//         int pos = subSet.find_first_of(",");
//         // cout << "Pos of comma is: " << pos << endl;
//         //not last number
//         if (pos < subSet.size())
//         {
//             string strNumber = subSet.substr(0, pos);
//             if(stoi(strNumber) < 0 || stoi(strNumber) > 15)
//             {
//                 cout << "The universe set only accepts number in range 0-15."<<endl;
//                 return false;
//             }
//             else
//                 num.flip(stoi(strNumber));
//             //remove substring
//             subSet.erase(0, pos + 1);
//             // cout << "The subset after erase: " << subSet << endl;
//         }
//         //last number
//         else
//         {
//             string strNumber = subSet.substr(); //get last number
//             if(stoi(strNumber) < 0 || stoi(strNumber) > 15)
//             {
//                 cout << "The universe set only accepts number in range 0-15."<<endl;
//                 return false;
//             }
//             else
//                 num.flip(stoi(strNumber));
//             //remove last substring number in the set
//             subSet.erase(0, subSet.size());
//             // cout << "The subset after erase: " << subSet << endl;
//         }
//     }
//     // return num.to_ulong();
//     setNum = num.to_ulong();
//     return true;
// }




// //remove all spaces in input string
// void removeSpace(string& input)
// {
//     int pos = 0;
//     while((pos = input.find(' ')) < input.size())
//     {
//         input.erase(pos,1);
//     }
// }

// //compare user's command input to default command
// //return true if matching
// bool commandMatching(string input,string commandName,unsigned int size)
// {
//     // cout << "Substring is: "<<input.substr(0,size)<<endl;
//     return (input.substr(0,size) == commandName);
// }
// /*  Find a first letter matches to first letter from one of commands in the list(LIST,HELP,SET,LOAD,SAVE)
//     Check the rest of letters. If it matches return true
//     otherwise return false
// */
// bool commandHelper(string input,unsigned int& index)
// {
//     while(input.size()>0)
//     {
//         switch(input[0])
//         {
//             case ' ' :  input.erase(0,1);
//                         break;
//             case 'S' :  if(commandMatching(input,"SET",3))
//                         {
//                             index = 0;          //NEW
//                             return true;
//                         }
//                         else if(commandMatching(input,"SAVE",4))
//                         {
//                             index = 1;          //SAVE
//                             return true;
//                         }
//                         else if(commandMatching(input, "SHOW", 4))
//                         {
//                           index = 3; //LIST
//                           return true;
//                         }
//                         else return false;
//             case 'H' :  if(commandMatching(input,"HELP",4))
//                         {
//                             index = 2;          //HELP
//                             return true;
//                         }
//                         else return false;
//             case 'L' :  if(commandMatching(input,"LOAD",4))
//                         {
//                             index = 4;          //LOAD
//                             return true;
//                         }
//                         else if(commandMatching(input, "LIST", 4))
//                         {
//                             index = 6; //LIST
//                             return true;
//                         }
//             case 'I' :  if(commandMatching(input,"IS",2))
//                         {
//                             index = 5;          //LOAD
//                             return true;
//                         }
//             default:    return false;   //invalid command
//         }
//     }
// }

// /*  Call commandHelper to parse the command
//     If the command doesn't match the listed command keyword
//     return false.
//  */
// bool commandInput(string &input, int sets[],bool& hasSaved,bool& isEmpty)
// {
//     unsigned int index = -1;        //index of command
//     //check valid command
//     if(!commandHelper(input,index))
//         return false;


//     switch (index)
//     {
//         case 0:
//             return setCommand(input,sets,isEmpty,hasSaved);
//             break;
//         case 1:
//             return saveCommand(sets,input,hasSaved,isEmpty);
//             break;
//         case 2:
//             return helpCommand(input);
//             break;
//         case 3:
//             return showCommand(sets,input);
//             break;
//         case 4:
//             return loadCommand(sets,input,isEmpty,hasSaved);
//             break;
//         case 6:
//             return listCommand(sets, input);
//             break;
//         default:
//             return false;   //invalid command
//     }
// }

// bool helpCommand(string input)
// {
//     string fileName = "exam2.help";
//     if(checkFileName(fileName))
//     {
//         ifstream opFile(fileName);
//         unsigned int pos = 0;
//         string token = "";
//         if(opFile.is_open())
//         {
//             while(getline(opFile,token,';'))
//                 cout << token;
//             cout << endl;
//             opFile.close();
//         }
//         return true;
//     }
//     return false;//invalid command
// }


// //check if File Name already exist
// //return true if exists ; otherwise false
// bool checkFileName(string fileName)
// {
//     ifstream file(fileName);
//     return (bool)file;
// }


// //store all expressions if file doesn't exist
// //otherwise rename or remove that file
// bool saveCommand(int sets[],string& input,bool& checkSaved,bool& isEmpty)
// {
//     unsigned int pos = input.find("SAVE");
//     string fileName = input.substr(pos+4);          //get fileName
//     removeSpace(fileName);
//     if(isEmpty)
//     {
//         cout << "The list of expressions is empty." << endl;
//         return true;
//     }
//     else if(fileName == "")
//     {
//         cout << "Missing the file name." <<endl;
//         return true;
//     }
//     return saveHelper(fileName,sets,checkSaved);
//     // return true;
// }



// bool question(string title)
// {
//     string line;
//     cout << title;
//     getline(cin, line);
//     transform(line.begin(), line.end(), line.begin(), ::toupper);
//     return (line == "Y" || line == "YES");
// }


// //Create a new file
// //if file already exists, overwrite it or give another file name
// //return true if checkFileName
// bool saveHelper(string fileName, int sets[],bool& hasSaved)
// {
//     string ans = "";
//     removeSpace(fileName);
//     //add extension to file if missing
//     //if exist lowercase extensions
//     //if fileName exists
//     if(checkFileName(fileName))
//     {
//         cout << "File already exists" << endl;
//         //erase file
//         if(question("Would you like to overwrite file(Y/N): "))
//         {
//             remove(fileName.c_str());                                   //remove the file
//             cout << "Old file is overwritten succesfully." << endl;
//             saveFile(fileName,sets,hasSaved);                                    //write a new file
//             return true;
//         }
//         else if(question("Would you like to rename file(Y/N): "))
//         {
//             cout << "Enter new file name: " << endl;
//             getline(cin, ans);
//             removeSpace(ans);
//             transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
//             //check extension missing
//             while(checkFileName(ans))
//             {
//                 cout << "File already exists. Please enter another name:" << endl;
//                 getline(cin, ans);
//                 transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
//             }
//             rename(fileName.c_str(), ans.c_str());
//             cout << "File successfully renamed." << endl;
//             return true;
//           }
//           else if(question("Would you like to create new file(Y/N): "))
//           {
//               cout << "Enter new file name: " << endl;
//               getline(cin, ans);
//               removeSpace(ans);
//               transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
//               //check extension missing
//               while(checkFileName(ans))
//               {
//                   cout << "File already exists. Please enter another name:" << endl;
//                   getline(cin, ans);
//                   transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
//               }
//               saveFile(ans,sets,hasSaved);
//               cout << "File successfully created." << endl;

//           }
//           else if(question("Would you like to erase the current file(Y/N): "))
//           {
//             remove(fileName.c_str());
//             cout << "File successfully erased." << endl;
//             return true;
//           }
//           return true;
//     }
//     else
//     {
//         saveFile(fileName,sets,hasSaved);
//         return true;
//     }
// }


// //LOAD command
// bool loadCommand(int sets[], string input,bool& isEmpty,bool& hasSaved)
// {
//     unsigned int pos = input.find("LOAD");
//     string fileName = input.substr(pos + 4); //get fileName
//     removeSpace(fileName);
//     if(fileName == "")
//     {
//         cout << "Missing the file name." << endl;
//         return true;//invalid command
//     }
//     loadHelper(sets,fileName,isEmpty,hasSaved);
//     return true;
// }

// //check the file name
// //if  the file exist read through the text file.
// //token it by ";" delimeter and assign each number to an array element of sets[]
// void loadHelper(int sets[],string fileName,bool& isEmpty,bool& hasSaved)
// {
//     string ans = "";
//     removeSpace(fileName);
//     //add extension to file if missing
//     //if exist lowercase extensions

//     //check file exist
//     if(!checkFileName(fileName))
//         cout << "The file name doesn't exist." << endl;
//     else if(fileName.size() == 0)
//         cout << "Missing file name." <<endl;
//     else
//     {
//         string line;
//         ifstream opFile(fileName);
//         unsigned int index = 0;
//         string rpn = "";
//         if (opFile.is_open())
//         {
//             while (getline(opFile, line,';'))
//             {
//                 sets[index++] = stoi(line);
//             }
//             isEmpty = false;                        //Turn off isEmpty
//             hasSaved = true;                        //Saved the current state
//             cout<<"File already is already loaded."<<endl;
//             opFile.close();
//         }
//         else
//             cout << "Unable to open file" << endl;
//     }
// }


// //print a particular set
// bool showCommand(int *sets, string input, map<int, string> uniSet)
// {
//     int pos = 0;                           //get LIST index in the input string
//     removeSpace(input);
//     pos = input.find("SHOW");
//     string command = input.substr(pos,4);   //get "SHOW" string
//     string setIndex = input.substr(pos+4);  //get set number want to see the content
//     removeSpace(setIndex);
//     if(setIndex == "")
//     {
//         cout<<"Missing the name of a set."<<endl;
//         return true;
//     }
//     else
//     {
//         int setName = int(setIndex[0]); //set name in char
//         if(setName<65 || setName>90)
//             cout<<"Index of the set is invalid or out of range (A-Z)."<<endl;
//         else
//         {
//             if(sets[setName-65] != -1)
//             {
//                 cout << char(setName) << " = ";
//                 showHelper(sets[setName-65]);
//             }
//             else
//                 cout<<"The set "<<char(setName)<<" is empty."<<endl;
//         }
//         return true;
//     }
//     return false;    //command valid
// }

// //print list of expressions
// bool listCommand(int *sets, string input, map<int, string> uniSet)
// {
//     int pos = 0; //get LIST index in the input string
//     removeSpace(input);
//     pos = input.find("LIST");
//     string getList = input.substr(pos);
//     //no other parameter after or before LIST
//     if (getList != "LIST" && getList.size() != 4)
//     {
//         cout << "Command LIST is ambiguous." << endl;
//         return true;
//     }
//     else
//     {
//         unsigned int setName = 65; //set name in char
//         for (int i = 0; i < 26; ++i)
//         {
//             if (sets[i] != -1)
//             {
//                 cout << char(setName + i) << " = ";
//                 showHelper(sets[i]);
//             }
//         }
//         return true;
//     }
//     return false; //command valid
// }



// /*
//     Create a file with fileName,
//     Loop through the sets[], write each element into the file, delimited by ";"
// */
// void saveFile(string fileName,int sets[],bool& hasSaved)
// {
//     ofstream myFile;
//     myFile.open(fileName,ios::trunc); //append to the expression to the end
//     for(int i=0;i<26;++i)
//     {
//         int num= sets[i];
//         string temp = to_string(num) + ";" ;
//         myFile << temp ;
//     }
//     hasSaved = true;    //turn SAVED flag on to detect EXIT WITHOUT SAVE
//     myFile.close(); //close the file
// }

// void exitCommand(int sets[],bool hasSaved,bool isEmpty)
// {
//     string ans = "";

//     //ask you to save before exit
//     if(!hasSaved && !isEmpty)
//     {
//         cout << "Would you like to save your changes(Y/N): ";
//         getline(cin,ans);
//         transform(ans.begin(),ans.end(),ans.begin(),::toupper);         //convert to upper case
//         if(ans == "Y" || ans == "YES")
//         {
//             string fileName = "";
//             cout << "Please enter your file name: ";
//             getline(cin,fileName);
//             transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
//             //call saveHelper to process save command
//             saveHelper(fileName,sets,hasSaved);
//         }
//     }
//     else
//         exit(0);    //totalExpression is empty
//     exit(0);//close program
// }


// //remove leading and trailing space before convert RPN
// void leadingTrailing(string &input)
// {
//     int count = 0;
//     while(input.size() > 0 && input[count++] == ' ')
//         input.erase(0,1);
//     while(input.size() > 0 && input[input.size()-1] == ' ')
//         input.erase(input.size()-1);
// }

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
void getNum(string &input, string &operand)
{
    string temp = input;
    while((temp[0] >= '0' && temp[0] <= '9') && temp.size() > 0)
    {
        operand += input[0];
        input.erase(0,1);
    }
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