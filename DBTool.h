#pragma once
#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <unordered_map>

using namespace std;

/* fill accordingly */
#define xstr(s) str(s)
#define str(s) #s
#define DRIVER DRIVER={MySQL ODBC 8.0 ANSI Driver};
#define SERVER SERVER=hostname,
#define PORT portnumber;
#define DATABASE DATABASE=dbname;
#define UID UID=uname;
#define PWD PWD=pwd;
#define MULTI_HOST MULTI_HOST=1;
#define CONNECTION_STRING DRIVER SERVER PORT DATABASE UID PWD MULTI_HOST

typedef unordered_map<string, SQLCHAR*> QUERY_MAP;
typedef pair<string, SQLCHAR*> QUERY_PAIR;

class DBTool {

private:
	static QUERY_MAP m_query;
	static void populateQueryMap();
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	static DBTool* obj;
	SQLCHAR* connection_str = NULL;

	SQLRETURN init_handles();
	SQLRETURN init_database(SQLCHAR *conn_str = NULL);
	DBTool();

public:
	static SQLCHAR* GetSQLQuery(string query);
	static DBTool* getInstance();
	void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
	SQLHANDLE getStatementHandle();
	void setConnectionString(string conn_str);
	~DBTool();
};