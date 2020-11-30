#include "DBTool.h"

DBTool* DBTool::obj = NULL;
QUERY_MAP DBTool::m_query = {};
DBTool::DBTool() {
	auto res = init_handles();
	if (res < 0) {
		cout << "Init Handles Failed!" << endl;
	}
	res = init_database(connection_str);
	if (res < 0) {
		cout << "Init Database Failed!" << endl;
	}
}

void DBTool::showSQLError(unsigned int handleType, const SQLHANDLE& handle) {
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS != SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL)) {
		cout << "SQL Driver Message : " << message << "\nState : " << SQLState << endl;
	}
}

SQLRETURN DBTool::init_handles() {
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle)) {
		cout << "Error Allocating SQL_HANDLE_ENV" << endl;
		return -1;
	}
	if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
		cout << "Error Setting Env attributes" << endl;
		return -1;
	}
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle)) {
		cout << "Error Allocating SQL_HANDLE_DBC" << endl;
		return -1;
	}
	if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0)) {
		cout << "Error Setting Connect attributes" << endl;
		return -1;
	}
	return 1;
}

SQLRETURN DBTool::init_database(SQLCHAR* conn_str) {

	SQLRETURN retCode = 0;
	int res = 0;
	retCode = init_handles();
	if (retCode < 0) {
		return -1;
	}
	SQLCHAR retConString[1024];
	if (!conn_str) {
		res = SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)xstr(CONNECTION_STRING), SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT);
	}
	else {
		res = SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)conn_str, SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT);
	}
	
	switch (res) {
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_ERROR:
		case SQL_INVALID_HANDLE:
		case SQL_NO_DATA_FOUND:
			cout << "ERROR in SQL DRIVER CONNECT : Code : " << res << endl;
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;

		default:
			break;
	}
	if (retCode == -1) {
		return -1;
	}
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle)) {
		cout << "Error Allocating SQL_HANDLE_STMT" << endl;
		return -1;
	}

	return retCode;
}

SQLHANDLE DBTool::getStatementHandle() {
	return SQLStatementHandle;
}

DBTool* DBTool::getInstance() {
	if (!obj) {
		obj = new DBTool();
	}
	return obj;
}

void DBTool::setConnectionString(string conn_str) {
	int i = 0;
	for (auto x : conn_str) {
		connection_str[i] = x;
		i++;
	}
	connection_str[i] = '\0';
}
DBTool::~DBTool() {
	/* Clear handles if while ends */
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}

//InsertCustomer
//InsertSlot
//InsertRoom
//FetchRoomSchedule
//FetchFreeRoomScheduleWithinDates
//BookRoom
//FetchCustomerInfoByID
//FetchCustomerInfoByEmail
//LoginVerification
//Registeration

//void DBTool::populateQueryMap() {
//	SQLCHAR temp[1024] = "Insert into customers values(?,?,?,?)";
//	m_query.insert(QUERY_PAIR("InsertCustomer", temp));
//	memset(temp, 0, 1024);
//	*temp = "Insert into customers values(?,?,?,?)";
//	m_query.insert(QUERY_PAIR("InsertCustomer", temp));
//}

SQLCHAR* DBTool::GetSQLQuery(string query) {
	auto res = m_query.find(query);
	if (res != m_query.end()) {
		return res->second;
	}
	return NULL;
}
