/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  N-Digit Addition, Subtraction, Multiplication
*/
#include "operation.h"

// int main()
// {
//     string one, two;
//     char op;
//     while(getInput(one, op, two))
//         perform(one, op, two);
//     return 0;
// }
 
void display(const string &one, char op,const string &two,const string &ans)
{
    cout<<one<<" "<<op<<" "<< two<<" = "<<ans<<endl;
    cout<<"-----------------\n";
}
 
//reverse a n-digits string number  
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
                    break;
        case '/':  ans = division(one,two,false);
                    break;
        case '@':  ans = gcd(one,two);
                    break;
        case '!':   if(!factorial(one,ans))
                    {
                        cout << "Factorial can't accept a negative number." << endl;
                        return;
                    }
        case 'P':   ans = permutation(one,two);                                 
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
           !illegalChar(line,"0123456789+-/*@!P ") &&
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
    //Permutation
    if(line[0] == 'P')
    {
        int pos = line.find_first_of(",");
        op = 'P';
        one = line.substr(2,pos);
        two = line.substr(pos+1,line.size()-1);
        cout << "num 1 :"<<one<<endl;
        cout << "num 2 :"<<two<<endl;
    }
    //factorial
    if(line[line.size()-1] == '!')
    {
        int pos = line.find_first_of("!");
        op = line[pos];
        one = trim(line.substr(0,pos));
    }
    //first number is negative -A/B
    if(line[0]=='-')
    {
        int pos = line.find_first_of("-+*/@!",1);
        op = line[pos];
        one = trim(line.substr(0,pos));
        two = trim(line.substr(pos+1));
    }
    else
    {
        int pos = line.find_first_of("-+*/@!");
        op = line[pos];
        one = trim(line.substr(0,pos));
        two = trim(line.substr(pos+1));
    }
    return true;
}
 
// bool getInput(string &one, char &op, string &two)
// {
//     string line;
//     cout<<"Expression: ";
//     getline(cin, line);
//     return parseLine(line, one, op, two);
// }


/*  Compare two non-negative numbers 
    Return 1 if first number is greater
    otherwise 2 if first number less than or equal second number
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
        return toDigit(first[i]) >= toDigit(second[i]) ? 1 : 2;     //A>B ? 1 : 2
                                                                    //A=B -> 2
    }
}


string multiply(const string &one, const string &two)
{
    // -A*-B || A*B
    if ((one[0] == '-' && two[0] == '-') || (one[0] != '-' && two[0] != '-'))
        return (one[0] == '-' && two[0] == '-') ? 
                multiplyHelper(one.substr(1),two.substr(1)) :
                multiplyHelper(one,two);
    else
        return '-' + (one[0]=='-' ? 
                        multiplyHelper(one.substr(1),two) :     //-A*B
                        multiplyHelper(one,two.substr(1)));     //A*-B
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
    while(compare(subtract(largeNum,"1"),count) == 1)   //run to n-1
    {
        result = (whoLarge == 1 ? addition(one,result) : addition(two,result));
        count = addition(count,"1");
    }
    return result;
}


/*  Take a greater number(ONE) subtract a smaller number(TWO) n-times
    Until current number less than a smaller number.
    If it's not divisible operation, return a fraction or improper fraction
    Alternatively, return a integer string.
    @modState: represent modulus state
    @oneClone: remainder 
    ONE and TWO are positive numbers.
 */
string divisionHelper(const string &one, const string &two,bool modState,string &remainder)
{
    string count = "0";                 //numbers of subtraction
    string oneClone = one;
    while(compare(oneClone,two) == 1)   //A >= B : 1    
    {
        oneClone = subtract(oneClone,two);        
        count = addition(count,"1");
    }
    //get remainder
    remainder = oneClone;
    //modulus case
    if(modState)
        return oneClone;
    //Reduce fraction             //convert mixed number to fraction    
    return oneClone == "0" ? count : (addition(multiply(count,two),oneClone)) + '/' + two;  
}

