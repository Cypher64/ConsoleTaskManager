#pragma once
#include "DB_helper.h"
#include "task.h"
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

	void insertInToList(Task& task);
	void selectList();
	void updateNoteStatus(const std::string& taskName, Priority priority, Status status);
	void deletenNote(const std::string& taskName);
private:
	DB_helper& server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};