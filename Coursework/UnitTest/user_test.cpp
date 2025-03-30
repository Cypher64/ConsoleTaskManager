#include "pch.h"
#include "../Coursework/User.h"


TEST(UserTest, Constructor)
{
    std::string name = "Alice";
    std::string email = "Alice@example.com";
    std::string password = "password123";

    User user(name, email, password);

    EXPECT_EQ(user.getName(), name);
    EXPECT_EQ(user.getEmail(), email);
    EXPECT_EQ(user.getPWD(), password);
}

TEST(UserTest, getName)
{
    User user("John Doe", "john@example.com", "password");
    EXPECT_EQ(user.getName(), "John Doe");
}

TEST(UserTest, getEmail)
{
    User user("Bob Dir", "bob@example.com", "password111");
    EXPECT_EQ(user.getEmail(), "bob@example.com");
}

TEST(UserTest, getPWD)
{
    User user("Mike Wazovky", "john@example.com", "password333");
    EXPECT_EQ(user.getPWD(), "password333");
}