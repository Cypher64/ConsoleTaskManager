#include "task.h"
#include <string>

Task::Task() : task_name(""), task_description(""), task_priority(Priority::Low), task_status(Status::Not_started) {}
Task::Task(std::string name, std::string description, Priority priority, Status status) : task_name(name), task_description(description), 
																						  task_priority(priority), task_status(status) {}

std::string Task::getName() { return task_name; }
std::string Task::getDescription() { return task_description; }
Priority Task::getPriority() const { return task_priority; }
Status Task::getStatus() const { return task_status; }