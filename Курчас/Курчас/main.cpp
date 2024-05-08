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

    clearConsole();
    link:
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
            clearConsole();
            std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            if (username == "back") { clearConsole(); break; }

            if (username.length() > MAX_NAME_LENGTH) 
            {
                std::cerr << "Error: Maximum allowed length for username exceeded." << std::endl;
                continue;
            }
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
            if (email == "back") { clearConsole(); break; }
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
            if (password == "back") { clearConsole(); break; }

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
            if (dao.LoginUSR(email, password))
            {
                int ID = dao.getUserID(email);  
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
                            std::cout << std::endl;
                            dao.showUSR(ID);
                            std::cout << std::endl;
                            std::cout << "1. Change Name" << std::endl
                                << "2. Change email" << std::endl
                                << "3. Change password" << std::endl
                                << "4. Delete user" << std::endl
                                << "5. Back" << std::endl;

                            std::cin >> action;

                            std::string currentPassword;
                            std::string newValue;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            switch (action)
                            {
                            case '1':
                                clearConsole();
                                std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                std::cout << "Enter new name: ";
                                std::getline(std::cin, newValue);
                                if (newValue == "back") { clearConsole(); break; }
                                clearConsole();
                                dao.updateName(newValue, ID);
                                break;
                            case '2':
                                clearConsole();
                                std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                std::cout << "Enter current password for change email: ";
                                currentPassword = hidePass(currentPassword);
                                if (currentPassword == "back") { clearConsole(); break; }

                                std::cout << std::endl;

                                if (dao.checkPassword(ID, currentPassword))
                                {
                                    std::cout << "Enter new email: ";
                                    std::getline(std::cin, newValue);
                                    if (newValue == "back") { clearConsole(); break; }
                                    if (!isValidEmail(newValue))
                                    {
                                        clearConsole();
                                        std::cout << "Enter valid email\n";
                                        continue;
                                    }
                                    clearConsole();
                                    dao.updateEmail(newValue, ID);
                                }
                                else if (!dao.checkPassword(ID, currentPassword)) { clearConsole(); std::cerr << "Incorrect password." << std::endl; }
                                break;
                            case '3':
                                clearConsole();
                                std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                std::cout << "Enter current password for update: ";
                                currentPassword = hidePass(currentPassword);
                                if (currentPassword == "back") { clearConsole(); break; }

                                std::cout << std::endl;

                                if (dao.checkPassword(ID, currentPassword))
                                {
                                    std::cout << "Enter new password: ";
                                    newValue = hidePass(newValue);
                                    if (newValue == "back") { clearConsole(); break; }
                                    clearConsole();
                                    dao.updatePassword(newValue, ID);
                                }
                                else if (!dao.checkPassword(ID, currentPassword)) { clearConsole(); std::cerr << "Incorrect password." << std::endl; }
                                break;
                            case '4':
                                clearConsole();
                                std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                std::cout << "Enter current password for delete your account: ";
                                currentPassword = hidePass(currentPassword);
                                if (currentPassword == "back") { clearConsole(); break; }

                                std::cout << std::endl;

                                if (dao.checkPassword(ID, currentPassword))
                                {
                                    if (dao.deleteUSR(currentPassword, ID)) 
                                    { 
                                        clearConsole();
                                        std::cout << "User deleted successfully." << std::endl;
                                        goto link; 
                                    }
                                    else { clearConsole(); std::cerr << "Account deletion cancelled." << std::endl; }
                                }
                                else if (!dao.checkPassword(ID, currentPassword)) { clearConsole(); std::cerr << "Incorrect password." << std::endl; }
                                break;
                            case '5':
                                clearConsole();
                                usrMenu = false;
                                break;
                            default:
                                std::cerr << "Wrong action. Try again." << std::endl;
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
                                << "2. Edit task" << std::endl
                                << "3. Task list" << std::endl
                                << "4. Back" << std::endl;

                            std::cin >> action;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            switch (action)
                            {
                            case '1':
                            {
                                std::string title, description;
                                int priority;
                                clearConsole();
                                std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                std::cout << "Enter title: ";
                                std::getline(std::cin, title);
                                if (title == "back") { clearConsole(); break; }
                                std::cout << "Enter description: ";
                                std::getline(std::cin, description);
                                if (description == "back") { clearConsole(); break; }
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
                                bool taskMenu2 = true;
                                clearConsole();
                                while (taskMenu2)
                                {
                                    std::string title, newTitle, newDescription;
                                    int priority, status;
                                    Priority newPriority;
                                    Status newStatus;
                                    taskDao.selectList(ID);
                                    std::cout << std::endl;
                                    std::cout << "1. Change name" << std::endl
                                        << "2. Edit description" << std::endl
                                        << "3. Update priority " << std::endl
                                        << "4. Update status" << std::endl
                                        << "5. Delete task" << std::endl
                                        << "6. Back" << std::endl;

                                    std::cin >> action;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    switch (action)
                                    {
                                    case '1':
                                        clearConsole();
                                        std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                        std::cout << "Enter title of the task to update: ";
                                        std::getline(std::cin, title);
                                        if (title == "back") { clearConsole(); break; }
                                        std::cout << "Enter new title: ";
                                        std::getline(std::cin, newTitle);
                                        clearConsole();
                                        taskDao.updateTaskName(title, newTitle, ID);
                                        break;
                                    case '2':
                                        clearConsole();
                                        std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                        std::cout << "Enter title of the task to update: ";
                                        std::getline(std::cin, title);
                                        if (title == "back") { clearConsole(); break; }
                                        std::cout << "Enter new description: ";
                                        std::getline(std::cin, newDescription);
                                        clearConsole();
                                        taskDao.updateDescription(title, newDescription, ID);
                                        break;
                                    case '3':
                                        clearConsole();
                                        std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                        std::cout << "Enter title of the task to update: ";
                                        std::getline(std::cin, title);
                                        if (title == "back") { clearConsole(); break; }
                                        std::cout << "Enter new priority (1 - High, 2 - Medium, 3 - Low): ";
                                        std::cin >> priority;
                                        if (priority <= 3)
                                        {
                                            newPriority = static_cast<Priority>(priority - 1);
                                            clearConsole();
                                            taskDao.updatePriority(title, newPriority, ID);
                                        }
                                        else { clearConsole(); std::cerr << "Wrong action" << std::endl; }
                                        break;
                                    case '4':
                                        clearConsole();
                                        std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                        std::cout << "Enter title of the task to update: ";
                                        std::getline(std::cin, title);
                                        if (title == "back") { clearConsole(); break; }
                                        std::cout << "Enter new status (1 - In progress, 2 - Done, 3 - Not started): ";
                                        std::cin >> status;
                                        if (status <= 3)
                                        {
                                            newStatus = static_cast<Status>(status - 1);
                                            clearConsole();
                                            taskDao.updateStatus(title, newStatus, ID);
                                        }
                                        else { clearConsole(); std::cerr << "Wrong action" << std::endl; }
                                        break;
                                    case '5':
                                    {
                                        std::string title;
                                        clearConsole();
                                        std::cout << "if you randomly selected this item, write 'back' to return" << std::endl;
                                        std::cout << "Enter title of the task to delete: ";
                                        std::getline(std::cin, title);
                                        if (title == "back") { clearConsole(); break; }
                                        clearConsole();
                                        taskDao.deletenNote(title, ID);
                                        break;
                                    }
                                    case '6':
                                        clearConsole();
                                        taskMenu2 = false;
                                        break;
                                    default:
                                        std::cerr << "Wrong action. Try again." << std::endl;
                                        break;
                                    }
                                }
                                break;
                            }
                            case '3':
                            {
                                bool taskMenu3 = true;
                                clearConsole();
                                while (taskMenu3)
                                {
                                    taskDao.selectList(ID);
                                    std::cout << std::endl;

                                    std::cout << "1. Sort by priority" << std::endl
                                        << "2. Sort by status" << std::endl
                                        << "3. Back" << std::endl;

                                    std::cin >> action;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                    switch (action)
                                    {
                                    case '1':
                                    {
                                        clearConsole();
                                        bool taskMenu4 = true;
                                        while (taskMenu4)
                                        {
                                            std::cout << std::endl;
                                            std::cout << "1. From done " << std::endl
                                                << "2. From not started" << std::endl
                                                << "3. From in progress" << std::endl
                                                << "4. Back" << std::endl;

                                            std::cin >> action;
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            switch (action)
                                            {
                                            case '1':
                                                clearConsole();
                                                taskDao.sortByStatus(ID, action);
                                                break;
                                            case '2':
                                                clearConsole();
                                                taskDao.sortByStatus(ID, action);
                                                break;
                                            case '3':
                                                clearConsole();
                                                taskDao.sortByStatus(ID, action);
                                                break;
                                            case '4':
                                                clearConsole();
                                                taskMenu4 = false;
                                                break;
                                            default:
                                                std::cerr << "Wrong action. Try again." << std::endl;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    case '2':
                                    {
                                        clearConsole();
                                        bool taskMenu5 = true;
                                        while (taskMenu5)
                                        {
                                            std::cout << std::endl;
                                            std::cout << "1. From high" << std::endl
                                                << "2. From low" << std::endl
                                                << "3. From medium" << std::endl
                                                << "4. Back" << std::endl;

                                            std::cin >> action;
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            switch (action)
                                            {
                                            case '1':
                                                clearConsole();
                                                taskDao.sortByPriority(ID, action);
                                                break;
                                            case '2':
                                                clearConsole();
                                                taskDao.sortByPriority(ID, action);
                                                break;
                                            case '3':
                                                clearConsole();
                                                taskDao.sortByPriority(ID, action);
                                                break;
                                            case '4':
                                                clearConsole();
                                                taskMenu5 = false;
                                                break;
                                            default:
                                                std::cerr << "Wrong action. Try again." << std::endl;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    case '3':
                                    {
                                        clearConsole();
                                        taskMenu3 = false;
                                        break;
                                    }
                                    default:
                                        std::cerr << "Wrong action. Try again." << std::endl;
                                        break;
                                    }
                                }
                                break;
                            }
                            case '4':
                            {
                                clearConsole();
                                taskMenu = false;
                                break;
                            }
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
            else { break; }
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