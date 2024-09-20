#include <iostream>
#include <vector>
#include <string>
#include <fstream>   // For file operations
#include <algorithm> // For sorting
#include <ctime>     // For system date and time
#include <iomanip>   // For date formatting
#include <limits>    // For std::numeric_limits
#include <sstream>   // Allows reading formatted input from strings like keyboard input

using namespace std;

class Task
{
public:
    std::string description;
    bool completed;
    std::string priority; // Priority level of the task
    std::string dueDate;  // Due date of the task (format: dd/mm/yyyy)

    Task(const std::string &desc, const std::string &prio, const std::string &date)
        : description(desc), completed(false), priority(prio), dueDate(date) {}
};

class ToDoList
{
private:
    std::vector<Task> tasks;

public:
    void addTask(const std::string &task, const std::string &priority, const std::string &dueDate)
    {
        // Validate due date format and compare with system date
        if (isValidDueDate(dueDate))
        {
            tasks.emplace_back(task, priority, dueDate);
        }
        else
        {
            std::cout << "Invalid due date. Please enter a valid due date (format: dd/mm/yyyy)." << std::endl;
        }
    }

    bool completeTask(int index)
    {
        if (index >= 0 && index < tasks.size())
        {
            if (!tasks[index].completed)
            {
                tasks[index].completed = true;
                return true;
            }
            else
            {
                std::cout << "Task already completed." << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Invalid task number." << std::endl;
            return false;
        }
    }

    void showTasks() const
    {
        if (tasks.empty())
        {
            std::cout << "No tasks to display." << std::endl;
            return;
        }
        for (size_t i = 0; i < tasks.size(); ++i)
        {
            std::cout << i + 1 << ". " << tasks[i].description
                      << " [Priority: " << tasks[i].priority << "]"
                      << " [Due Date: " << tasks[i].dueDate << "]"
                      << (tasks[i].completed ? " (completed)" : "") << std::endl;
        }
    }

    void sortByPriority()
    {
        std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b)
                  {
            // Assuming priority levels are strings like "high", "medium", "low"
            return a.priority < b.priority; });
    }

    bool saveTasksToFile(const std::string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Error: Unable to open file for writing." << std::endl;
            return false;
        }
        for (const Task &task : tasks)
        {
            file << task.description << "," << task.priority << "," << task.dueDate << "," << task.completed << std::endl;
        }
        file.close();
        std::cout << "Tasks saved to file: " << filename << std::endl;
        return true;
    }

private:
    bool isValidDueDate(const std::string &dueDate) const
    {
        std::tm dueDateTm;
        // Attempt to parse the due date string
        if (strptime(dueDate.c_str(), "%d/%m/%Y", &dueDateTm) == nullptr)
        {
            return false; // Invalid format
        }
        std::time_t sysTime_t = std::time(nullptr); // Get current system time
        std::time_t dueDate_t = std::mktime(&dueDateTm);
        return dueDate_t >= sysTime_t; // Due date should be equal to or after system date
    }
};

int main()
{
    ToDoList myList;
    std::string input, priority, dueDate;
    int choice, taskNumber;
    std::string filename; // Declare filename variable here

    while (true)
    {
        std::cout << "\nTo-Do List Application\n";
        std::cout << "1. Add Task\n";
        std::cout << "2. Complete Task\n";
        std::cout << "3. Show Tasks\n";
        std::cout << "4. Sort Tasks by Priority\n";
        std::cout << "5. Save Tasks to File\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Enter task description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, input);
            std::cout << "Enter priority level (e.g., high, medium, low): ";
            std::getline(std::cin, priority);
            std::cout << "Enter due date (format: dd/mm/yyyy): ";
            std::getline(std::cin, dueDate);
            myList.addTask(input, priority, dueDate);
            break;
        case 2:
            myList.showTasks();
            std::cout << "Enter task number to complete: ";
            if (!(std::cin >> taskNumber))
            {
                std::cout << "Invalid input. Please enter a valid number." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            if (myList.completeTask(taskNumber - 1))
            {
                std::cout << "Task marked as completed." << std::endl;
            }
            break;
        case 3:
            myList.showTasks();
            break;
        case 4:
            myList.sortByPriority();
            std::cout << "Tasks sorted by priority." << std::endl;
            break;
        case 5:
            std::cout << "Enter filename to save tasks: ";
            std::cin >> filename;
            if (myList.saveTasksToFile(filename))
            {
                std::cout << "Tasks saved successfully." << std::endl;
            }
            else
            {
                std::cout << "Failed to save tasks." << std::endl;
            }
            break;
        case 6:
            return 0;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
