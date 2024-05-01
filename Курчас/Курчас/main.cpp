#include <iostream>
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

static void logo()
{
    std::cout << "  ______" << std::endl;
    std::cout << " /\\__  _\\" << std::endl;
    std::cout << " \\/_/\\ \\/" << std::endl;
    std::cout << "    \\ \\_\\" << std::endl;
    std::cout << "     \\/_/ " << std::endl;
}

int main()
{
	DB_helper server("127.0.0.1:3306", "Walde", "2005090717_Vol");

    link1:
    do
    {
        logo();
        std::cout << "1. Sign up" << std::endl
            << "2. Log in" << std::endl
            << "3. Exit" << std::endl;

        int action1;
        std::cin >> action1;
        switch (action1)
        {
        case 1:
        {
            std::string username, email, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter email: ";
            std::cin >> email;
            std::cout << "Enter password: ";
            std::cin >> password;

            User newUser(username, email, password);
            USR_DAO dao(server);
            clearConsole();
            dao.registerUSR(newUser);
            break;
        }
        case 2:
        {
            std::string email, password;
            std::cout << "Enter email: ";
            std::cin >> email;
            std::cout << "Enter password: ";
            std::cin >> password;

            USR_DAO dao(server);
            clearConsole();
            dao.LoginUSR(email, password);
            int ID = dao.getUserID(email);
            if (dao.isLoggedIn()) 
            {

                Task_DAO taskDao(server);
                link2:
                do 
                {
                    logo();
                    std::cout << "1. Edit user" << std::endl
                        << "2. Task" << std::endl
                        << "3. Log out" << std::endl;
                    int action2;
                    std::cin >> action2;
                    switch (action2)
                    {
                    case 1:
                        break;
                    case 2:
                        link3:
                        //clearConsole();
                        logo();
                        std::cout << "1. Create task" << std::endl
                            << "2. Update task" << std::endl
                            << "3. Task list" << std::endl
                            << "4. Delete Task" << std::endl
                            << "5. back" << std::endl;

                        int action4;
                        std::cin >> action4;

                        switch (action4)
                        {
                        case 1:
                        {
                            std::string title, description;
                            int priority;
                            clearConsole();
                            std::cout << "Enter title: ";
                            std::cin >> title;
                            std::cout << "Enter description: ";
                            std::cin >> description;
                            std::cout << "Enter priority (1 - High, 2 - Medium, 3 - Low): ";
                            std::cin >> priority;
                            Priority taskPriority = static_cast<Priority>(priority - 1);
                            Task newTask(title, description, taskPriority, Status::not_started);
                            clearConsole();
                            taskDao.insertInToList(newTask, ID);
                            goto link3;
                            break;
                        }
                        case 2:
                        {
                            std::string title;
                            int priority, status;
                            clearConsole();
                            taskDao.selectList(ID);
                            std::cout << "Enter title of the task to update: ";
                            std::cin >> title;
                            std::cout << "Enter new priority (1 - High, 2 - Medium, 3 - Low): ";
                            std::cin >> priority;
                            std::cout << "Enter new status (1 - In progress, 2 - Done, 3 - Not started): ";
                            std::cin >> status;
                            Priority newPriority = static_cast<Priority>(priority - 1);
                            Status newStatus = static_cast<Status>(status - 1);
                            clearConsole();
                            taskDao.updateNoteStatus(title, newPriority, newStatus, ID);
                            goto link3;
                            break;
                        }
                        case 3:
                        {
                            clearConsole();
                            taskDao.selectList(ID);
                            break;
                        }
                        case 4:
                        {
                            std::string title;
                            clearConsole();
                            std::cout << "Enter title of the task to delete: ";
                            std::cin >> title;
                            clearConsole();
                            taskDao.deletenNote(title, ID);
                            goto link3;
                            break;
                        }
                        case 5:
                        {
                            clearConsole();
                            goto link2;
                        }
                        default:
                        {
                            clearConsole();
                            std::cerr << "Wrong action. Try again." << std::endl;
                            goto link3;
                            break;
                        }
                        }
                        break;
                    case 3:
                        clearConsole();
                        goto link1;
                        break;
                    default:
                        clearConsole();
                        std::cerr << "Wrong action. Try again." << std::endl;
                        break;
                    }

                } while (true);
            }
            else 
            {
                clearConsole();
                std::cerr << "Account not exist, try to sign up." << std::endl;
            }
            break;
        }
        case 3:
        {
            clearConsole();
            std::cout << "ByeBye:)" << std::endl;
            exit(1);
        }
        default:
            clearConsole();
            std::cerr << "Wrong action. Try again." << std::endl;
            break;
        }
    } while (true);

	return 0;
}