#include <iomanip>
#include "Menus.h"
#include "utils.h"
#include "Date.h"


bool initialInfo(string & store, string & clientsFile, string & productsFile, string & transactionsFile) {
	cout << TAB_BIG; showTitle("Welcome to the best sales management software"); 
	cout << endl << endl << setw(15) << "Store's name: "; getline(cin, store); trim(store);
	cout << "\n Type the name of the files where information is stored (FileName.txt): \n" << endl;
	cout << setw(21) << "Clients' file: "; setcolor(14); cin >> clientsFile;
	setcolor(15); cin.ignore(INT_MAX, '\n');

	cout << setw(21) << "Products' file: "; setcolor(14); cin >> productsFile;
	setcolor(15); cin.ignore(INT_MAX, '\n');

	cout << setw(21) << "Transactions' file: "; setcolor(14); cin >> transactionsFile;
	setcolor(15); cin.ignore(INT_MAX, '\n'); // clear the end-of-line character from the cin stream buffer

	return (validFile(clientsFile) && validFile(productsFile) && validFile(transactionsFile) && !cin.fail());
}

/******************************************
 * Gestao de Clientes
 ******************************************/
unsigned short int clientEditOptions() {
	unsigned short int option;

	clearScreen();
	showTitle("Edit Client Menu");
	cout << TAB << "1 - Change name" << endl;
	cout << TAB << "2 - Add amount spent" << endl;
	cout << TAB << "3 - Change total spent" << endl;
	cout << TAB << "4 - Exit sub-menu" << endl << endl;
	string msg = TAB; msg += "Your option: ";
	option = getUnsignedShortInt(1, 4, msg);

	if (option == 4)
		return false;	// false == 0

	return option;
}

void clientEditMenu() {
	unsigned short int option; string name1, name2; float value;

	while ((option = clientEditOptions())) {
		switch (option) {
		case 1: //Change name
			Supermarket::instance()->changeClientName();
			break;
		case 2: //add amount
			cout << endl << TAB <<"Client's name: "; getline(cin, name1);
			cout << TAB << "Amount spent: "; cin >> value;
			Supermarket::instance()->addClientTotal(name1, value);
			break;
		case 3: //change total
			cout << endl << TAB <<"Client's name: "; getline(cin, name1);
			cout << TAB << "Total spent: "; cin >> value;
			Supermarket::instance()->changeClientTotal(name1, value);
			break;
		}
		cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
	}

	Supermarket::instance()->saveChanges();
}

unsigned short int clientOptions() {
	unsigned short int option;

	clearScreen();
	showTitle("Clients' Menu");
	cout << TAB << "1 - List clients alphabetically" << endl;
	cout << TAB << "2 - Show client's information" << endl;
	cout << TAB << "3 - Edit client" << endl;
	cout << TAB << "4 - Remove client" << endl;
	cout << TAB << "5 - Create client" << endl;
	cout << TAB << "6 - Exit sub-menu" << endl << endl;
	string msg = TAB; msg += "Your option: ";
	option = getUnsignedShortInt(1, 6, msg);
	cout << endl;

	if (option == 6)
		return false;	// false == 0

	return option;
}

void clientMenu() {
	unsigned int option; string name;

	while ((option = clientOptions()))
		switch (option) {
		case 1: // List clients
			Supermarket::instance()->listClientsAlphabetically();
			cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
			break;
		case 2: // Show client info
			cout << TAB << "Client's name: ";
			getline(cin, name); cout << endl;
			Supermarket::instance()->showClientInfo(name);
			cout << endl <<TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
			break;
		case 3: // Edit client
			clientEditMenu();
			break;
		case 4: // Remove client
			cout << TAB << "Client's name: ";
			getline(cin, name);
			Supermarket::instance()->eraseClient(name);
			cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
			break;
		case 5:
			cout << TAB << "Client's name: ";
			getline(cin, name);
			Supermarket::instance()->addClient(name);
			cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
			break;
		}

	Supermarket::instance()->saveChanges();
}


/******************************************
 * Gestao de Transacoes
 ******************************************/
