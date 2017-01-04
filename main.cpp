#include <iostream>
#include <string>
#include <vector>

#include "Supermarket.h"
#include "Menus.h"
#include "Product.h"
#include "Client.h"
#include "utils.h"
#include "defs.h"

int main() {
	cout << *Supermarket::instance();  // mostra estatisticas da loja
	
	initialMenu();	// menu inicial com as grandes opcoes
					// que implementam as funcionalidades
					// disponibilizadas

	return 0;
}
