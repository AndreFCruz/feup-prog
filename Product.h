#pragma once


#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Product {
private:
	string name;
	float price;


public:
	Product(ifstream & in);
	string get_name() const;
	float get_price() const;
	friend ostream& operator<<(ostream& out, const Product & prod); // ostra umproduto
	friend bool operator<(const Product &prod1, const Product &prod2); // compara 2 produtos (alfabeticamente)
};


