#include "User.h"
#include <string>

User::User(std::string name, std::string login, std::string password) : usr_name(name), usr_login(login), usr_password(password) {}

std::string User::getName() { return usr_name; }
std::string User::getLogin() { return usr_login; }
std::string User::getPWD() { return usr_password; }