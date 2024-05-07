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
    try
    {
        pstmt = server_func.getCon()->prepareStatement("INSERT INTO task(task_name, task_description, priority, status, usr_id) VALUES(?,?,?,?,?)");
        pstmt->setString(1, task.getName());
        pstmt->setString(2, task.getDescription());

        // Convert priority to string
        std::string priorityStr;
        switch (task.getPriority()) {
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

        // Convert status to string
        std::string statusStr;
        switch (task.getStatus()) {
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
        std::cout << "Could not insert data. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }
}

void Task_DAO::selectList(int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("SELECT * FROM task WHERE usr_id = ?;");
    pstmt->setInt(1, usrID);
    result = pstmt->executeQuery();

    std::cout << std::left << std::setw(20) << "Task Name" << std::setw(40) << "Description" << std::setw(10) << "Priority" << std::setw(15) << "Status" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    while (result->next()) {
        std::string taskName = result->getString("task_name");
        std::string description = result->getString("task_description");
        std::string priority = result->getString("priority");
        std::string status = result->getString("status");

        std::cout << std::left << std::setw(20) << taskName << std::setw(40) << description << std::setw(10) << priority << std::setw(15) << status << std::endl;
    }
}

void Task_DAO::updateNoteStatus(const std::string& taskName, Priority priority, Status status, int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("UPDATE task SET priority = ?, status = ? WHERE task_name = ? AND usr_id = ?");

    // Convert priority to string
    std::string priorityStr;
    switch (priority) {
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
    pstmt->setString(1, priorityStr);

    // Convert status to string
    std::string statusStr;
    switch (status) {
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
    pstmt->setString(2, statusStr);

    pstmt->setString(3, taskName);
    pstmt->setInt(4, usrID);
    pstmt->executeUpdate();
    printf("Row updated\n");
}

void Task_DAO::deletenNote(const std::string& taskName, int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("DELETE FROM task WHERE task_name = ? AND usr_id = ?");
    pstmt->setString(1, taskName);
    pstmt->setInt(2, usrID);
    pstmt->executeUpdate();
    printf("Row deleted\n");
}