unsigned short int transactionOptions() {
	unsigned short int option;

	clearScreen();
	showTitle("Transactions Menu");
	cout << TAB << "1 - List ALL transactions" << endl;
	cout << TAB << "2 - List client's transactions" << endl;
	cout << TAB << "3 - List transactions between 2 days" << endl;
	cout << TAB << "4 - List daily transactions " << endl;
	cout << TAB << "5 - Add transaction" << endl;
	cout << TAB << "6 - Exit sub-menu" << endl << endl;
	string msg = TAB; msg += "Your option: ";
	option = getUnsignedShortInt(1, 6, msg);

	if (option == 6)
		return false;	// false == 0

	return option;
}

void transactionMenu() {
	unsigned short int option; string clientName;

	while ((option = transactionOptions())) {
		switch (option) {
		case 1: //list all transactions
			cout << endl;
			Supermarket::instance()->listTransactions();
			break;
		case 2: //list client transactions
			cout << endl << TAB << "Client name: "; getline(cin, clientName);
			Supermarket::instance()->listTransactions(clientName);
			break;
		case 3: //list transactions between 2 days
			Supermarket::instance()->listTransactions(getDate("First day"), getDate("Last day"));
			break;
		case 4: //list daily transactions
			cout << endl << TAB << "Transaction day\n";
			Supermarket::instance()->listTransactions(getDate());
			break;
		case 5: //add transaction
			Supermarket::instance()->addTransaction();
			break;
		}
	cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
	}

	Supermarket::instance()->saveChanges();
}

/******************************************
 * Recomendacoes
 ******************************************/
unsigned short int recommendationOptions() {
	unsigned short int option;

	clearScreen();
	showTitle("Recommendations Menu");
	cout << TAB << "1 - Personalized recommendation" << endl;
	cout << TAB << "2 - Bottom 10 clients recommendation" << endl;
	cout << TAB << "3 - Exit sub-menu" << endl << endl;
	string msg = TAB; msg += "Your option: ";
	option = getUnsignedShortInt(1, 3, msg);

	if (option == 3)
		return false;	// false == 0

	return option;
}

void recommendationMenu() {
	unsigned int option; string clientName;

	while ((option = recommendationOptions())) {
		switch (option) {
		case 1:
			Supermarket::instance()->clientRecommendation();
			break;
		case 2:
			cout << endl << TAB << "We believe the bottom 10 customers will enjoy our ";
			setcolor(14);
			cout << Supermarket::instance()->bottom10Recommendation();
			setcolor(15); cout << "!\n";
			break;
		}
		cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
	}
}

/******************************************
 * Menu Inicial
 ******************************************/
unsigned short int initialOptions() {
	unsigned short int option;

	clearScreen();
	showTitle("Initial Menu");
	cout << TAB << "1 - Manage clients" << endl;
	cout << TAB << "2 - List available products" << endl;
	cout << TAB << "3 - Manage transactions" << endl;
	cout << TAB << "4 - Recommendations" << endl;
	cout << TAB << "5 - Statistic Information" << endl;
	cout << TAB << "6 - Exit program" << endl << endl;
	string msg = TAB; msg += "Your option: ";
	option = getUnsignedShortInt(1, 6, msg);
	cout << endl << endl;

	if (option == 6) {
		setcolor(14);
		cout << TAB << "Thank you for using our software!\n" << TAB << "Developed by Andre Cruz and Joao Conde\n" << endl;
		setcolor(15);
		return false;
	}
		

	return option;
}

void initialMenu() {
	unsigned int option;


	while ((option = initialOptions()))
		switch (option) {
		case 1: clientMenu();
			break;
		case 2:
			Supermarket::instance()->listProducts();
			cout << endl << TAB << "Press ENTER to continue..."; cin.ignore(INT_MAX, '\n');
			break;
		case 3: transactionMenu();
			break;
		case 4: recommendationMenu();
			break;
		case 5: cout << *Supermarket::instance();
			break;
		}

	Supermarket::instance()->saveChanges();
}
