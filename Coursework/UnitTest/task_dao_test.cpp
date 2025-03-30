#include "pch.h"
#include "../Coursework/task.h"
#include "../Coursework/Task_DAO.h"
#include "../Coursework/DB_helper.h"

TEST(TaskDAOTest, InsertAndSelectTask)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    Task task("Test Task", "Test Description", Priority::High, Status::Not_started);

    dao.insertInToList(task, 1);

    dao.selectList(1);
}

TEST(TaskDAOTest, UpdateTaskName)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.updateTaskName("Old Task Name", "New Task Name", 1);
}

TEST(TaskDAOTest, UpdateDescription)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.updateDescription("Task Name", "New Description", 1);
}

TEST(TaskDAOTest, UpdatePriority)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.updatePriority("Task Name", Priority::Medium, 1);
}

TEST(TaskDAOTest, UpdateStatus)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.updateStatus("Task Name", Status::Done, 1);
}

TEST(TaskDAOTest, DeleteTask)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.deletenNote("Task Name", 1);
}

TEST(TaskDAOTest, SortByPriority)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.sortByPriority(1, 1);
}

TEST(TaskDAOTest, SortByStatus) 
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    Task_DAO dao(server);

    dao.sortByStatus(1, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}