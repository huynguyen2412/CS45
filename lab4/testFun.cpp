#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <bitset>
#include <string.h>
#include <map>
#include <regex>

using namespace std;

vector<vector<bool>> genTruthTable(unsigned int num);
bitset<26> countLetter(string input);
map<string,unsigned int> mapLetterToColumn(bitset<26> setLetters);
void removeTrailing(string& input);
vector<vector<bool>> cart_product(const vector<vector<bool>> &v);
bool commandMatching(string input,string commandName,unsigned int size);
bool commandHelper(string input,unsigned int& index);

void removeTrailing(string& input)
{
    std::regex_replace(input, std::regex("^\\s+|\\s+$"), "", std::regex_constants::format_default);
}

vector<vector<bool>> cart_product(const vector<vector<bool>> &v)
{
    vector<vector<bool>> s = {{}};
    for (auto& u : v) {
        vector<vector<bool>> r;
        for (auto& x : s) {
            for (auto y : u) {
                r.push_back(x);
                r.back().push_back(y);
            }
        }
        s.swap(r);
    }
    return s;
}

//compare user's command input to default command
//return true if matching
bool commandMatching(string input,string commandName,unsigned int size)
{
    return (input.substr(0,size) == commandName);
}

// "SIZE","NEW","DELETE","LIST","LOAD","IS","QUIT","EDIT","EXIT","WEXIT","WQUIT","HELP","TABLE","STORE"
bool commandHelper(string input,unsigned int& index)
{
    while(input.size()>0)
    {
        switch(input[0])
        {
            case ' ' :  input.erase(0,1);
                        break;
            case 'N' :  if(commandMatching(input,"NEW",3))
                        {
                            index = 0;          //NEW
                            return true;                           
                        }
                        else return false;
            case 'D' :  if(commandMatching(input,"DELETE",6))
                        {
                            index = 1;          //DELETE
                            return true;                           
                        }
                        else return false;
            case 'L' :  if(commandMatching(input,"LIST",4))
                        {
                            index = 2;          //LIST
                            return true;                           
                        }
                        else if(commandMatching(input,"LOAD",4))
                        {
                            index = 3;          //LOAD
                            return true;                           
                        }
                        return false;           //invalid command
            case 'I' :  if(commandMatching(input,"IS",2))
                        {
                            index = 4;          //IS
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
            case 'H' :  if(commandMatching(input,"HELP",4))
                        {
                            index = 8;          //HELP
                            return true;                           
                        }
                        else return false;
            case 'T' :  if(commandMatching(input,"TABLE",5))
                        {
                            index = 9;          //TABLE
                            return true;                           
                        }
                        else return false;
            case 'S' :  if(commandMatching(input,"STORE",5))
                        {
                            index = 10;          //STORE
                            return true;                           
                        }
                        else return false;                                                                                                                                                                                                
            default:    return false;   //invalid command                                            
        }
    }
}


int main () 
{
    
    

    return 0;
}


/*  Created a 2-dimensional dynamic array. Take n-th index of a row and convert to bitset.
    Convert the index number to bitset(number start 0,1,2,3... -> 2^n).
    Map each single bit of a bitset to each array element in horizontal .
    @num: number of letters A-Z
    @numRows: total number of array's rows 2^n
    @binTable: 2d-array 
    @bitNum: binary string used to assign each element of array in a row.
 */
vector<vector<bool>> genTruthTable(unsigned int num)
{
    vector<vector<bool>> binTable = {{}};
    int numRows = pow(2,num);
    binTable.resize(numRows);           //get an actual size of the array
    bitset<27> bitNum = 0;              //initalize the first number
    for(int i=0; i < numRows;++i)
    {
        bitNum = i; //assign a N-th row
        for(int j=num-1; j >=0;j--)
        {
            (bitNum.test(j)) ? (binTable[i].push_back(true)) : (binTable[i].push_back(false)); // push true if bitset is 1, otherwise 0
        }
    }
    //print the truth table
    cout << "TRUTH TABLE Rows: "<< numRows << " Cols: " << num << endl;
    for(int i=0; i < binTable.size(); ++i)
    {
        for (int j = 0; j < num; ++j)
            cout << binTable[i][j] << " ";
        cout << endl;
    }
    return binTable;    
}

bitset<26> countLetter(string input)
{
    bitset<26> letters = 0; // each bit represent the appearance of alphabet letter
    for (int i = 0; i < input.size(); ++i)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            unsigned int index = input[i] - 'A'; //determine the index in bitset
            if (!letters.test(index))
                letters.set(index); //turn on the bit
        }
    }
    cout << "The total letters appear are: " << letters.count() << endl;
    return letters;
}

/*  Create a map<index of letter,index of column>
    To determine which letter correspond to a truthtable's index column
    Each ON bitset will map to index of corresponding column.
    @letterMap: the cotainer store <key,value> with key is index of bitset,
                value is index of the column
    @index: the index of column correspond to the letter in the RPN expression                
    @setLetters: bitset<26> represents the letters appear in RPN expression
 */
map<string, unsigned int> mapLetterToColumn(bitset<26> setLetters)
{
    map<string, unsigned int> letterMap;
    unsigned int index = 0;
    for (int i = 0; i < setLetters.size(); ++i)
    {
        //insert key is an index of ON bitset
        //and value is an index of column
        if (setLetters.test(i))
        {
            string c = string(1,char(65 + i));
            letterMap.insert(pair<string, unsigned int>(c, index++));
        }
    }

    map<string, unsigned int>::iterator it = letterMap.begin();
    for (it = letterMap.begin(); it != letterMap.end(); ++it)
        cout << it->first << " => " << it->second << '\n';
    return letterMap;
}