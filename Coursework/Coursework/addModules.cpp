#include "addModules.h"

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

void clearConsole()
{
#ifdef _WIN32
    std::system("cls");
#endif
}

std::string hashing(const std::string& pwd)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(pwd.c_str()), pwd.length(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}

bool isValidEmail(const std::string& email)
{
    const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    return std::regex_match(email, pattern);
}

std::string hidePass(std::string pass)
{
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '\r')
            break;
        else if (ch == 8)
        {
            if (!pass.empty()) {
                pass.pop_back();
                std::cout << "\b \b";
            }
        }
        else if (std::isalnum(ch) || ch == ' ')
        {
            pass.push_back(ch);
            std::cout << '*';
        }
        else if (ch == 0 || ch == -32)
        {
            ch = _getch();
            switch (ch)
            {
            case 72:
            case 80:
            case 75:
            case 77:
            case 71:
            case 79:
            case 73:
            case 81:
            case 82:
            case 83:
                break;
            default:
                std::cout << ch;
                pass.push_back(ch);
            }
        }
    }
    return pass;
}