#include "Task_DAO.h"
#include "DB_helper.h"
#include "task.h"
#include "USR_DAO.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <string>

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
        pstmt->setInt(3, static_cast<int>(task.getPriority()));
        pstmt->setInt(4, static_cast<int>(task.getStatus()));
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

    while (result->next())
        printf("Reading from table=(%s, %s, %d, %d)\n", result->getString("task_name").c_str(), result->getString("task_description").c_str(), result->getInt("priority"), result->getInt("status"));
}

void Task_DAO::updateNoteStatus(const std::string& taskName, Priority priority, Status status, int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("UPDATE task SET priority = ?, status = ? WHERE usr_id = ?, task_name = ?");
    pstmt->setInt(1, static_cast<int>(priority));
    pstmt->setInt(2, static_cast<int>(status));
    pstmt->setString(3, taskName);
    pstmt->setInt(4, usrID);
    pstmt->executeUpdate();
    printf("Row updated\n");
}

void Task_DAO::deletenNote(const std::string& taskName, int usrID)
{
    pstmt = server_func.getCon()->prepareStatement("DELETE FROM task WHERE task_name = ?, usr_id = ?");
    pstmt->setString(1, taskName);
    pstmt->setInt(2, usrID);
    pstmt->executeUpdate();
    printf("Row deleted\n");
}