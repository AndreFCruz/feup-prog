#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>


#include "defs.h"
#include "Date.h"
#include "Client.h"
#include "Transaction.h"
#include "utils.h"
#include "Product.h"

using namespace std;


// Singleton Class
class Supermarket {
	friend ostream& operator<<(ostream& out, const Supermarket & supermarket);

private:
	static Supermarket* singleton_instance;
	Supermarket();	// Unique private constructor to ensure singleton design pattern

	string store; // name da loja
	string clientsFile, productsFile, transactionsFile; // nome dos ficheiros de clientes, produtos e transacoes
	bool transactionsChanged;			// flag que fica a true se for preciso guardar no final as transacoes
	bool clientsChanged;				// flag que fica a true se for preciso guardar no final os clienets
	unsigned int maxClientId = 0;			// variavel que guarda o identificador unico do cliente com o maior identificador
	vector<Client> clients;				// vetor que guarda a informacao dos clientes existentes
	vector<Product> products;			// vetor que guarda a informacao dos produtos disponiveis
	vector<Transaction> transactions;	// vetor que guarda a informacao das transacoes efetuadas
	map<string, int> clientIdx;			// map para "traduzir" nome do cliente no indice dele no vetor de clientes
	map<string, int> productIdx;		// map para "traduzir" nome do produto no indice dele no vetor de produtos
	multimap<int, int> transactionIdx;	// multimap para "traduzir" o identificador do cliente nos indices das suas transacoes no vetor de transacoes

	string getRecommendation(int clientIdx);

public:
	void listClientsAlphabetically() const;
	void listTransactions() const;
	void listTransactions(Date d) const;
	void listTransactions(Date d1, Date d2) const;
	bool listTransactions(string clientName) const;
	void listProducts() const;
	void saveChanges() const;
	void showClientInfo(string name) const;
	bool Supermarket::changeClientName();
	int getProductIdx(string name) const;

	bool addClient(string name);
	bool eraseClient(string name);
	bool addClientTotal(string name, float value);
	bool changeClientTotal(string name, float value);

	bool addTransaction();

	bool clientRecommendation();
	string bottom10Recommendation();

	static Supermarket* instance();
};
