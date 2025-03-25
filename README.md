Enrollment System

Overview

This project is a C-based Enrollment System developed for Term 2 of the 2024-25 academic year. It enables students to enroll in courses, faculty members to manage their teaching loads, and an academic assistant to oversee the entire enrollment process. The system handles prerequisite validation, automatic room and schedule assignment, and structured file management.

Features

1. Student Module

Maximum of 20 students.

Enroll in courses (system ensures prerequisites are met).

Modify enrollment (add, edit, delete courses).

Generate Enrollment Assessment Form (EAF):

Displays on screen.

Saves in a properly formatted text file (students.txt).

2. Faculty Module

Maximum of 20 faculty members.

Select teaching loads (ensuring a maximum of 12 units).

Modify assigned courses (add, edit, delete courses).

View assigned courses and teaching load status.

Generate reports:

Faculty course load report.

Student list per course.

Data stored in faculty.txt.

3. Academic Assistant Module

Manage student and faculty records (add, edit, delete entries).

Manage courses and prerequisites.

Schedule courses and assign rooms.

Generate reports and maintain structured files:

students.txt (Student data and EAFs)

faculty.txt (Faculty data and course loads)

courses.txt (Available courses and schedules)

prerequisite.txt (Course prerequisite mapping)

rooms.txt (Room assignments and schedules)

Data Structures

The system follows a structured approach with arrays of structs to manage:

Students (max 20): Name, ID, program, enrolled courses, past courses.

Faculty (max 20): ID, department, deloading units, assigned courses.

Courses: Code, section, units, schedule, room, faculty.

File Handling

The system supports text-based storage for structured data management. Bonus points available for implementing binary file storage.

File Formats

students.txt

STUDENT 1
<name> <ID>
<course 1 taken>
<course 2 taken>
...
EAF
<course code> <section> <units> <day> <time> <room> <faculty>
...

faculty.txt

FACULTY 1
<name> <ID> <department> <deloading units>
<course code> <section>
...

courses.txt

<course code> <section> <units> <day> <time> <room> <faculty>

prerequisite.txt

<prerequisite course> <course>

rooms.txt

<room number> <day> <time> <course code> <section> <occupancy>

Constraints

No global variables.

No goto, break (except in switch), return (for early exits), exit(), fgets(), strcspn(), memset().

Strict error handling for fscanf() return values.

Formatted file storage with consistent parsing structure.

MW/TH 1.5-hour schedule format for course assignments.

Compilation with GCC using -Wall -std=C99 to ensure warnings are resolved.

Implementation Plan

Load data from files into arrays of structs.

Implement main menu and submenus.

Enable user operations (enrollment, faculty selection, course management, etc.).

Save updated data back to structured files.

Thoroughly test and debug.

Optional: Implement binary file storage for bonus points.

Submission Guidelines

Final deadline: March 31, 2025, 07:30 AM (AnimoSpace submission).

Deliverables:

Source code (.c files) with proper documentation.

Test scripts (.txt or table format).

Sample exported text files (students.txt, faculty.txt, etc.).

Demo presentation required (must compile and run with gcc -Wall -std=C99).

Acknowledgment

This project follows the academic integrity policy. Any form of plagiarism or code-sharing outside the group is strictly prohibited.
