#include "headers.h"
#include "mainfunctions.c"

void assistantMenu(databaseType *db, int *exit_main)
{
    int choice;

    printf("Academic Assistant Menu\n");
    printf("1] Enter student information\n");
    printf("2] Enter faculty information\n");
    printf("3] Enter courses and prerequisites\n");
    printf("4] Schedule and Room Assignment\n");    
    printf("5] Display Faculty List\n");
    printf("6] Exit to Return to Main Menu\n");

    switch (choice) {
        case 1: newStudentInfo(); break;
        case 2: newFacultyInfo(); break;
        case 3: newCoursePrereq(); break;
        case 4: scheduleRooms(); break;
        case 5: displayFacultyList(); break;
        case 6: *exit_main = 0;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
    }
}

void newStudentInfo()
{
}

void newFacultyInfo()
{
}

void newCoursePrereq()
{
}

void scheduleRooms()
{
}

void displayFacultyList()
{
}