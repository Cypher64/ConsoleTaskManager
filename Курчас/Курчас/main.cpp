#include <iostream>
#include <conio.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cctype> 
#include <regex>
#include "DB_helper.h"
#include "task.h"
#include "Task_DAO.h"
#include "User.h"
#include "USR_DAO.h"
#ifdef _WIN32
#include <cstdlib>
#endif // _WIN32

static void clearConsole()
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

static void logo()
{
    std::cout <<
        "    ______           __      __  ___                                 \n"
        "   /_  __/___ ______/ /__   /  |/  /___  ____  ____  ____  ___  _____\n"
        "    / / / __ `/ ___/ //_/  / /|_/ / __ `/ __ \\/ __ `/ __ `/ _ \\/ ___/\n"
        "   / / / /_/ (__  ) ,<    / /  / / /_/ / / / / /_/ / /_/ /  __/ /    \n"
        "  /_/  \\__,_/____/_/|_|  /_/  /_/\\__,_/_/ /_/\\__,_/\\__, /\\___/_/     \n"
        "                                                  /____/             \n";
}

char action;

const int MAX_NAME_LENGTH = 40;
const int MAX_EMAIL_LENGTH = 30;


int main()
{
    DB_helper server("127.0.0.1:3306", "Walde", "2005090717_Vol");

    bool running = true;
    while (running)
    {
        logo();
        std::cout << "1. Sign up" << std::endl
            << "2. Log in" << std::endl
            << "3. Exit" << std::endl;

        std::cin >> action;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (action)
        {
        case '1':
        {
            std::string username, email, password;
            std::cout << "Enter username: ";
            std::getline(std::cin, username);

            if (username.length() > MAX_NAME_LENGTH) 
            {
                std::cerr << "Error: Maximum allowed length for username exceeded." << std::endl;
                continue;
            }
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
            if (email.length() > MAX_EMAIL_LENGTH)
            {
                std::cerr << "Error: Maximum allowed length for email exceeded." << std::endl;
                continue;
            }
            if (!isValidEmail(email)) 
            { 
                clearConsole();
                std::cout << "Enter valid email\n"; 
                continue; 
            }

            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            User newUser(username, email, password);
            USR_DAO dao(server);
            clearConsole();
            dao.registerUSR(newUser);
            break;
        }
        case '2':
        {
            std::string email, password;
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
            std::cout << "Enter password: ";
            password = hidePass(password);

            USR_DAO dao(server);
            clearConsole();
            dao.LoginUSR(email, password);
            int ID = dao.getUserID(email);
            if (dao.isLoggedIn())
            {
                Task_DAO taskDao(server);
                bool loggedIn = true;
                while (loggedIn)
                {
                    logo();
                    std::cout << "1. Edit user" << std::endl
                        << "2. Task" << std::endl
                        << "3. Log out" << std::endl;

                    std::cin.clear();
                    std::cin >> action;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    switch (action)
                    {
                    case '1':
                    {
                        clearConsole();
                        bool usrMenu = true;
                        while (usrMenu)
                        {
                            logo();
                            std::cout << "1. Show user" << std::endl
                                << "2. Change Name" << std::endl
                                << "3. Change email" << std::endl
                                << "4. Change password" << std::endl
                                << "5. Delete user" << std::endl
                                << "6. Back" << std::endl;

                            std::cin >> action;

                            std::string currentPassword;
                            std::string newValue;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            switch (action)
                            {
                            case '1':
                                clearConsole();
                                dao.showUSR(ID);
                                break;
                            case '2':
                                clearConsole();
                                std::cout << "Enter new name: ";
                                std::getline(std::cin, newValue);
                                clearConsole();
                                dao.updateName(newValue, ID);
                                break;
                            case '3':
                                clearConsole();
                                std::cout << "Enter current password for change email: ";
                                currentPassword = hidePass(currentPassword);

                                std::cout << std::endl;

                                if (dao.checkPassword(ID, currentPassword))
                                {
                                    std::cout << "Enter new email: ";
                                    std::getline(std::cin, newValue);
                                    if (!isValidEmail(newValue))
                                    {
                                        clearConsole();
                                        std::cout << "Enter valid email\n";
                                        continue;
                                    }
                                    clearConsole();
                                    dao.updateEmail(newValue, ID);
                                }
                                break;
                            case '4':
                                clearConsole();
                                std::cout << "Enter current password for update: ";
                                currentPassword = hidePass(currentPassword);

                                std::cout << std::endl;

                                if (dao.checkPassword(ID, currentPassword))
                                {
                                    std::cout << "Enter new password: ";
                                    newValue = hidePass(newValue);
                                    clearConsole();
                                    dao.updatePassword(newValue, ID);
                                }
                                break;
                            case '5':
                                clearConsole();
                                std::cout << "Enter current password for delete your account: ";
                                currentPassword = hidePass(currentPassword);
                                clearConsole();
                                dao.deleteUSR(currentPassword, ID);
                                break;
                            case '6':
                                clearConsole();
                                usrMenu = false;
                                break;
                            default:
                                std::cerr << "Invalid choice." << std::endl;
                                break;
                            }
                        }
                        break;
                    }
                    case '2':
                    {
                        bool taskMenu = true;
                        clearConsole();
                        while (taskMenu)
                        {
                            logo();
                            std::cout << "1. Create task" << std::endl
                                << "2. Update task" << std::endl
                                << "3. Task list" << std::endl
                                << "4. Delete Task" << std::endl
                                << "5. Back" << std::endl;

                            std::cin.clear();
                            std::cin >> action;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            switch (action)
                            {
                            case '1':
                            {
                                std::string title, description;
                                int priority;
                                clearConsole();
                                std::cout << "Enter title: ";
                                std::getline(std::cin, title);
                                std::cout << "Enter description: ";
                                std::getline(std::cin, description);
                                std::cout << "Enter priority (1 - High, 2 - Medium, 3 - Low): ";
                                std::cin >> priority;
                                Priority taskPriority = static_cast<Priority>(priority - 1);
                                Task newTask(title, description, taskPriority, Status::Not_started);
                                clearConsole();
                                taskDao.insertInToList(newTask, ID);
                                break;
                            }
                            case '2':
                            {
                                std::string title;
                                int priority, status;
                                clearConsole();
                                taskDao.selectList(ID);
                                std::cout << std::endl;
                                std::cout << "Enter title of the task to update: ";
                                std::getline(std::cin, title);
                                std::cout << "Enter new priority (1 - High, 2 - Medium, 3 - Low): ";
                                std::cin >> priority;
                                std::cout << "Enter new status (1 - In progress, 2 - Done, 3 - Not started): ";
                                std::cin >> status;
                                Priority newPriority = static_cast<Priority>(priority - 1);
                                Status newStatus = static_cast<Status>(status - 1);
                                clearConsole();
                                taskDao.updateNoteStatus(title, newPriority, newStatus, ID);
                                break;
                            }
                            case '3':
                                clearConsole();
                                taskDao.selectList(ID);
                                std::cout << std::endl;
                                break;
                            case '4':
                            {
                                std::string title;
                                clearConsole();
                                taskDao.selectList(ID);
                                std::cout << "Enter title of the task to delete: ";
                                std::getline(std::cin, title);
                                clearConsole();
                                taskDao.deletenNote(title, ID);
                                break;
                            }
                            case '5':
                                clearConsole();
                                taskMenu = false;
                                break;
                            default:
                                clearConsole();
                                std::cerr << "Wrong action. Try again." << std::endl;
                                break;
                            }
                        }
                        break;
                    }
                    case '3':
                        clearConsole();
                        loggedIn = false;
                        break;
                    default:
                        clearConsole();
                        std::cerr << "Wrong action. Try again." << std::endl;
                        break;
                    }
                }
            }
            else
            {
                clearConsole();
                std::cerr << "Account not exist, try to sign up." << std::endl;
            }
            break;
        }
        case '3':
        {
            clearConsole();
            std::cout << "ByeBye:)" << std::endl;
            running = false;
            break;
        }
        default:
            clearConsole();
            std::cerr << "Wrong action. Try again." << std::endl;
            break;
        }
    }

    return 0;
}