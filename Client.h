#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Date.h"

using namespace std;


class Client {
private:
	unsigned int id;
	string name;
	Date clientCard;
	float total;

public:
	Client(unsigned int id, string name);
	Client(ifstream &in);
	string get_name() const;
	unsigned int get_id() const;
	float get_total() const;
	void add_total(float value);
	void set_total(float value);
	void set_name(string name);
	void save(ofstream &out) const;
	friend ostream& operator<<(ostream& out, const Client &cli);	// shows client's information on screen
	friend bool operator<(const Client &cli1, const Client &cli2);	// compares 2 clients (alphabetically)
};