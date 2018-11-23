/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  Test 2
*/
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <chrono>

using namespace std;
bool getInput(string &line);
bool validateCredit();
bool checkDigit(string creditNumber);
char toChar(int num);
void generatorCC(string& input);

int main(int argc, char const *argv[])
{
    string input;
    int total = 0;
    srand(time(NULL));
    double average=0;
    
    // while(getInput(input))
    // {
    //     if(input.size()>16 && input.find_first_not_of("0123456789") < input.size())
    //         cout <<"Invalid input."<<endl;
    //     else if(checkDigit(input))
    //         cout<<"Credit card is valid."<<endl;
    //     else
    //         cout<<"Credit card is invalid."<<endl;
    // }
    cout<<"STARTING --"<<endl;
    while(total<5)
    {
        clock_t begin = clock();
        // auto start = std::chrono::high_resolution_clock::now();
        generatorCC(input);
        if(checkDigit(input))
        {
            // cout<<input<<"--"<<endl;
            total++;
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            // auto finish = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> elapsed = finish - start;
            average+=elapsed_secs;
        }

        // double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    }
    cout << "Total elapsed time: "<<average <<" secs"<<endl;
    cout<<"ENDING --"<<endl;
    return 0;
}

bool getInput(string &line)
{
    cout<<"Credit card numbers: ";
    getline(cin,line);
    fflush(stdin);                                //Clear input buffer
    return !line.empty();
}

bool checkDigit(string num)
{
    int nonCheck = 0;
    string inverse = "";
    for(int i=0;i<num.size();++i)
        inverse+= num[num.size()-1-i];
    for(int i=0;i<inverse.size();++i)
    {
        int temp = 0; 
        if(i%2==0)
            (temp = ((int)inverse[i]-'0')*2)> 9 ? nonCheck+= temp-9 : nonCheck+= temp;
        else
            nonCheck+= ((int)inverse[i]-'0');
    }
    return ((nonCheck*9)%10)== ((int)num[num.size()-1]-'0');
}

char toChar(int num)
{
    return static_cast<char>(48+num);
}

/*  Create a 16-digit number
    First digit is between 3-6 represent CreditCard provider
 */
void generatorCC(string& input)
{
    input ="";
    for(int i=0;i<16;++i)
        (i==0)? input+=toChar((rand()%4+3)) : input+=toChar((rand()%10));
}
