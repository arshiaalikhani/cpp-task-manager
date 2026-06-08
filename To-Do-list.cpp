#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

// ─── Data ────────────────────────────────────────────────────────────────────

struct Task {
    std::string title;
    std::string description;
    std::string priority; // "high" | "medium" | "low"
    std::string deadline; // YYYY-MM-DD
    bool completed;
};

// ─── Helpers ─────────────────────────────────────────────────────────────────

std::string statusToString(bool completed) {
    return completed ? "Done" : "Pending";
}

bool isValidPriority(const std::string& p) {
    return p == "high" || p == "medium" || p == "low";
}

int priorityRank(const std::string& p) {
    if (p == "high")   return 1;
    if (p == "medium") return 2;
    return 3;
}

// ─── File I/O ─────────────────────────────────────────────────────────────────

void saveTask(const Task& task) {
    std::ofstream file("tasks.txt", std::ios::app);
    file << task.title       << "|"
         << task.description << "|"
         << task.priority    << "|"
         << task.deadline    << "|"
         << task.completed   << "\n";
}

std::vector<Task> loadTasks() {
    std::vector<Task> tasks;
    std::ifstream file("tasks.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Task task;
        std::string completedStr;

        std::getline(ss, task.title,       '|');
        std::getline(ss, task.description, '|');
        std::getline(ss, task.priority,    '|');
        std::getline(ss, task.deadline,    '|');
        std::getline(ss, completedStr,     '|');

        task.completed = (completedStr == "1");
        tasks.push_back(task);
    }
    return tasks;
}

void saveAllTasks(const std::vector<Task>& tasks) {
    std::ofstream file("tasks.txt");
    for (const auto& task : tasks) {
        file << task.title       << "|"
             << task.description << "|"
             << task.priority    << "|"
             << task.deadline    << "|"
             << task.completed   << "\n";
    }
}

// ─── Display ─────────────────────────────────────────────────────────────────

void displayTask(const Task& task, int index) {
    std::cout << "\n[" << index << "] " << task.title
              << "\n   Description : " << task.description
              << "\n   Priority     : " << task.priority
              << "\n   Deadline     : " << task.deadline
              << "\n   Status       : " << statusToString(task.completed) << "\n";
}

void displayAllTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks found.\n";
        return;
    }
    std::cout << "\n========== ALL TASKS ==========\n";
    for (size_t i = 0; i < tasks.size(); ++i)
        displayTask(tasks[i], static_cast<int>(i));
    std::cout << "================================\n";
}

// ─── Actions ─────────────────────────────────────────────────────────────────

void addTask() {
    Task t;
    std::cin.ignore();

    std::cout << "Title       : "; std::getline(std::cin, t.title);
    std::cout << "Description : "; std::getline(std::cin, t.description);

    do {
        std::cout << "Priority (high/medium/low): ";
        std::getline(std::cin, t.priority);
        if (!isValidPriority(t.priority))
            std::cout << "  Invalid priority, try again.\n";
    } while (!isValidPriority(t.priority));

    std::cout << "Deadline (YYYY-MM-DD): "; std::getline(std::cin, t.deadline);
    t.completed = false;

    saveTask(t);
    std::cout << "Task added successfully.\n";
}

void searchByPriority() {
    std::string priority;
    std::cin.ignore();
    std::cout << "Priority to search (high/medium/low): ";
    std::getline(std::cin, priority);

    auto tasks = loadTasks();
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].priority == priority) {
            displayTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found)
        std::cout << "No tasks with priority '" << priority << "' found.\n";
}

void searchByStatus() {
    std::string status;
    std::cin.ignore();
    std::cout << "Status to search (done/pending): ";
    std::getline(std::cin, status);

    bool searchCompleted = (status == "done" || status == "Done");
    auto tasks = loadTasks();
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].completed == searchCompleted) {
            displayTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found)
        std::cout << "No " << status << " tasks found.\n";
}

