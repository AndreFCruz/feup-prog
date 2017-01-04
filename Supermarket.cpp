#include <iomanip>
#include <algorithm>
#include <set>
#include "Supermarket.h"
#include "Menus.h"
#include "utils.h"

Supermarket* Supermarket::singleton_instance = NULL;

Supermarket::Supermarket() {
	ifstream file_in; string line;
	int numberOfObjects;
	transactionsChanged = clientsChanged = false;

	while (!initialInfo(this->store, this->clientsFile, this->productsFile, this->transactionsFile))
	{
		setcolor(12); cout << "\n Invalid Supermarket Initialization ! Carefully type the information required! \a\n\n"; setcolor(15);
		cout << TAB << "\n Press ENTER to retry..."; cin.ignore(INT_MAX, '\n');
		clearScreen();
	}


	/** Populate Vectors From Files **/
	// Clients from file
	file_in.open(clientsFile);
	file_in >> numberOfObjects;
	clients.reserve(numberOfObjects);
	for (int i = 0; i < numberOfObjects; i++) {
		Client temp_c(file_in);
		clients.push_back(temp_c);
		if (temp_c.get_id() > maxClientId)
			maxClientId = temp_c.get_id();
	}
	file_in.close();

	// Transactions from file
	file_in.open(transactionsFile);
	file_in >> numberOfObjects;
	transactions.reserve(numberOfObjects);
	for (int i = 0; i < numberOfObjects; i++)
		transactions.emplace_back(file_in);
	file_in.close();

	// Sort transactions by chronological order
	sort(transactions.begin(), transactions.end(), [](Transaction& t1, Transaction& t2) {return t1.get_date() < t2.get_date(); });

	// Products from file
	file_in.open(productsFile);
	file_in >> numberOfObjects; file_in.ignore(5, '\n');
	products.reserve(numberOfObjects);
	for (int i = 0; i < numberOfObjects; i++)
		products.emplace_back(file_in);
	file_in.close();
	/// ///

	/** Populate Maps **/
	// Client.name	-->	Client Index
	for (unsigned i = 0; i < clients.size(); i++)
		clientIdx.insert(pair<string, int>(clients[i].get_name(), i));

	// Product.name	-->	Product Index
	for (unsigned i = 0; i < products.size(); i++)
		productIdx.insert(pair<string, int>(products[i].get_name(), i));

	// Client.id	-->	Transaction1 Index, Transaction2 Index, ...
	for (unsigned i = 0; i < transactions.size(); i++)
		transactionIdx.insert(pair<int, int>(transactions[i].get_client_id(), i));

	//clearScreen();
}

void Supermarket::listClientsAlphabetically() const {
	setcolor(14);  showTitle("Client's List"); setcolor(15);
	for (auto pair : clientIdx)
		cout << TAB << " - " << pair.first << endl;
}

bool Supermarket::changeClientName()
{
	string oldName, newName; int index;
	cout << endl << TAB << "Client's current name: "; getline(cin, oldName); trim(oldName);
	if (clientIdx.find(oldName) == clientIdx.end()) {
		cout << TAB << "\n\nClient not found.\n";
		return false;
	}
	else index = clientIdx[oldName];

	cout << TAB << "Client's new desired name: "; getline(cin, newName); trim(newName);
	if (clientIdx.insert(pair<string, int>(newName, index)).second) {
		clients.at(index).set_name(newName);
		clientIdx.erase(oldName);
	}
	else {
		cout << TAB << "\n\nName already in use. Sorry for the incovenience.\n";
		return false;
	}

	return true;
}

void Supermarket::listTransactions() const {
	setcolor(14);  showTitle("All transactions"); setcolor(15);
	for (Transaction t : transactions)
		cout << TAB << t << endl;
	cout << endl;
}
void Supermarket::listTransactions(Date d) const {
	for (Transaction t : transactions)
		if (t.get_date() == d)
			cout << TAB << t << endl;
	cout << endl;
}
void Supermarket::listTransactions(Date d1, Date d2) const {
	for (Transaction t : transactions)
		if (d1 <= t.get_date() && t.get_date() <= d2)
			cout << TAB << t << endl;
	cout << endl;
}
bool Supermarket::listTransactions(string clientName) const {
	int index;
	try {
		index = clientIdx.at(clientName);
	}
	catch (out_of_range& e) {
		setcolor(12); cout << endl << TAB << "Client not found." << endl; setcolor(15);
		return false;
	}
	auto range = transactionIdx.equal_range(clients[index].get_id());
	showTitle(clientName + "'s Transactions");
	for (auto it = range.first; it != range.second; ++it)
		cout << TAB << transactions[it->second] << endl;
	cout << endl;

	return true;
}


