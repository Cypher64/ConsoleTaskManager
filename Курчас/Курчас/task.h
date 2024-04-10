#pragma once
#include <string>

enum class Priority { High, Medium, Low };
enum class Status { In_progress, Done, not_started };

class Task
{
public:
	Task(std::string name, std::string description, Priority priority, Status status);

	std::string getName();
	std::string getDescription();
	Priority getPriority();
	Status getStatus();
private:
	std::string task_name;
	std::string task_description;
	Priority task_priority;
	Status task_status;
};