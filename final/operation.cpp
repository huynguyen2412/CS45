#include <iostream>
#include <string>
#include <string.h> 

using namespace std;
 
int maxSize(int x, int y);
void pad(string &input, int size);
char toChar(int input);
int toDigit(char input);
string reverse(const string &input);
string subtract(const string &one, const string &two);
string subtractHelper(const string &one, const string &two);
string addition(const string &one, const string &two);
string trim(string line);
bool getInput(string &one, char &op, string &two);
bool parseLine(const string &line, string &one, char &op, string &two);
bool illegalChar(const string &line, const string &validChar);
bool breakApart(const string &line, string &one, char &op, string &two);
void perform(const string &one, char op, const string &two);
void display(const string &one, char op,const string &two,const string &ans);
void trimLeadingZeros(string &input);
int compare(string num1, string num2);
void padLeading(string input, int size);



int main()
{
    string one, two;
    char op;
    while(getInput(one, op, two))
        perform(one,op,two);
 
    return 0;
}
 
void display(const string &one, char op,const string &two,const string &ans)
{
    cout<<one<<" "<<op<<" "<< two<<" = "<<ans<<endl;
}
 
 
string reverse(const string &input)
{
    string ans;
    for(int i = input.size()-1; i >= 0; --i)
        ans += input[i];
    return ans;
}
 
char toChar(int input)
{
    return static_cast<char>(48 + input);
}
 
int toDigit(char input)
{
    return input-'0';
}
 
int maxSize(int x, int y)
{
    return x > y ? x : y;
}


//adding trailing zeros
void pad(string &input, int size)
{
    while(input.size() < size)
        input += '0';
}

//adding leading zeros
//only pass a copy
void padLeading(string input, int size)
{
    while(input.size() < size)
        input = '0' + input;
}

void trimLeadingZeros(string &input)
{
    while(input.size() > 1 && input[input.size()-1] == '0')
        input.erase(input.size()-1);
}

/*  Call compare() to determine the first number
    is smaller or greater than the second number.
    If first is smaller than second, subtracting second from first
    andd adding minus(-) sign then.
    Otherwise, do subtracting normally.
    Return result of subtraction of first and second.
 */
string subtract(const string &one, const string &two)
{
    return compare(one,two) == 1 ? subtractHelper(one,two): ('-' + subtractHelper(two,one));
}

 
string subtractHelper(const string &one, const string &two)
{
    string oneR = reverse(one), twoR = reverse(two), ans;
    int padding = maxSize(oneR.size(), twoR.size()) + 1;
    pad(twoR, padding);
    pad(oneR, padding);
    int carry = 0;
    char digit;
    for(int i = 0; i < oneR.size(); ++i)
    {
        digit = toDigit(oneR[i]) - (toDigit(twoR[i]) + carry);
        if((carry = digit < 0))                                     //borrow 1
            digit += 10;
        ans += toChar(digit);
    }
    trimLeadingZeros(ans);
    return  reverse(ans);
}
 
void perform(const string &one, char op, const string &two)
{
    string ans;
    switch(op)
    {
        case '-' : ans = subtract(one, two);
                   break;
        case '+' : ans = addition(one,two);
    }
    display(one, op, two, ans);
}
 
 
string addition(const string &one, const string &two)
{
    string oneR = reverse(one), twoR = reverse(two), ans;
    int padding = maxSize(oneR.size(), twoR.size()) + 1;
    pad(twoR, padding);
    pad(oneR, padding);
    int carry = 0;
    char digit;
    for(int i = 0; i < oneR.size(); ++i)
    {
        digit = toDigit(oneR[i]) + (toDigit(twoR[i]) + carry);
        if((carry = digit > 10))
            digit -= 10;
        ans += toChar(digit);
    }
    trimLeadingZeros(ans);
    return  reverse(ans);
}
 
bool illegalChar(const string &line, const string &validChar)
{
    return line.find_first_not_of(validChar) < line.size();
}
 
bool parseLine(const string &line, string &one, char &op, string &two)
{
    return !line.empty() &&
           !illegalChar(line,"0123456789+-/* ") &&
           breakApart(line, one, op, two);
}


/* Remove leading and trailing spaces */
string trim(string line)
{
    while(line[0] == ' ')
        line.erase(0,1);
    while(line[line.size()-1] == ' ')
        line.erase(line.size()-1);
    return line;
}


/*  Split the numerator and denominator */
bool breakApart(const string &line, string &one, char &op, string &two)
{
    int pos = line.find_first_of("-+*/");
    op = line[pos];
    one = trim(line.substr(0,pos));
    two = trim(line.substr(pos+1));
    return true;
}
 
bool getInput(string &one, char &op, string &two)
{
    string line;
    cout<<"Expression: ";
    getline(cin, line);
    return parseLine(line, one, op, two);
}


/*  Compare two non-negative numbers 
    Return 1 if first number is greater
    otherwise 2 second number
*/
int compare(string first, string second)
{
    int i = 0;
    string result = "";
    int padding = maxSize(first.size(),second.size());
    padLeading(first,padding);
    padLeading(second,padding);
    //stop when one of most significant digits of two numbers is greater than the other.
    while(first[i++]==second[i++]){}                   
    return toDigit(first[i]) > toDigit(second[i]) ? 1 : 2;     
}