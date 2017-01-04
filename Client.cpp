#include <iomanip>
#include "Client.h"
#include "utils.h"

Client::Client(unsigned int id, string name) {
	this->id = id;
	this->name = name;
}

Client::Client(ifstream &in) {
	char dummy; string s;
	in >> id >> dummy;		// Extract id from stream and ignore following char

	while (in >> s)	{
		if (s == ";")
			break;
		else
			name = name + s + " ";
	}
	name.pop_back();		// Remove last space added

	in >> clientCard >> dummy >> total;
}

string Client::get_name() const {
	return name;
}

unsigned int Client::get_id() const {
	return id;
}

float Client::get_total() const {
	return total;
}

void Client::add_total(float value) {
	this->total += value;
}

void Client::set_total(float value) {
	this->total = value;
}

void Client::set_name(string new_name)
{
	name = new_name;
}

void Client::save(ofstream &out) const {
	out << id << " ; " << name << " ; " << clientCard << " ; " << total << endl;
}

ostream& operator<<(ostream& out, const Client &cli) {
	out << TAB << "ID: " << cli.id << "  Name: " << cli.name << "  Total spent: " << cli.total;
	return out;
}

bool operator<(const Client &cli1, const Client &cli2) {
	return cli1.name < cli2.name;
}