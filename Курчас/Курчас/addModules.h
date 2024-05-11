#pragma once

#include <iostream>
#include <conio.h>
#include <regex>
#include <string>
#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <cstdlib>
#endif // _WIN32

void clearConsole();
std::string hashing(const std::string& pwd);
bool isValidEmail(const std::string& email);
std::string hidePass(std::string pass);