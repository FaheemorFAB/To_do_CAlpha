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
    string description;
    bool completed;
    string priority; // Priority level of the task
    string dueDate;  // Due date of the task (format: dd/mm/yyyy)

    Task(const string &desc, const string &prio, const string &date)
        : description(desc), completed(false), priority(prio), dueDate(date) {}
};

class ToDoList
{
private:
    vector<Task> tasks;

public:
    bool addTask(const string &task, const string &priority, const string &dueDate)
    {
        // Validate due date format and compare with system date
        if (isValidDueDate(dueDate))
        {
            tasks.emplace_back(task, priority, dueDate);
            cout<<endl<<"Task Added Successfully"<<endl;
            return true;
        }
        else
        {
            cerr << "Invalid due date. Please enter a valid due date (format: dd/mm/yyyy)." <<endl;
            return false;

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
                cout << "Task already completed." << endl;
                return false;
            }
        }
        else
        {
            cout << "Invalid task number." << endl;
            return false;
        }
    }

    bool showTasks() const
    {
        if (tasks.empty())
        {
            cout << "No tasks to display." << endl;
            return false;
        }
        for (size_t i = 0; i < tasks.size(); ++i)
        {
            cout << i + 1 << ". " << tasks[i].description
                      << " [Priority: " << tasks[i].priority << "]"
                      << " [Due Date: " << tasks[i].dueDate << "]"
                      << (tasks[i].completed ? " (completed)" : "") << endl;
        }
        return true;
    }

    void sortByPriority()
    {
        sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b)
                  {
            // Assuming priority levels are strings like "high", "medium", "low"
            return a.priority < b.priority; });
    }

    bool saveTasksToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file for writing." << endl;
            return false;
        }
        for (const Task &task : tasks)
        {
            file << task.description << "," << task.priority << "," << task.dueDate << "," << task.completed << endl;
        }
        file.close();
        cout << "Tasks saved to file: " << filename << endl;
        return true;
    }

private:
    bool isValidDueDate(const string &dueDate)
{
    tm dueDateTm = {};
    istringstream ss(dueDate);
    
    // Attempt to parse the due date string
    ss >> get_time(&dueDateTm, "%d/%m/%Y");
    if (ss.fail())
    {
        return false; // Invalid format
    }

    time_t sysTime_t = time(nullptr); // Get current system time
    time_t dueDate_t = mktime(&dueDateTm); // Convert tm structure to time_t
    
    return dueDate_t >= sysTime_t; // Due date should be equal to or after system date
}

};

int main()
{
    ToDoList myList;
    string input, priority, dueDate;
    int choice, taskNumber;
    string filename; // Declare filename variable here

    while (true)
    {
        cout<<endl<<"-----------------------"<<endl;
        cout << "\nTo-Do List Application\n";
        cout << "1. Add Task\n";
        cout << "2. Complete Task\n";
        cout << "3. Show Tasks\n";
        cout << "4. Sort Tasks by Priority\n";
        cout << "5. Save Tasks to File\n";
        cout << "6. Exit\n";
        cout <<endl<< "Enter your choice: ";
        cin >> choice;
        cout<<endl<<"-----------------------"<<endl;

        switch (choice)
        {
        case 1:
            cout << "Enter task description: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, input);
            cout << "Enter priority level (e.g., high, medium, low): ";
            getline(cin, priority);
            cout << "Enter due date (format: dd/mm/yyyy): ";
            getline(cin, dueDate);

            if(!myList.addTask(input, priority, dueDate))
            {
            cout << "Enter due date (format: dd/mm/yyyy): ";
            getline(cin, dueDate);
            myList.addTask(input, priority, dueDate);
            break;
        case 2:
            if(!myList.showTasks())
            break;
            cout << "Enter task number to complete: ";
            if (!(cin >> taskNumber))
            {
                cout << "Invalid input. Please enter a valid number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            if (myList.completeTask(taskNumber - 1))
            {
                cout << "Task marked as completed." << endl;
            }
            break;
        case 3:
            myList.showTasks();
            break;
        case 4:
            myList.sortByPriority();
            cout << "Tasks sorted by priority." << endl;
            break;
        case 5:
            cout << "Enter filename to save tasks: ";
            cin >> filename;
            if (myList.saveTasksToFile(filename))
            {
                cout << "Tasks saved successfully." << endl;
            }
            else
            {
                cout << "Failed to save tasks." << endl;
            }
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return 0;
}
