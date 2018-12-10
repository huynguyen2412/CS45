/*
  CS45 DISCRETE STRUCTURES WITH COMPUTER SCIENCE APPLICATIONS
  Professor PAUL WILKINSON
  Tom Nguyen
  N-Digit Addition, Subtraction, Multiplication
*/
#ifndef OPERATION_H
#define OPERATION_H
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
string divisionHelper(const string &one, const string &two,bool modState,string &remainder);
string division(const string &one, const string &two, bool modState);
bool factorial(const string &input, string &ans);
string factorialHelper(const string &input);
string permutation(const string &n,const string &k);
string combination(const string &n, const string &k);
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
string abs(string num);                      //take absolute
string gcd(string p, string q);
void reduce(string &num, string &denom);
string reduceFraction(string ans);
string modHelper(const string &one, const string &two);
#endif