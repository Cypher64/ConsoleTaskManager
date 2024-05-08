#pragma warning(disable: 26495)

#include "Task_DAO.h"
#include "DB_helper.h"
#include "task.h"
#include "USR_DAO.h"

#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <string>
#include <iomanip>

Task_DAO::Task_DAO(DB_helper& server) : server_func(server) {}

sql::PreparedStatement* Task_DAO::getPreparedStmt() { return pstmt; }
sql::ResultSet* Task_DAO::getResult() { return result; }

void Task_DAO::insertInToList(Task& task, int usrID)
{
    if (task.getName().empty())  
    {
        std::cerr << "all fields except the description must be filled." << std::endl;
        return;
    }

    try
    {
        pstmt = server_func.getCon()->prepareStatement("INSERT INTO task(task_name, task_description, priority, status, usr_id) VALUES(?,?,?,?,?)");
        pstmt->setString(1, task.getName());
        pstmt->setString(2, task.getDescription());

        std::string priorityStr;
        switch (task.getPriority()) 
        {
        case Priority::High:
            priorityStr = "High";
            break;
        case Priority::Medium:
            priorityStr = "Medium";
            break;
        case Priority::Low:
            priorityStr = "Low";
            break;
        }
        pstmt->setString(3, priorityStr);

        std::string statusStr;
        switch (task.getStatus())
        {
        case Status::In_progress:
            statusStr = "In progress";
            break;
        case Status::Done:
            statusStr = "Done";
            break;
        case Status::Not_started:
            statusStr = "Not started";
            break;
        }
        pstmt->setString(4, statusStr);

        pstmt->setInt(5, usrID);
        pstmt->execute();
        std::cout << "One row inserted." << std::endl;
    }
    catch (const sql::SQLException& e)
    {
        std::cerr << "Could not insert data. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }
}

void Task_DAO::selectList(int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ?;");
    pstmt->setInt(1, usrID);
    result = pstmt->executeQuery();

    bool tasksFound = false;
    
    if (!tasksFound)
    {
        std::cout << std::left << std::setw(20) << "Task Name" << std::setw(40) << "Description" << std::setw(10) << "Priority" << std::setw(15) << "Status" << std::endl;
        std::cout << std::string(85, '-') << std::endl;

        while (result->next())
        {
            std::string taskName = result->getString("task_name");
            std::string description = result->getString("task_description");
            std::string priority = result->getString("priority");
            std::string status = result->getString("status");

            std::cout << std::left << std::setw(20) << taskName << std::setw(40) << description << std::setw(10) << priority << std::setw(15) << status << std::endl;
        }
    }
    else if (tasksFound) { std::cout << "You have no tasks yet." << std::endl; }
}

void Task_DAO::updateTaskName(const std::string& currentTaskName, const std::string& newTaskName, int usrID)
{
    if (currentTaskName.empty())
    {
        std::cerr << "Current name can't be empty." << std::endl;
        return;
    }
    else if (newTaskName.empty())
    {
        std::cout << "Keeping the current name." << std::endl;
        return;
    }

    pstmt = server_func.getCon()->prepareStatement("UPDATE task SET task_name = ? WHERE task_name = ? AND usr_id = ?");
    pstmt->setString(1, newTaskName);
    pstmt->setString(2, currentTaskName);
    pstmt->setInt(3, usrID);
    int rowsAffected = pstmt->executeUpdate();
    if (rowsAffected == 0)
    {
        std::cerr << "Task with the specified name not found for the given user." << std::endl;
    }
    else { std::cout << "Task name updated\n" << std::endl; }
}

void Task_DAO::updateDescription(const std::string& taskName, const std::string& newDescription, int usrID)
{
    if (taskName.empty())
    {
        std::cerr << "Task name can`t be empty." << std::endl;
        return;
    }
    else if (newDescription.empty())
    {
        std::cout << "Keeping the current name." << std::endl;
        return;
    }

    try
    {
        pstmt = server_func.getCon()->prepareStatement("UPDATE Task SET task_description = ? WHERE task_name = ? AND usr_id = ?");
        pstmt->setString(1, newDescription);
        pstmt->setString(2, taskName);
        pstmt->setInt(3, usrID);
        int rowsAffected = pstmt->executeUpdate();
        if (rowsAffected == 0)
        {
            std::cerr << "Task with the specified name not found for the given user." << std::endl;
        }
        else { std::cout << "Task description updated\n" << std::endl; }
    }
    catch (sql::SQLException& e) 
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return;
    }
}

