#ifndef PARSING_H
#define PARSING_H

#include <iostream>
#include <string>

using namespace std;

void reduceSign(string &input);
bool checkPrevious(char input);
bool checkForward(char input);
void minusPlusHelper(string &input,unsigned long long &firstPos);
bool getInput(string &line);
void unaryHelper(string &exp);
bool isLetter(char i);
bool isNumber(char i);
// void removeTrailing(string& exp, unsigned long long pos);
void parsing(string &input);
#endif