🗂️ C++ Task Manager (CLI)

A simple and efficient command-line task management system built with C++.
This project allows users to manage their daily tasks with full file-based persistence.

🚀 Features
➕ Add new tasks
📋 View all tasks
🔍 Search tasks by priority
🔎 Search tasks by status (done / pending)
✏️ Edit task priority
🔁 Toggle task completion status
🗑️ Delete a single task
🧹 Delete all completed tasks
📊 Sort tasks by deadline
📊 Sort tasks by priority
💾 Data Storage

All tasks are stored in a local file:

tasks.txt

Each task is saved in this format:

title|description|priority|deadline|completed

Example:

Math Homework|Chapter 3 exercises|high|2026-06-10|0
🧠 Concepts Used
File handling (fstream)
Structs in C++
Vectors
String parsing (stringstream)
Sorting with custom comparator
CLI-based user interaction
Basic CRUD operations
🛠️ How to Run
1. Compile:
g++ main.cpp -o taskmanager
2. Run:
./taskmanager

(On Windows: taskmanager.exe)

📂 Project Structure
📁 project-folder
 ├── main.cpp
 ├── tasks.txt (auto-generated)
 └── README.md
🎯 Purpose

This project was built for learning C++ fundamentals, including file handling, data structures, and real-world application logic.

🔥 Future Improvements
GUI version (Qt or SFML)
Task categories / tags
Due date reminders
Encryption for saved data
Database version (SQLite)
👨‍💻 Author

Made with 💻 in C++
For learning and practice purposes.
