#pragma once
#include <iostream>
#include <string>
#include "DBTool.h"

using namespace std;



class Customer {
private:
	int id;
	SQLCHAR name[1024];
	SQLCHAR email[1024];
	SQLCHAR password[1024];
public:
	/* Singleton Object to access DB */
	static DBTool* tool;
	/* Customer Utility */
	static void BuildDB();
	string getPassword();
	static Customer* getCustomerById(int id);
	static Customer* getCustomerByEmail(string email);
	void displayCustomer();
	/* Customer Oprerations */
	void insertCustomer(int id, string name, string email, string password);//Register API
	void updateCustomerName(int id, string name);
	void updateCustomerEmail(int id, string email);
	void updateCustomerPassword(int id, string password);
	bool login(string email, string password);
	void deleteCustomer(int id);

	

};