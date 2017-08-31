//Smit shah sss160030@utdallas.edu cs3376
#ifndef _PROG5_H_
#define _PROG5_H_
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
enum e1{D,INPUT,V,L,P,N,W};
extern map<int,string> options;
void SIGINThandler(int);
void SIGHUPhandler(int);
string currentDateTime();
/*string getV(string);
string getL(string);
string getP(string);
string getN(string);
string getW(string);*/
//extern ofstream logfile;
//extern bool EXIT;
//extern int EXITcode;
#endif
