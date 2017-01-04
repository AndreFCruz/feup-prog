#include "Product.h"
#include "utils.h"

Product::Product(ifstream & in) {
	getline(in, name, ';');
	trim(name);
	in >> price;
	in.ignore(3, '\n');
}

string Product::get_name() const {
	return name;
}

float Product::get_price() const {
	return price;
}

ostream& operator<<(ostream& out, const Product & prod) {
	out << prod.name;
	return out;
}

// compara 2 produtos (alfabeticamente)
bool operator<(const Product &prod1, const Product &prod2) {
	return prod1.name< prod2.name;
}