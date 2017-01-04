#pragma once


#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Date {
private:
	int day;
	int month;
	int year;

public:
	Date(); // Default constructor -> current system date
	Date(string data); // date in format DD/MM/YYYY
	Date(unsigned short int day,unsigned short int month,unsigned short int year);
	int get_day() const;
	int get_month() const;
	int get_year() const;

	//void set_day(int day);
	//void set_month(int month);
	//void set_year(int year);

	friend ostream& operator<<(ostream& out, const Date & date);
	friend istream& operator>>(istream& in, Date& date);
	friend bool operator<=(const Date& d1, const Date& d2);
	friend bool operator<(const Date& d1, const Date& d2);
	friend bool operator==(const Date& d1, const Date& d2);
};


