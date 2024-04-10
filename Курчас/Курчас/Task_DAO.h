#pragma once
#include "DB_helper.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

class Task_DAO
{
public:
	Task_DAO(DB_helper server, sql::PreparedStatement* pstmt_c, sql::ResultSet* result_c);

	sql::PreparedStatement* getPreparedStmt();
	sql::ResultSet* getResult();

	void insertInToList();
	void selectList();
	void updateNoteStatus();
	void deletenNote();

private:
	DB_helper server_func;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};