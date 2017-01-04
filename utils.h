#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>

#include "defs.h"
#include "Date.h"

using namespace std;


void clearScreen();

unsigned short int getUnsignedShortInt(unsigned short int min, unsigned short int max, string msg = "", size_t width = 0);
void setcolor(unsigned int color);

template <class numT>
numT getValue(string msg = "", size_t width = 0);

bool validFile(const string file);
char toUpper(char c);
void trim(string &value);
void showTitle(string title, ostream& out = cout);
Date getDate(string msg = "");


