#include "Task_DAO.h"
#include "DB_helper.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

Task_DAO::Task_DAO(DB_helper server, sql::PreparedStatement* pstmt_c, sql::ResultSet* result_c) : server_func(server), pstmt(pstmt_c), result(result_c) {}

sql::PreparedStatement* Task_DAO::getPreparedStmt() { return pstmt; }
sql::ResultSet* Task_DAO::getResult() { return result; }

void Task_DAO::insertInToList()
{
	pstmt = server_func.getCon()->prepareStatement("INSERT INTO Task(task_name, task_description, priority, status) VALUES(?,?,?,?)");
	pstmt->setString(1, "banana");
	pstmt->setInt(2, 150);
	pstmt->execute();
	std::cout << "One row inserted." << std::endl;
}

void Task_DAO::selectList()
{
	pstmt = server_func.getCon()->prepareStatement("SELECT * FROM inventory;");
	result = pstmt->executeQuery();

	while (result->next())
		printf("Reading from table=(%d, %s, %d)\n", result->getInt(1), result->getString(2).c_str(), result->getInt(3));
}

void Task_DAO::updateNoteStatus()
{
	pstmt = server_func.getCon()->prepareStatement("UPDATE inventory SET quantity = ? WHERE name = ?");
	pstmt->setInt(1, 200);
	pstmt->setString(2, "banana");
	pstmt->executeQuery();
	printf("Row updated\n");
}

void Task_DAO::deletenNote()
{
	pstmt = server_func.getCon()->prepareStatement("DELETE FROM inventory WHERE name = ?");
	pstmt->setString(1, "orange");
	result = pstmt->executeQuery();
	printf("Row deleted\n");
}