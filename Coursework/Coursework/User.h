#pragma once
#include <string>

class User
{
public:
	User(std::string name, std::string email, std::string password);

	std::string getName();
	std::string getEmail();
	std::string getPWD();
private:
	std::string usr_name;
	std::string usr_email;
	std::string usr_password;
};