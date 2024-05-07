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
	void LoginUSR(const std::string& email, const std::string& password);
	void updateName(const std::string& usrName, int ID);
	void updateEmail(const std::string& usrEmail, int ID);
	void updatePassword(const std::string& usrPassword, int ID);
	void deleteUSR(const std::string& usrPassword, int ID);
	bool isLoggedIn();
	int getUserID(const std::string& login);
	bool checkPassword(int ID, const std::string& currentPassword);
	void showUSR(int ID);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};