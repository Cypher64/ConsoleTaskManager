#include "DB_helper.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <string>
#include <iostream>
#pragma warning(disable: 26495)

DB_helper::DB_helper(const std::string& server, const std::string& username, const std::string& password) : server_ip(server), srv_username(username), srv_password(password)
{
	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}

	con->setSchema("to_do_db");
}

sql::Driver* DB_helper::getDriver() { return driver; }
sql::Connection* DB_helper::getCon() { return con; }

DB_helper::~DB_helper() = default;