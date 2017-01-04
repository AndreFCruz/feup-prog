#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Date.h"



using namespace std;


class Transaction {
 private:
  unsigned int client_id;
  Date date;
  vector<string> products;

 public:
  Transaction(ifstream & in); // transacao na forma de  idCliente ; data ; lista de produtos
  Transaction(unsigned int client_id, Date date, vector<string> products);
  unsigned int get_client_id() const;
  const vector<string>& get_products() const;
  Date get_date() const;
  void save(ofstream & out) const; // guarda a transacao
  friend ostream& operator<<(ostream& out, const Transaction & trans); // mostra a transacao
};



