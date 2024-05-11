#pragma once
#include <string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#pragma warning(disable: 26495)

class DB_helper
{
public:
	DB_helper(const std::string& server, const std::string& username, const std::string& password);

	std::string getServer() const;
	std::string getUsername() const;
	std::string getPassword() const;

	sql::Driver* getDriver();
	sql::Connection* getCon();

	~DB_helper();
private:
	const std::string server_ip;
	const std::string srv_username;
	const std::string srv_password;

	sql::Driver* driver;
	sql::Connection* con;
};