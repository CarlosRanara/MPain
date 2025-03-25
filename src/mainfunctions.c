#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers.h"

void initData(databaseType *db)
{
    db->student_count = 0;
    db->faculty_count = 0;
    db->course_count = 0;
    db->prereq_count = 0;
    db->room_count = 0;
}

void loadData(databaseType *db)
{
    // make sure file exists
    // if file exists, then put the data in the struct(s)
    // if not then we start blank

    FILE *fp;

    String10 temp;

    fp = fopen(STUDENT_FILE, "r");
    if (fp != NULL) {
        while ( fscanf(fp, "%s", temp) != EOF ) {

        }
    }
}

void saveData(databaseType *db)
{
}

/*
    Purpose: determine if the search key is in the array or not using a linear
    Returns: either the (a) index of the array where the key was found or
    @param : db is a pointer to the main database containing the information to
             be stored in the text files
    @param : exit_flag is a flag to determine when to quit the program
    Pre-condition: the parameters contain valid values
*/
void mainMenu(databaseType *db, int *exit_flag)
{
    int choice;
    int exit_main = 1;

    printf("\nMain Menu\n");
    printf("1] Student\n");
    printf("2] Faculty\n");
    printf("3] Academic Assistant\n");
    printf("4] Exit\n");
    printf("Answer: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: 
            while (exit_main) {
                studentMainMenu(&db, &exit_main); 
            } break;
        case 2: 
            while(exit_main) {
                 facultyMainMenu(&db, &exit_main); 
            } break;
        case 3:
            while(exit_main) {
                assistantMainMenu(&db, &exit_main);
            } break;
        case 4: *exit_flag = 1; break;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
    }
}



