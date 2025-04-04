#pragma once
#pragma warning(disable: 26495)

#include "DB_helper.h"
#include "task.h"
#include "USR_DAO.h"

#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <string>
#include <iomanip>

class Task_DAO
{
public:
	Task_DAO(DB_helper& server);

	sql::PreparedStatement* getPreparedStmt();
	sql::ResultSet* getResult();

	void insertInToList(Task& task, int usrID);
	void selectList(int usrID);

	void updateTaskName(const std::string& currentTaskName, const std::string& newTaskName, int usrID);
	void updateDescription(const std::string& taskName, const std::string& newDescription, int usrID);
	void updatePriority(const std::string& taskName, Priority priority, int usrID);
	void updateStatus(const std::string& taskName, Status status, int usrID);
	void deletenNote(const std::string& taskName, int usrID);

	void displaySortedResult();
	void sortByPriority(int usrID, int action);
	void sortByStatus(int usrID, int action);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};