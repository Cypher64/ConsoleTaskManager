#include <iostream>
#include <iostream>
#include "DB_helper.h"
#include "task.h"
#include "Task_DAO.h"
#include "User.h"
#include "USR_DAO.h"

int main()
{
	DB_helper server("127.0.0.1:3306", "Walde", "2005090717_Vol");

    link1:
    do
    {
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
            dao.LoginUSR(email, password);
            if (dao.isLoggedIn()) 
            {

                Task_DAO taskDao(server);
                link2:
                do 
                {
                    std::cout << "1 Edit user" << std::endl
                        << "2. Task" << std::endl
                        << "3. Log out" << std::endl;
                    int action2;
                    std::cin >> action2;
                    switch (action2)
                    {
                    case 1:
                        break;
                    case 2:
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
                            std::cout << "Enter title: ";
                            std::cin >> title;
                            std::cout << "Enter description: ";
                            std::cin >> description;
                            std::cout << "Enter priority (1 - High, 2 - Medium, 3 - Low): ";
                            std::cin >> priority;
                            Priority taskPriority = static_cast<Priority>(priority - 1);
                            Task newTask(title, description, taskPriority, Status::not_started);
                            taskDao.insertInToList(newTask);
                            break;
                        }
                        case 2:
                        {
                            std::string title;
                            int priority, status;
                            std::cout << "Enter title of the task to update: ";
                            std::cin >> title;
                            std::cout << "Enter new priority (1 - High, 2 - Medium, 3 - Low): ";
                            std::cin >> priority;
                            std::cout << "Enter new status (1 - In progress, 2 - Done, 3 - Not started): ";
                            std::cin >> status;
                            Priority newPriority = static_cast<Priority>(priority - 1);
                            Status newStatus = static_cast<Status>(status - 1);
                            taskDao.updateNoteStatus(title, newPriority, newStatus);
                            break;
                        }
                        case 3:
                        {
                            taskDao.selectList();
                            break;
                        }
                        case 4:
                        {
                            std::string title;
                            std::cout << "Enter title of the task to delete: ";
                            std::cin >> title;
                            taskDao.deletenNote(title);
                            break;
                        }
                        case 5:
                        {
                            goto link2;
                        }
                        default:
                        {
                            std::cout << "Wrong action. Try again." << std::endl;
                            break;
                        }
                        }
                        break;
                    case 3:
                        goto link1;
                        break;
                    default:
                        std::cout << "Wrong action. Try again." << std::endl;

                        break;
                    }

                } while (true);
            }
            else 
            {
                goto link1;
            }
            break;
        }
        case 3:
        {
            std::cout << "ByeBye:)" << std::endl;
            exit(1);
        }
        default:
            std::cout << "Wrong action. Try again." << std::endl;
            break;
        }
    } while (true);

	return 0;
}