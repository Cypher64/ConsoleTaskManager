#pragma once
#include "DB_helper.h"
#include "User.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <string>

class USR_DAO
{
public:
	USR_DAO(DB_helper& server);

	sql::PreparedStatement* getPreparedStmt();
	sql::ResultSet* getResult();

	void registerUSR(User& user);
	void LoginUSR(const std::string& login, const std::string& password);
	void update_USR(const std::string& usrName, const std::string& usrLogin, const std::string& usrPassword);
	void deletenUSR(const std::string& usrLogin, const std::string& usrPassword);
	bool isLoggedIn();
	int getUserID(const std::string& login);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};