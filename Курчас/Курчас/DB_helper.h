#pragma once
#include <string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

class DB_helper
{
public:
	DB_helper(std::string server, std::string username, std::string password);

	std::string getServer() const;
	std::string getUsername() const;
	std::string getPassword() const;

	sql::Driver* getDriver();
	sql::Connection* getCon();

	~DB_helper();
private:
	const std::string server_ip = "127.0.0.1:3306";
	const std::string srv_username = "Walde";
	const std::string srv_password = "2005090717_Vol";

	sql::Driver* driver;
	sql::Connection* con;
};