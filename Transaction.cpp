#include <iomanip>
#include "Transaction.h"
#include "utils.h"

// Constructor from file - idCliente ; data ; lista de produtos
Transaction::Transaction(ifstream & in) {
	
	string prodLine; string tempProd;
	char dummy;

	in >> client_id >> dummy >> date >> dummy;

	getline(in, prodLine);
	int start = prodLine.find_first_not_of(' '), end = prodLine.find(',');
	while (end != string::npos) {
		tempProd = prodLine.substr(start, end - start);
		products.push_back(tempProd);
		start = prodLine.find_first_not_of(' ', end + 1);
		end = prodLine.find(',', start);
	}
	products.push_back(prodLine.substr(start, end));

	// Calcular o total da transacao e adicionar ao cliente ??

}

Transaction::Transaction(unsigned int client_id, Date date, vector<string> products) {
	this->client_id = client_id;
	this->date = date;
	this->products = products;
}


unsigned int Transaction::get_client_id() const {
	return client_id;
}

Date Transaction::get_date() const {
	return date;
}

const vector<string>& Transaction::get_products() const {
	return products;
}

void Transaction::save(ofstream & out) const {
	out << client_id << " ; " << date << " ; ";
	
	for (unsigned int i = 0; i < products.size(); i++)
		out << products[i] << (i == products.size() - 1 ? "\n" : ", ");
}

ostream& operator<<(ostream& out, const Transaction & trans) {
	setcolor(14); out << "Client ID:"; 
	setcolor(15); out << setw(3) << trans.client_id;
	setcolor(14); out << "  Date: ";
	setcolor(15); out << trans.date;
	setcolor(14); out << "  Products: "; setcolor(15);

	// Print products
	for (unsigned int i = 0; i < trans.products.size(); i++)
		cout << trans.products.at(i) << (i < trans.products.size() - 1 ? ", " : ".");

	return out;
}