/*  A > B return a positive integer if expression is divisible
          return a reduction of the improper fraction if not divisible                             
    A < B return a reduced fracation
    To get a reduction, need to find GCD , divide denominator and numerator by GCD
    (-)/(-) || (+)/(+) --> (+)
    (+)/(-) || (-)/(+) --> (-) 
    @divisionHelper: only divide a greater number by a smaller number
*/
string division(const string &one, const string &two, bool modState)
{
    string remainder = "0";
    string ans = "";
    int whoLarge = compare(abs(one),abs(two));
    //A >= B
    if(whoLarge == 1)
    {
        ans = divisionHelper(abs(one),abs(two),false,remainder);
        remainder == "0" ? ans : ans = reduceFraction(ans);
    }
    else    //A < B
    {
        ans = reduceFraction(abs(one) + "/" + abs(two));
    }
    return ((one[0] == '-' && two[0] == '-') 
                || (one[0] != '-' && two[0] != '-'))
                ? ans : "-" + ans;
}

//Take absolute of a number
//reuturn a positive string number
string abs(string num)
{
    return (num[0] == '-' ? num.substr(1) : num);
}
//Get Greatest common divisor
//Find GCD of first number and modulus of first and second numbers.
string gcd(string p, string q)
{                               
    string temp ="";            //void this variable            
    return q == "0" ? p : gcd(q, divisionHelper(abs(p),abs(q),true,temp)); //modulus mod ON
}


//Reduce the fraction by finding the GCD
//Divide both num and denom by GCD to get the reduction
void reduceHelper(string &num, string &denom)
{
    string remainder = "";
    string gcdNum = gcd(num,denom);
    num = divisionHelper(num,gcdNum,false,remainder);
    denom = divisionHelper(denom,gcdNum,false,remainder);
}


//call reduceHelper to find GCD and take division of num, denom and GCD num
string reduceFraction(string fraction)
{
    bool numMinus = false, denomMinus = false;
    unsigned long long pos = fraction.find_first_of("/");
    string num = fraction.substr(0,pos), denom = fraction.substr(pos+1);
    num[0] == '-' ? numMinus = true : numMinus;
    denom[0] == '-' ? denomMinus = true : denomMinus;
    numMinus == true ? num = num.erase(0,1) : num;
    denomMinus == true ? denom = denom.erase(0, 1) : denom;
    reduceHelper(num,denom);
    if(denom == "1")
        return (numMinus && denomMinus) 
                || (!numMinus && !denomMinus) ? num : ("-" + num);
    else
        return (numMinus && denomMinus) 
                || (!numMinus && !denomMinus) ? (num + "/" + denom) : ("-" + num + "/" + denom);
    // return denom == "1" ? num : (num + "/" + denom);
}


bool factorial(const string &input,string &ans)
{
    if(input[0] == '-')
        return false;       //factorial can be negative
    ans = factorialHelper(input);
    return true;
}  

/* FACTORIAL */
string factorialHelper(const string &input)
{
    if(input == "0" || input == "1" || input == "2")
        return input == "0" ? "1" : input;
    return multiply(input,factorialHelper(subtract(input,"1")));
}

/* Permutation */
string permutation(const string &n,const string &k)
{    
    if(n[0] == '-' || k[0] == '-')
    {
        cout << "Objects and samples can't be negative"<<endl;
        return "INVALID";
    }
    // n < k
    if(compare(n,k) == 2)
    {
        cout << "N-Objects must be greater or equal K-samples" << endl;
        return "INVALID"; 
    }
    return division(factorialHelper(n), factorialHelper(subtract(n,k)), false);
}

/* Combinations */
string combination(const string &n, const string &k)
{
    if (n[0] == '-' || k[0] == '-')
    {
        cout << "Objects and samples can't be negative" << endl;
        return "INVALID";
    }
    // n < k
    if (compare(n, k) == 2)
    {
        cout << "N-Objects must be greater or equal K-samples" << endl;
        return "INVALID";
    }
    return division(permutation(n,k), factorialHelper(k), false);
}

//detect fraction
//return true if find '/' in fraction
bool checkFrac(string fraction)
{
    return fraction.find_first_of("/") < fraction.size();   
}

/* Adding Fraction */
string addingFrac(string first, string second)
{
    //add 1 to denom if first or second is an integer
    first = !checkFrac(first) ? first + "/1" : first;
    second = !checkFrac(second) ? second + "/1" : second;
    //get numerator and denominator
    unsigned long long op = first.find_first_of("/");
    string numA = first.substr(0,op), denomA = first.substr(op+1);
    string numB = second.substr(0, op), denomB = second.substr(op+1);
    if((numA[0] != '-' && numB[0] != '-')
        && (denomA[0] != '-' && denomB[0] != '-'))
    
}