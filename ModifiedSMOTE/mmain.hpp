#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define space cout << endl << endl


typedef vector<string> vs;
typedef vector<vs> vvs;
typedef vector<double>  vd;
typedef vector<vd> vvd;
typedef vector<vvd> vvvd;


vvd vvsTovvd(vvs, int);
vvs getData(string);
vs splitLines(string,char);
void printDatas(vvs);
void printDatas(vd);
void printDatas(vvd);
void printDatas(vvvd);

void csvExporter(string, vvd);

vvd computeEDs(vvd, int);
double computeED(vd, vd, int);
vvd pairMinorityAndEDs(vvd, vvd);
