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
string additionHelper(const string &one, const string &two);
string multiply(const string &one, const string &two);
string multiplyHelper(const string &one, const string &two);
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
    cout<<"-----------------\n";
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
    //-A-B <=> -(A+B)
    if(one[0] == '-')
        return '-' + addition(one.substr(1),two);
    else        //A-B
    {
        string temp = (compare(one,two) == 1 ? 
                            subtractHelper(one,two): ('-' + subtractHelper(two,one)));
        return temp == "-0" ? temp.substr(1) : temp;
    }
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
                    break;
        case '*' : ans = multiply(one,two);           
    }
    display(one, op, two, ans);
}
 

string addition(const string &one, const string &two)
{
    //convert -A+B <=> -(A-B)
    //A>B <=> '-' ||  A<B <=> '+' 
    if(one[0]=='-')                             
    {
        string temp = (compare(one.substr(1),two) == 1 ? 
                        '-' + subtract(one.substr(1),two) : subtract(two,one.substr(1)));
        return temp == "-0" ? temp.substr(1) : temp;
    }
    else
        return additionHelper(one,two);
}

//reverse two numbers
//if sum of two numbers is great 10, carry one and add to next number.
//reverse the string and return
string additionHelper(const string &one, const string &two)
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
        if((carry = digit >= 10))
            digit -= 10;
        ans += toChar(digit);
    }
    trimLeadingZeros(ans);
    return  reverse(ans);
}

//find an char is not belong to a set of specific chars. 
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
    //first number is negative -A/B
    if(line[0]=='-')
    {
        int pos = line.find_first_of("-+*/",1);
        op = line[pos];
        one = trim(line.substr(0,pos));
        two = trim(line.substr(pos+1));
    }
    else
    {
        int pos = line.find_first_of("-+*/");
        op = line[pos];
        one = trim(line.substr(0,pos));
        two = trim(line.substr(pos+1));
    }
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
    unsigned long long i =0;
    if(first.size() != second.size())
        return first.size() > second.size() ? 1 : 2;       //A>B ? 1 : 2
    else
    {
        //stop when one of most significant digits of two numbers is greater than the other.
        while((first[i]==second[i]) && i < first.size()){i++;}
        return toDigit(first[i]) > toDigit(second[i]) ? 1 : 2;     //A>B ? 1 : 2
                                                                    //A=B -> 2
    }
}


string multiply(const string &one, const string &two)
{
    return multiplyHelper(one,two);
}

/*  Repeat n-times the sum of either one or two 
    With n is the number one or two.
    Return the total of sum.
 */
string multiplyHelper(const string &one, const string &two)
{
    int whoLarge = compare(one,two);
    string count = "0";
    string largeNum = "0";
    string result = "";
    whoLarge == 1 ?  largeNum = two : largeNum = one;  //get n
    while(compare(largeNum,count) == 1)
    {
        result = (whoLarge == 1 ? addition(one,result) : addition(two,result));
        count = addition(count,"1");
    }
    cout << "Total of count: "<<count<<endl;
    return result;
}