void Supermarket::listProducts() const {
	// Alphabetically
	int i = 1;
	showTitle("Product/Price Table");
	setcolor(14); cout << TAB << setw(20) << "Product" << setw(10) << "Price" << endl << endl; setcolor(15);
	for (auto p : productIdx)
		cout << TAB << setw(3) << i++ << " - " << setw(14) << p.first << setw(8) << products[p.second].get_price() << endl;
}

int Supermarket::getProductIdx(string name) const {
	int index;
	try {
		index = productIdx.at(name);
	}
	catch (out_of_range& e) {
		return -1; // Product not found
	}

	return index;
}

void Supermarket::saveChanges() const {
	ofstream file_out;

	if (clientsChanged) {
		file_out.open(clientsFile);
		file_out << clients.size() << endl;
		for (Client c : clients)
			c.save(file_out);
		file_out.close();
	}

	if (transactionsChanged) {
		file_out.open(transactionsFile);
		file_out << transactions.size() << endl;
		for (Transaction t : transactions)
			t.save(file_out);
		file_out.close();
	}
}

void Supermarket::showClientInfo(string name) const {
	int index;
	try {
		index = clientIdx.at(name);
	}
	catch (out_of_range& e) {
		cout << TAB << "Client not found." << endl;
		return;
	}
	setcolor(14);  showTitle(name + "'s Information"); setcolor(15);
	cout << clients.at(index) << endl;
}

bool Supermarket::addClient(string name) {
	auto ret = clientIdx.emplace(name, clients.size());
	if (!ret.second) {
		setcolor(12); cout << "Client name already in use.\a" << endl; setcolor(15);
		return false;
	}
	clients.emplace_back(++maxClientId, name);
	clients.at(clients.size() - 1).set_total(0);
	clientsChanged = true;
	return true;
}

bool Supermarket::eraseClient(string name) {
	int index;
	try {
		index = clientIdx.at(name);
	}
	catch (out_of_range& e) {
		setcolor(12); cout << endl << TAB << "Client not found.\a" << endl; setcolor(15);
		return false;
	}

	clients.erase(clients.begin() + index);
	clientIdx.erase(name);

	// Repopulate map<client_name, client_index> from index onwards
	for (unsigned i = index; i < clients.size(); i++)
		clientIdx.insert(pair<string, int>(clients[i].get_name(), i));


	clientsChanged = true;

	return true;
}

bool Supermarket::addClientTotal(string name, float value) {
	int index;
	try {
		index = clientIdx.at(name);
	}
	catch (out_of_range& e) {
		setcolor(12); cout << "Cliend not found.\a" << endl; setcolor(15);
		return false;
	}

	clients[index].add_total(value);
	clientsChanged = true;

	return true;
}

bool Supermarket::changeClientTotal(string name, float value) {
	int index;
	try {
		index = clientIdx.at(name);
	}
	catch (out_of_range& e) {
		setcolor(12); cout << "Cliend not found.\a" << endl; setcolor(15);
		return false;
	}

	clients[index].set_total(value);
	clientsChanged = true;

	return true;
}

bool Supermarket::addTransaction() {
	int id; string name;
	cout << endl << TAB << "Client's name: "; getline(cin, name);
	try {
		id = clients[clientIdx.at(name)].get_id();
	}
	catch (out_of_range& e) {
		setcolor(12); cout << endl << TAB <<"Cliend not found.\a" << endl; setcolor(15);
		return false;
	}

	Date d; //data da transação é o dia atual

	string product;
	vector<string> transProducts;
	vector<int> vec_prodIdx;	// para calcular o total da transação sem ser necessário voltar a iterar pelos produtos
	while (true) //preenche um vector de produtos
	{
		bool valid = true;
		setcolor(14);  cout << TAB << "Product bought (Press ENTER to end): "; setcolor(15); getline(cin, product); trim(product);
		if (product.empty())
			break;
		else
		{
			try {
				vec_prodIdx.push_back(productIdx.at(product));
			}
			catch (out_of_range& e) {
				setcolor(12);  cout << TAB << "Product not found." << endl; setcolor(15);
				valid = false;
			}
			if (valid) transProducts.push_back(product);
		}
	}

	if (transProducts.empty())
	{
		setcolor(12); cout << "Invalid transaction, no products bought!\a\n"; setcolor(15);
		return false;
	}

	for (unsigned i = 0; i < vec_prodIdx.size(); i++)
		addClientTotal(name, products.at(vec_prodIdx.at(i)).get_price());

	transactions.emplace_back(id, d, transProducts);

	transactionIdx.insert(pair<int, int>(id, transactions.size() - 1));

	transactionsChanged = true;
	clientsChanged = true;
	return true;
}