void Task_DAO::updatePriority(const std::string& taskName, Priority priority, int usrID)
{
    if (taskName.empty())
    {
        std::cerr << "Task name can`t be empty." << std::endl;
        return;
    }

    std::string priorityStr;
    switch (priority)
    {
    case Priority::High:
        priorityStr = "High";
        break;
    case Priority::Medium:
        priorityStr = "Medium";
        break;
    case Priority::Low:
        priorityStr = "Low";
        break;
    }

    pstmt = server_func.getCon()->prepareStatement("UPDATE task SET priority = ? WHERE task_name = ? AND usr_id = ?");
    pstmt->setString(1, priorityStr);
    pstmt->setString(2, taskName);
    pstmt->setInt(3, usrID);
    int rowsAffected = pstmt->executeUpdate();
    if (rowsAffected == 0)
    {
        std::cerr << "Task with the specified name not found for the given user." << std::endl;
    }
    else { std::cout << "Task priority updated\n" << std::endl; }
}

void Task_DAO::updateStatus(const std::string& taskName, Status status, int usrID)
{
    if (taskName.empty())
    {
        std::cerr << "Task name can`t be empty." << std::endl;
        return;
    }
    
    std::string statusStr;
    switch (status) 
    {
    case Status::In_progress:
        statusStr = "In progress";
        break;
    case Status::Done:
        statusStr = "Done";
        break;
    case Status::Not_started:
        statusStr = "Not started";
        break;
    }

    pstmt = server_func.getCon()->prepareStatement("UPDATE task SET status = ? WHERE task_name = ? AND usr_id = ?");
    pstmt->setString(1, statusStr);
    pstmt->setString(2, taskName);
    pstmt->setInt(3, usrID);
    int rowsAffected = pstmt->executeUpdate();
    if (rowsAffected == 0)
    {
        std::cerr << "Task with the specified name not found for the given user." << std::endl;
    }
    else { std::cout << "Task status updated\n" << std::endl; }
}

void Task_DAO::deletenNote(const std::string& taskName, int usrID)
{
    if (taskName.empty())
    {
        std::cerr << "Task name can`t be empty." << std::endl;
        return;
    }

    pstmt = server_func.getCon()->prepareStatement("DELETE FROM task WHERE task_name = ? AND usr_id = ?");
    pstmt->setString(1, taskName);
    pstmt->setInt(2, usrID);
    int rowsAffected = pstmt->executeUpdate();
    if (rowsAffected == 0)
    {
        std::cerr << "Task with the specified name not found for the given user." << std::endl;
    }
    else { std::cout << "Row deleted\n" << std::endl; }
}

void Task_DAO::displaySortedResult()
{
    bool tasksFound = false;
    if (!tasksFound)
    {
        std::cout << std::left << std::setw(20) << "Task Name" << std::setw(40) << "Description" << std::setw(10) << "Priority" << std::setw(15) << "Status" << std::endl;
        std::cout << std::string(85, '-') << std::endl;

        while (result->next())
        {
            std::string taskName = result->getString("task_name");
            std::string description = result->getString("task_description");
            std::string priority = result->getString("priority");
            std::string status = result->getString("status");

            std::cout << std::left << std::setw(20) << taskName << std::setw(40) << description << std::setw(10) << priority << std::setw(15) << status << std::endl;
        }
    }
    else if (tasksFound)
    {
        std::cout << "You have no tasks yet." << std::endl;
    }
}

void Task_DAO::sortByPriority(int usrID, int action)
{
    switch (action)
    {
    case '1':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(priority, 'High', 'Medium', 'Low');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    case '2':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(priority, 'Low', 'Medium', 'High');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    case '3':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(priority, 'Medium', 'High', 'Low');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    }
}

void Task_DAO::sortByStatus(int usrID, int action)
{
    switch (action)
    {
    case '1':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(status, 'Done', 'In progress', 'Not started');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    case '2':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(status, 'Not started', 'In progress', 'Done');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    case '3':
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ? ORDER BY FIELD(status, 'In progress', 'Not started', 'Done');");
        pstmt->setInt(1, usrID);
        result = pstmt->executeQuery();

        displaySortedResult();
        break;
    }
}