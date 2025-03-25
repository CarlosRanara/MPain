#include "headers.h"
#include "mainfunctions.c"

void studentMainMenu(databaseType *db, int *exit_main)
{
    int idx;
    int exit_flag;
    String10 student_id;

    exit_flag = 0;

    printf("Student Menu\n");
    printf("Enter Student ID: ");
    scanf("%s", student_id);

    idx = searchStudent(db, student_id);

    if (idx != -1)
        while(!exit_flag)
            studentAccountMenu(db->students[idx], &exit_flag);
    else
        fprintf(stderr, "ERROR: Student %s not found.", student_id);
}


void studentAccountMenu(studentType student, int *exit_flag)
{
    int choice;

    print("Choose your transaction: ");
    print("1] Enroll Courses");
    print("2] Print EAF");
    print("3] Exit to Return to Main Menu");
    scanf("%d", &choice);

    switch (choice) {
        case 1: studentEnroll(&student); break;
        case 2: studentEAF(&student); break;
        case 3: *exit_flag = 1; break;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
    }
}

int searchStudent(databaseType *db, String10 key)
{
    int i, found;

    found = 0;
    i = 0;

    while (!found && i < db->student_count)
        if (strcmp(key, db->students[i++].id) == 0)
            found = 1;
    

    return found ? --i : -1;
}

void studentEAF(studentType *db)
{
    /* look at the eaf and print it in the format*/
}

void studentEnroll(studentType *db)
{
    int choice;

    printf("Enroll Courses\n");
    printf("1] Add Courses\n");
    printf("2] Edit Courses\n");
    printf("3] Delete Courses\n");

    switch (choice) {
        case 1: studentAddCourses(&db); break;
        case 2: studentEditCourses(&db); break;
        case 3: studentDeleteCourses(&db); break;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break; 
    }
}

void studentAddCouses(studentType *db)
{
    printf("Adding Courses\n");
    /* call studentEAF if not display
    you have no enrolled*/
    /*print offered courses*/
    /* scanf 2 strings the course code and the section
    validation if its there if have prereq for it if EXIT then ur done*/
    /*once call StudentEAF and go back*/    
}

void studentEditCourses(studentType *db) 
{
    printf("Editing Courses\n");
    /* call studentEAF if not display
    you have no enrolled end it*/
 
}

void studentDeleteCourses(studentType *db)
{
    printf("Delete Courses\n");
    /* call studentEAF if not display
    you have no enrolled end it*/ 
}