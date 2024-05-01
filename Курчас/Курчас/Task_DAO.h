#pragma once
#include "DB_helper.h"
#include "task.h"
#include "USR_DAO.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <string>

class Task_DAO
{
public:
	Task_DAO(DB_helper& server);

	sql::PreparedStatement* getPreparedStmt();
	sql::ResultSet* getResult();

	void insertInToList(Task& task, int usrID);
	void selectList(int usrID);
	void updateNoteStatus(const std::string& taskName, Priority priority, Status status, int usrID);
	void deletenNote(const std::string& taskName, int usrID);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};