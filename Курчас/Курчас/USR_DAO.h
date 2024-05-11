#pragma once
#pragma warning(disable: 26495)

#include "DB_helper.h"
#include "User.h"
#include "addModules.h"

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
	bool LoginUSR(const std::string& email, const std::string& password);

	void updateName(const std::string& usrName, int ID);
	void updateEmail(const std::string& usrEmail, int ID);
	void updatePassword(const std::string& usrPassword, int ID);
	bool deleteUSR(const std::string& usrPassword, int ID);

	int getUserID(const std::string& login);
	bool checkPassword(int ID, const std::string& currentPassword);
	void showUSR(int ID);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};