void deleteTask() {
    auto tasks = loadTasks();
    if (tasks.empty()) { std::cout << "No tasks to delete.\n"; return; }

    displayAllTasks(tasks);
    int index;
    std::cout << "Task number to delete: ";
    std::cin >> index;

    if (index >= 0 && index < static_cast<int>(tasks.size())) {
        tasks.erase(tasks.begin() + index);
        saveAllTasks(tasks);
        std::cout << "Task deleted.\n";
    } else {
        std::cout << "Invalid task number.\n";
    }
}

void deleteCompletedTasks() {
    auto tasks = loadTasks();
    int removed = 0;

    for (auto it = tasks.begin(); it != tasks.end();) {
        if (it->completed) { it = tasks.erase(it); ++removed; }
        else               { ++it; }
    }

    if (removed > 0) {
        saveAllTasks(tasks);
        std::cout << "Removed " << removed << " completed task(s).\n";
    } else {
        std::cout << "No completed tasks to remove.\n";
    }
}

void editTaskStatus() {
    auto tasks = loadTasks();
    if (tasks.empty()) { std::cout << "No tasks to edit.\n"; return; }

    displayAllTasks(tasks);
    int index;
    std::cout << "Task number to toggle status: ";
    std::cin >> index;

    if (index >= 0 && index < static_cast<int>(tasks.size())) {
        tasks[index].completed = !tasks[index].completed;
        saveAllTasks(tasks);
        std::cout << "Status changed to: " << statusToString(tasks[index].completed) << "\n";
    } else {
        std::cout << "Invalid task number.\n";
    }
}

void editTaskPriority() {
    auto tasks = loadTasks();
    if (tasks.empty()) { std::cout << "No tasks to edit.\n"; return; }

    displayAllTasks(tasks);
    int index;
    std::cout << "Task number to edit priority: ";
    std::cin >> index;
    std::cin.ignore();

    if (index >= 0 && index < static_cast<int>(tasks.size())) {
        std::string newPriority;
        do {
            std::cout << "New priority (high/medium/low): ";
            std::getline(std::cin, newPriority);
            if (!isValidPriority(newPriority))
                std::cout << "  Invalid priority, try again.\n";
        } while (!isValidPriority(newPriority));

        tasks[index].priority = newPriority;
        saveAllTasks(tasks);
        std::cout << "Priority updated.\n";
    } else {
        std::cout << "Invalid task number.\n";
    }
}

void sortByDeadline() {
    auto tasks = loadTasks();
    std::sort(tasks.begin(), tasks.end(),
              [](const Task& a, const Task& b){ return a.deadline < b.deadline; });
    displayAllTasks(tasks);
}

void sortByPriority() {
    auto tasks = loadTasks();
    std::sort(tasks.begin(), tasks.end(),
              [](const Task& a, const Task& b){
                  return priorityRank(a.priority) < priorityRank(b.priority);
              });
    displayAllTasks(tasks);
}

// ─── Menu ────────────────────────────────────────────────────────────────────

void showMenu() {
    std::cout << "\n======= TO-DO LIST MANAGER =======\n"
              << " 1. Add new task\n"
              << " 2. Show all tasks\n"
              << " 3. Search by priority\n"
              << " 4. Search by status\n"
              << " 5. Delete a task\n"
              << " 6. Delete all completed tasks\n"
              << " 7. Toggle task status\n"
              << " 8. Edit task priority\n"
              << " 9. Sort by deadline\n"
              << "10. Sort by priority\n"
              << " 0. Exit\n"
              << "==================================\n"
              << "Choice: ";
}

// ─── Main ────────────────────────────────────────────────────────────────────

int main() {
    int choice;
    while (true) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:  addTask();                    break;
            case 2:  displayAllTasks(loadTasks()); break;
            case 3:  searchByPriority();           break;
            case 4:  searchByStatus();             break;
            case 5:  deleteTask();                 break;
            case 6:  deleteCompletedTasks();       break;
            case 7:  editTaskStatus();             break;
            case 8:  editTaskPriority();           break;
            case 9:  sortByDeadline();             break;
            case 10: sortByPriority();             break;
            case 0:  std::cout << "Goodbye!\n"; return 0;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
