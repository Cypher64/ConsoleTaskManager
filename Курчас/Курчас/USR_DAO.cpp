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

bool USR_DAO::isLoggedIn() { return t; }

void USR_DAO::registerUSR(User& user)
{
    try
    {
        pstmt = server_func.getCon()->prepareStatement("SELECT COUNT(*) FROM User WHERE usr_email = ?");
        pstmt->setString(1, user.getEmail());
        result = pstmt->executeQuery();
        result->next();
        int userCount = result->getInt(1);
        if (userCount > 0)
        {
            std::cerr << "User with this Email already exists. Registration failed." << std::endl;
            return;
        }

        pstmt = server_func.getCon()->prepareStatement("SELECT COUNT(*) FROM User WHERE usr_name = ?");
        pstmt->setString(1, user.getName());
        result = pstmt->executeQuery();
        result->next();
        userCount = result->getInt(1);
        if (userCount > 0)
        {
            std::cerr << "User with this User Name already exists. Registration failed." << std::endl;
            return;
        }

        pstmt = server_func.getCon()->prepareStatement("INSERT INTO User(usr_name, usr_email, usr_password) VALUES(?,?,?)");
        pstmt->setString(1, user.getName());
        pstmt->setString(2, user.getEmail());
        pstmt->setString(3, user.getPWD());
        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected > 0) { std::cout << "User registered successfully." << std::endl; }
        else { std::cerr << "Failed to register user. Please try again." << std::endl; }
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }
}

void USR_DAO::LoginUSR(const std::string& email, const std::string& password)
{
    pstmt = server_func.getCon()->prepareStatement("SELECT * FROM User WHERE usr_email = ? AND usr_password = ?");
    pstmt->setString(1, email);
    pstmt->setString(2, password);
    result = pstmt->executeQuery();
    if (result->next())
    {
        std::cout << "Login successful." << std::endl;
        t++;
    }
    else { std::cout << "Login failed. Incorrect login or password." << std::endl; }
}


void USR_DAO::updateName(const std::string& usrName, int ID)
{
    try
    {
        pstmt = server_func.getCon()->prepareStatement("UPDATE User SET usr_name = ? WHERE id = ?");
        pstmt->setString(1, usrName);
        pstmt->setInt(2, ID);
        pstmt->executeUpdate();
        std::cout << "Username updated successfully." << std::endl;
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }
}

void USR_DAO::updateEmail(const std::string& usrEmail, int ID)
{
    try 
    {
        pstmt = server_func.getCon()->prepareStatement("SELECT * FROM User WHERE usr_email = ?");
        pstmt->setString(1, usrEmail);
        result = pstmt->executeQuery();
        if (result->rowsCount() == 0) 
        {
            pstmt = server_func.getCon()->prepareStatement("UPDATE User SET usr_email = ? WHERE id = ?");
            pstmt->setString(1, usrEmail);
            pstmt->setInt(2, ID);
            pstmt->executeUpdate();
            std::cout << "Email updated successfully." << std::endl;
        }
        else { std::cerr << "Failed to update user information. New email is already in use." << std::endl; }
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }
}

void USR_DAO::updatePassword(const std::string& usrPassword, int ID)
{
    try 
    {
        pstmt = server_func.getCon()->prepareStatement("UPDATE User SET usr_password = ? WHERE id = ?");
        pstmt->setString(1, usrPassword);
        pstmt->setInt(2, ID);
        pstmt->executeUpdate();
        std::cout << "Password updated successfully." << std::endl;
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }
}

void USR_DAO::deleteUSR(const std::string& usrPassword, int ID)
{
    char confirmation;
    std::cout << "Are you sure you want to delete your account? (y/n): ";
    std::cin >> confirmation;
    if (confirmation != 'y' && confirmation != 'Y') 
    {
        std::cout << "Account deletion cancelled." << std::endl;
        return;
    }

    if (!checkPassword(ID, usrPassword)) { return; }

    try 
    {
        pstmt = server_func.getCon()->prepareStatement("DELETE FROM Task WHERE usr_id = ?");
        pstmt->setInt(1, ID);
        pstmt->executeUpdate();

        pstmt = server_func.getCon()->prepareStatement("DELETE FROM User WHERE id = ?");
        pstmt->setInt(1, ID);
        pstmt->executeUpdate();

        std::cout << "User deleted successfully." << std::endl;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}


int USR_DAO::getUserID(const std::string& email)
{
    int userID = -1;

    try
    {
        pstmt = server_func.getCon()->prepareStatement("SELECT id FROM User WHERE usr_email = ?");
        pstmt->setString(1, email);
        result = pstmt->executeQuery();

        if (result->next()) { userID = result->getInt("id"); }
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }

    return userID;
}

bool USR_DAO::checkPassword(int ID, const std::string& currentPassword)
{
    try 
    {
        pstmt = server_func.getCon()->prepareStatement("SELECT usr_password FROM User WHERE id = ?");
        pstmt->setInt(1, ID);
        result = pstmt->executeQuery();

        if (result->rowsCount() != 1) 
        {
            std::cerr << "User not found." << std::endl;
            return false;
        }

        result->next();
        std::string storedPassword = result->getString("usr_password");

        if (storedPassword == currentPassword) { return true; }
        else 
        {
            std::cerr << "Incorrect password." << std::endl;
            return false;
        }
    }
    catch (const sql::SQLException& e) 
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        return false;
    }
}

void USR_DAO::showUSR(int ID)
{
    try
    {
        pstmt = server_func.getCon()->prepareStatement("SELECT usr_name, usr_email FROM User WHERE id = ?");
        pstmt->setInt(1, ID);
        result = pstmt->executeQuery();

        if (result->next())
        {
            std::string usrName = result->getString("usr_name");
            std::string usrEmail = result->getString("usr_email");
            std::cout << "User name: " << usrName << std::endl << "Email: " << usrEmail << std::endl;
        }
        else { std::cerr << "User with ID " << ID << " not found." << std::endl; }
    }
    catch (const sql::SQLException& e) { std::cerr << "SQL Error: " << e.what() << std::endl; }
}