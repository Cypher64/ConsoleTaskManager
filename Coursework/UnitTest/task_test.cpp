#include "pch.h"
#include "../Coursework/task.h"

TEST(TaskTest, DefaultConstructor)
{
    Task task;
    EXPECT_EQ(task.getName(), "");
    EXPECT_EQ(task.getDescription(), "");
    EXPECT_EQ(task.getPriority(), Priority::Low);
    EXPECT_EQ(task.getStatus(), Status::Not_started);
}

TEST(TaskTest, ParameterizedConstructor) 
{
    Task task("Sample Task", "Sample Description", Priority::High, Status::In_progress);
    EXPECT_EQ(task.getName(), "Sample Task");
    EXPECT_EQ(task.getDescription(), "Sample Description");
    EXPECT_EQ(task.getPriority(), Priority::High);
    EXPECT_EQ(task.getStatus(), Status::In_progress);
}

TEST(TaskTest, GetName)
{
    Task task("Task1", "Description1", Priority::High, Status::Not_started);
    EXPECT_EQ(task.getName(), "Task1");
}

TEST(TaskTest, GetDescription)
{
    Task task("Task1", "Description1", Priority::High, Status::Not_started);
    EXPECT_EQ(task.getDescription(), "Description1");
}

TEST(TaskTest, GetPriority)
{
    Task task("Task1", "Description1", Priority::High, Status::Not_started);
    EXPECT_EQ(task.getPriority(), Priority::High);
}

TEST(TaskTest, GetStatus)
{
    Task task("Task1", "Description1", Priority::High, Status::Not_started);
    EXPECT_EQ(task.getStatus(), Status::Not_started);
}