#pragma once

#include <iostream>
#include <fstream>
#include <string>


#include "defs.h"
#include "utils.h"
#include "Supermarket.h"

using namespace std;

bool initialInfo(string & store, string & clientsFile, string & productsFile, string & transactionsFile);

void initialMenu();

void clientMenu();

void transactionMenu();

void recommendationMenu();
