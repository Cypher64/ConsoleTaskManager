#pragma once
#include <string>

class User
{
public:
	User(std::string name, std::string login, std::string password);

	std::string getName();
	std::string getLogin();
	std::string getPWD();
private:
	std::string usr_name;
	std::string usr_login;
	std::string usr_password;
};