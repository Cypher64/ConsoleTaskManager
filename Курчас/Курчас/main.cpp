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

	User user1("walde", "walde_wozinskiy@gmail.com", "3287212324");
	USR_DAO DAO2(server);
	DAO2.registerUSR(user1);
	DAO2.LoginUSR("walde_wozinskiy@gmail.com", "3287212324");

	Task task1("learn docker", "watch course for 1 hour", Priority::High, Status::Done);

	Task_DAO DAO1(server);

	DAO1.insertInToList(task1);
	DAO1.updateNoteStatus("learn docker", Priority::High, Status::In_progress);

	return 0;
}