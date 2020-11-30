#include "Customer.h"

DBTool* Customer::tool = DBTool::getInstance();

void Customer::BuildDB() {
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR query_db[1024] = "Create database hotel";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_db, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {
		
	}
	SQLCHAR query_use_db[1024] = "Use hotel";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_use_db, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {

	}
	SQLCHAR query_tb1[1024] = "Create table customers(cutomer_id int not null,name varchar(255), email varchar(255) not null, password varchar(255) not null, PRIMARY KEY ( customer_id ))";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_tb1, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {

	}
	SQLCHAR query_tb2[1024] = "Create table rooms(room_id int not null,room_type varchar(255) not null, PRIMARY KEY ( room_id ))";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_tb2, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {

	}
	SQLCHAR query_tb3[1024] = "Create table slots(slot_id int not null, room_id int not null,f_day int not null, f_month int not null, f_year int not null,l_day int not null, l_month int not null, l_year int not null, PRIMARY KEY ( slot_id ))";
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)query_tb3, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
	}
	else {

	}
	
}

void strToSQLChar(string query, SQLCHAR* SQLQuery) {
	int i = 0;
	for (auto x : query) {
		SQLQuery[i] = x;
		i++;
	}
	SQLQuery[i] = '\0';
}

int sQLLen(SQLCHAR* SQLQuery) {
	int i = 0;
	while (SQLQuery[i]) {
		i++;
	}
	return i;
}

void sQLQueryToStr(SQLCHAR* SQLQuery, string& query) {
	int i = 0;
	while (SQLQuery[i]) {
		query +=SQLQuery[i];
		i++;
	}
}

string Customer::getPassword() {
	string res = "";
	sQLQueryToStr(this->password, res);
	return res;
}

Customer* Customer::getCustomerByEmail(string email) {

	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	Customer* obj = NULL;
	string query = "Select * from customers where email = \"";
	query.append(email);
	query.append("\"");
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		return NULL;
	}
	else {
		obj = new Customer();
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &obj->id, sizeof(obj->id), NULL);
			SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &obj->name, sizeof(obj->name), NULL);
			SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &obj->email, sizeof(obj->email), NULL);
			SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &obj->password, sizeof(obj->password), NULL);
		}
		obj->displayCustomer();
	}
	return obj;
}

Customer* Customer::getCustomerById(int id) {

	
	Customer* obj = NULL;
	string query = "Select * from customers where customer_id = ";
	query.append(to_string(id));
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);
	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		return NULL;
	}
	else {
		obj = new Customer();
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &obj->id, sizeof(obj->id), NULL);
			SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &obj->name, sizeof(obj->name), NULL);
			SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &obj->email, sizeof(obj->email), NULL);
			SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &obj->password, sizeof(obj->password), NULL);
		}
		obj->displayCustomer();
	}
	return obj;
}

bool Customer::login(string email, string password) {
	Customer* res_obj = Customer::getCustomerByEmail(email);
	if (res_obj == NULL) {
		cout << "Login Failed. Email not found" << endl;
		return false;
	}
	if (password.compare(res_obj->getPassword())) {
		cout << "Login Failed. Incorrect Password" << endl;
		return false;
	}
	cout << "Found Customer!" << endl;
	return true;
	delete res_obj;
	res_obj = NULL;
}

void Customer::insertCustomer(int id, string name, string email, string password) {
	Customer* res_obj = Customer::getCustomerByEmail(email);
	if (res_obj) {
		cout << "Email already exists. Please enter a new email address or login instead!" << endl;
		return;
	}
	string query = "Insert into Customers values(";
	query.append(to_string(id));
	query.append(",");
	query.append("'"+name+"'");
	query.append(",");
	query.append("'" + email + "'");
	query.append(",");
	query.append("'" + password + "'");
	query.append(")");
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		cout << "Error Inserting User." << endl;
	}
	else {
		cout << "Inserted Successfully." << endl;
	}
	delete res_obj;
	res_obj = NULL;
}

void Customer::updateCustomerName(int id, string name) {
	Customer* res_obj = Customer::getCustomerById(id);
	if (!res_obj) {
		cout << "Customer Not found!" << endl;
		return;
	}
	string query = "Update customers set name = ";
	query.append("'" + name + "'");
	query.append(" where customer_id = ");
	query.append(to_string(id));
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		cout << "Error Updating Name" << endl;
	}
	else {
		cout << "Updated Name Successfully." << endl;
	}
	delete res_obj;
	res_obj = NULL;
}
void Customer::updateCustomerEmail(int id, string email) {
	Customer* res_obj = Customer::getCustomerById(id);
	if (!res_obj) {
		cout << "Customer Not found!" << endl;
		return;
	}
	string query = "Update customers set email = ";
	query.append("'" + email + "'");
	query.append(" where customer_id = ");
	query.append(to_string(id));
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		cout << "Error Updating Email ID" << endl;
	}
	else {
		cout << "Updated Email ID Successfully." << endl;
	}
	delete res_obj;
	res_obj = NULL;
}
void Customer::updateCustomerPassword(int id, string password) {
	Customer* res_obj = Customer::getCustomerById(id);
	if (!res_obj) {
		cout << "Customer Not found!" << endl;
		return;
	}
	string query = "Update customers set password = ";
	query.append("'" + password + "'");
	query.append(" where customer_id = ");
	query.append(to_string(id));
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		cout << "Error Updating Password" << endl;
	}
	else {
		cout << "Updated Password Successfully." << endl;
	}
	delete res_obj;
	res_obj = NULL;
}

void Customer::deleteCustomer(int id) {
	Customer* res_obj = Customer::getCustomerById(id);
	if (!res_obj) {
		cout << "Customer Not found!" << endl;
		return;
	}
	string query = "Delete from customers where customer_id = ";
	query.append(to_string(id));
	SQLHANDLE SQLStatementHandle = Customer::tool->getStatementHandle();
	SQLCHAR SQLQuery[1024];
	strToSQLChar(query, SQLQuery);

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		tool->showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		cout << "Error Deleting Customer Record." << endl;
	}
	else {
		cout << "Deleted Customer Record Successfully." << endl;
	}
	delete res_obj;
	res_obj = NULL;
}

void Customer::displayCustomer() {
	cout << "Id : " << this->id << endl;
	cout << "Name : " << this->name << endl;
	cout << "Email ID : " << this->email << endl;
	cout << "Password : " << this->password << endl;
}