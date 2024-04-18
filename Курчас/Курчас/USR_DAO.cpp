#include "USR_DAO.h"
#include "DB_helper.h"
#include "User.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <string>

int t = 0;

USR_DAO::USR_DAO(DB_helper& server) : server_func(server) {}

sql::PreparedStatement* USR_DAO::getPreparedStmt() { return pstmt; }
sql::ResultSet* USR_DAO::getResult() { return result; }

void USR_DAO::registerUSR(User& user)
{
    pstmt = server_func.getCon()->prepareStatement("INSERT INTO User(usr_name, usr_login, usr_password) VALUES(?,?,?)");
    pstmt->setString(1, user.getName());
    pstmt->setString(2, user.getLogin());
    pstmt->setString(3, user.getPWD());
    pstmt->execute();
    std::cout << "User registered successfully." << std::endl;
}

void USR_DAO::LoginUSR(const std::string& login, const std::string& password)
{
    pstmt = server_func.getCon()->prepareStatement("SELECT * FROM User WHERE usr_login = ? AND usr_password = ?");
    pstmt->setString(1, login);
    pstmt->setString(2, password);
    result = pstmt->executeQuery();
    if (result->rowsCount() == 1)
    {
        std::cout << "Login successful." << std::endl;
        t++;
    }
    else 
    {
        std::cout << "Login failed. Incorrect login or password." << std::endl;
    }
}


void USR_DAO::update_USR(const std::string& usrName, const std::string& usrLogin, const std::string& usrPassword)
{
    pstmt = server_func.getCon()->prepareStatement("UPDATE User SET usr_name = ?, usr_login = ?, usr_password = ? WHERE usr_login = ?");
    pstmt->setString(1, usrName);
    pstmt->setString(2, usrLogin);
    pstmt->setString(3, usrPassword);
    pstmt->setString(4, usrLogin);
    pstmt->executeUpdate();
    printf("User information updated.\n");
}

void USR_DAO::deletenUSR(const std::string& usrLogin, const std::string& usrPassword)
{
    pstmt = server_func.getCon()->prepareStatement("SELECT * FROM User WHERE usr_login = ? AND usr_password = ?");
    pstmt->setString(1, usrLogin);
    pstmt->setString(2, usrPassword);
    result = pstmt->executeQuery();
    if (result->rowsCount() == 1) 
    {
        pstmt = server_func.getCon()->prepareStatement("DELETE FROM User WHERE usr_login = ?");
        pstmt->setString(1, usrLogin);
        pstmt->executeUpdate();
        printf("User deleted.\n");
    }
    else 
    {
        printf("Failed to delete user. Incorrect login or password.\n");
    }
}

bool USR_DAO::isLoggedIn()
{
    return t;
}