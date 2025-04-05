📌 Enrollment System

📖 Overview

This Enrollment System is a simple yet functional program designed for student course registration, faculty course selection, prerequisite validation, and automated room scheduling. The system supports three roles:

Student: Enroll in courses, manage enrolled subjects, and generate an Enrolment Assessment Form (EAF).

Faculty: Choose course loads while maintaining unit restrictions and review student enrollments.

Academic Assistant: Manage students, faculty, courses, prerequisites, schedules, and room assignments.

🚀 Features

✅ Structured Enrollment Process✅ Faculty Course Load Management✅ Prerequisite Verification✅ Automated Room and Schedule Assignment✅ Comprehensive File Handling (Text & Optional Binary Storage)✅ Student and Faculty Reports Generation✅ User-friendly CLI Interface

🛠️ System Requirements

Compiler: GCC with C99 standard (gcc -Wall -std=c99)

Development Environment: Linux/macOS/Windows with a terminal

Files Used:

students.txt – Student details & enrolled courses

faculty.txt – Faculty information & selected course loads

courses.txt – Course offerings

prerequisite.txt – Course prerequisite mapping

rooms.txt – Room and schedule assignments

🏗️ Installation & Compilation

Clone the repository and navigate to the project directory:

git clone <repo-link>
cd Enrollment-System

Compile the program:

gcc -Wall -std=c99 main.c student.c faculty.c assistant.c file_handler.c utils.c -o enrollment_system

Run the program:

./enrollment_system

🎯 Usage Guide

Upon running the program, users are presented with a Main Menu:

1. Student
2. Faculty
3. Academic Assistant
4. Exit

Each role has specific actions:

Students can enroll in courses, edit/delete enrollments, and generate an EAF.

Faculty can manage course loads, check loading status, and print assigned students.

Academic Assistant can add/edit/delete students, faculty, courses, prerequisites, and room assignments.

📌 Code Standards & Restrictions

To ensure code quality and maintainability, the following rules are enforced: ❌ No global variables❌ No goto, break (except in switch), return for early exits, or exit()❌ No fgets(), strcspn(), or memset()✅ Strict error handling and structured file operations✅ Proper function documentation and modular design

📊 Sample Output

Main Menu:
1. Student
2. Faculty
3. Academic Assistant
Answer: 1

Student Menu:
Enter Student ID: 123456
1. Enroll Courses
2. Print EAF
3. Exit
Answer: 1

🏆 Bonus Features

Binary file support for improved performance

Advanced error handling mechanisms

More detailed student/faculty reports

⏳ Deadline & Submission

📅 Final Submission: March 31, 2025, 07:30 AM📌 Submission via: AnimoSpace📌 Required Deliverables:

Source Code (.c & .h files)

Test Script (Formatted in a table with test cases)

Sample Output Files (Generated from program)


