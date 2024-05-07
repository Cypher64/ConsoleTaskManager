#include "User.h"
#include <string>

User::User(std::string name, std::string email, std::string password) : usr_name(name), usr_email(email), usr_password(password) {}

std::string User::getName() { return usr_name; }
std::string User::getEmail() { return usr_email; }
std::string User::getPWD() { return usr_password; }