// Rates a given client's purchase parity to the target client, and records the frequency of the products not bought by the target client.
unsigned compareVecs(const vector<bool> &client, const vector<bool> &targetClient, vector<unsigned> &diffProdFrequency) {
	unsigned int count = 0; bool diff = false;
	for (unsigned i = 0; i < client.size(); i++) {
		if (client[i] && targetClient[i])
			count += 1;
		else if (client[i] && !targetClient[i]) {
			diff = true;
			diffProdFrequency[i] += 1;
		}
	}

	if (diff)
		return count;
	else
		return 0;

	//return diff * count;
}

string Supermarket::getRecommendation(int clientIdx) {
	unsigned NUM_CLIENTS_USED = min(3, clients.size());	// Number of top clients used for recommendation

	vector < vector<bool> > prodMatrix(clients.size(), vector<bool>(products.size(), false));

	// Populate matrix[clientIndex][productIndex]
	for (unsigned i = 0; i < clients.size(); i++) {
		auto range = transactionIdx.equal_range(clients[i].get_id());
		for (auto it = range.first; it != range.second; it++)	// it := *pair<client_id, transaction_index>
			for (string prod : transactions[it->second].get_products())
				prodMatrix[i][productIdx[prod]] = true;
	}

	unsigned recommendedIndex = 0;
	vector< pair<unsigned, unsigned> > numSimilar;	// Rating for similarity of products between clients. vector< pair< clientIndex, clientRating > >
	vector<unsigned> diffProdFreq(products.size(), 0);	// Frequency of the products not bought by the target client

	for (unsigned i = 0; i < prodMatrix.size(); i++) {
		numSimilar.push_back(pair<unsigned, unsigned>(i, compareVecs(prodMatrix[i], prodMatrix[clientIdx], diffProdFreq)));
	}

	// Sort by decreasing order of rating
	sort(numSimilar.begin(), numSimilar.end(), [](pair<unsigned, unsigned> &p1, pair<unsigned, unsigned> &p2) {return p1.second > p2.second; });

	// Check if there's at least one client suitable for recommendation.
	// If no client meets the requirements, suggest the most frequent product not bought by the target client.
	if (numSimilar[0].second == 0) {
		for (unsigned i = 0; i < diffProdFreq.size(); i++)
			if (diffProdFreq[i] > diffProdFreq[recommendedIndex])
				recommendedIndex = i;
		return products[recommendedIndex].get_name();
	}

	// Use the NUM_CLIENTS_USED best rated clients
	numSimilar.resize(NUM_CLIENTS_USED);
	vector<unsigned> topClientsProdFreq(products.size(), 0);
	for (unsigned i = 0; i < numSimilar.size(); i++) {
		for (unsigned j = 0; j < products.size(); j++)
			if (prodMatrix[i][j]) 
				topClientsProdFreq[j] += 1;
	}
	// Find most frequent product
	for (unsigned i = 0; i < topClientsProdFreq.size(); i++) {
		if (topClientsProdFreq[recommendedIndex] < topClientsProdFreq[i] && !prodMatrix[clientIdx][i])
			recommendedIndex = i;
	}

	return products[recommendedIndex].get_name();
}

bool Supermarket::clientRecommendation() {
	int index; string name;
	cout << endl << TAB << "Client's name: "; getline(cin, name);
	try {
		index = clientIdx.at(name);
	}
	catch (out_of_range& e) {
		setcolor(12); cout << endl << TAB << "Cliend not found.\a" << endl; setcolor(15);
		return false;
	}

	cout << endl << TAB << name << ", have you considered trying out our "; setcolor(14);
	cout << getRecommendation(index); setcolor(15); cout << "? It's fantastic!" << endl;

	return true;
}

