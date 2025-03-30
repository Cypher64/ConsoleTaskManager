#include "pch.h"
#include "../Coursework/User.h"
#include "../Coursework/USR_DAO.h"
#include "../Coursework/DB_helper.h"

TEST(USR_DAOTest, RegisterUser_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    User user("TestUser", "test@example.com", "password");
    dao.registerUSR(user);
}

TEST(USR_DAOTest, LoginUser_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    bool result = dao.LoginUSR("test@example.com", "password");
    ASSERT_TRUE(result);
}

TEST(USR_DAOTest, UpdateUserName_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    dao.updateName("NewName", 123);
}

TEST(USR_DAOTest, UpdateUserEmail_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    dao.updateEmail("new@example.com", 123);
}

TEST(USR_DAOTest, UpdateUserPassword_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    dao.updatePassword("newPassword", 123);
}

TEST(USR_DAOTest, DeleteUser_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    bool result = dao.deleteUSR("password", 123);
    ASSERT_TRUE(result);
}

TEST(USR_DAOTest, GetUserID_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    int userID = dao.getUserID("test@example.com");
    int expectedUserID = 123;
    ASSERT_EQ(userID, expectedUserID);
}

TEST(USR_DAOTest, CheckUserPassword_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    bool result = dao.checkPassword(123, "password");
    ASSERT_TRUE(result);
}

TEST(USR_DAOTest, ShowUserData_Success)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    dao.showUSR(123);
}

TEST(USR_DAOTest, ShowUserData_UserNotFound)
{
    DB_helper server("127.0.0.1:3306", "test_user", "test_password");
    USR_DAO dao(server);

    dao.showUSR(999);
}