string Supermarket::bottom10Recommendation() {
	vector<unsigned> bottomClients;
	vector < vector<bool> > prodMatrix(clients.size(), vector<bool>(products.size(), false));

	// Populate matrix[clientIndex][productIndex]
	for (unsigned i = 0; i < clients.size(); i++) {
		bottomClients.push_back(i);
		auto range = transactionIdx.equal_range(clients[i].get_id());
		for (auto it = range.first; it != range.second; it++)	// it := *pair<client_id, transaction_index>
			for (string prod : transactions[it->second].get_products())
				prodMatrix[i][productIdx[prod]] = true;
	}

	// Sort clients's indexes in order of total spent
	sort(bottomClients.begin(), bottomClients.end(), [&](unsigned &idx1, unsigned &idx2) {return clients[idx1].get_total() < clients[idx2].get_total(); });
	
	bottomClients.resize(10);
	
	vector<unsigned> bottom10Prods;	// Products purchased by all bottom 10 clients 
	for (unsigned j = 0; j < products.size(); j++) {
		bool commonProd = true;
		for (unsigned i : bottomClients) {
			if (prodMatrix[i][j] && commonProd)
				commonProd = true;
			else {
				commonProd = false;
				break;
			}
		}
		if (commonProd)
			bottom10Prods.push_back(j);
	}

	
	vector<unsigned> interestingClients;	// Clients that bought all the bottom10 (common) products + 1
	set<unsigned> interestingProducts;		// Products bought by interesting clients, and not bought by all bottom 10 clients.
	map<unsigned, unsigned> prodFrequency;	// product index : product frequency

	for (unsigned i = 0; i < prodMatrix.size(); i++) {		// i -> client index
		bool diff = false;			// client bought something different ?
		bool interesting = true;	// client is usefull for recommendation ?
		for (unsigned j = 0; j < products.size(); j++) {	// j -> product index
			if (find(bottom10Prods.begin(), bottom10Prods.end(), j) != bottom10Prods.end()) {	// product is common to all bottom 10 clients ?
				if (!prodMatrix[i][j]) {
					interesting = false;
					break;
				}
			}
			else if (prodMatrix[i][j]) {
				diff = true;
				prodFrequency[j]++;
			}
		}

		if (diff && interesting) {
			interestingClients.push_back(i);
			// Gather products bought by interesting client and not bought by any bottom 10 client
			for (unsigned j = 0; j < products.size(); j++) {
				if (prodMatrix[i][j])
					interestingProducts.insert(j);
			}
		}
	}

	for (auto p : prodFrequency) {
		// Check if product was bought by any bottom10 client
		for (unsigned idx : bottomClients) {
			if (prodMatrix[idx][p.first])	// Product was bought by a bottom 10 client
				prodFrequency[p.first]--;	// Reduce frequency. Frequency acts as rating for recommendation
		}
	}

	unsigned recommendedProdIdx = 0;
	// Check frequency of interesting products and suggest the most common.
	for (unsigned prodIdx : interestingProducts) {
		if (prodFrequency[prodIdx] > prodFrequency[recommendedProdIdx])
			recommendedProdIdx = prodIdx;
	}

	return products[recommendedProdIdx].get_name();
}

Supermarket* Supermarket::instance() {
	if (!singleton_instance)
		singleton_instance = new Supermarket;

	return singleton_instance;
}

// Imprimir estatisticas da loja
ostream& operator<<(ostream& out, const Supermarket & supermarket)
{
	clearScreen();

	showTitle(Supermarket::instance()->store + "'s Statistical Information:");

	// numero de clientes
	setcolor(14); out << TAB << "Number of clients: "; setcolor(15); out << Supermarket::instance()->clients.size() << endl << endl;

	// numero de transacoes
	setcolor(14); out << TAB << "Number of transactions: "; setcolor(15); out << Supermarket::instance()->transactionIdx.size() << endl << endl;

	// numero de produtos
	setcolor(14); out << TAB << "Number of products available: "; setcolor(15); out << Supermarket::instance()->products.size() << endl << endl;

	// numero medio de transacoes por cliente
	setcolor(14); out << TAB << "Average number of transactions per client: "; setcolor(15);
	out << setprecision(3) << static_cast<float> (Supermarket::instance()->transactionIdx.size()) / Supermarket::instance()->clients.size() << endl << endl;

	// numero medio de dinheiro gasto por cliente
	double totalIncome = 0;
	for (unsigned i = 0; i < Supermarket::instance()->clients.size(); i++)
	{
		totalIncome = totalIncome + Supermarket::instance()->clients.at(i).get_total();
	}

	setcolor(14); out << TAB << "Average money spent per client: "; setcolor(15);
	out << setprecision(3) << totalIncome / Supermarket::instance()->clients.size() << endl << endl;


	// numero medio de produtos por transacao
	setcolor(14);  out << TAB << "Average number of products per transaction: "; setcolor(15);
	int totalProds = 0;
	for (Transaction t : Supermarket::instance()->transactions)
		totalProds += t.get_products().size();
	cout << static_cast<double> (totalProds) / Supermarket::instance()->transactions.size() << endl << endl;


	out << endl << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');


	return out;
}