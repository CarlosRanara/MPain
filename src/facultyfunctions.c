#include "headers.h"
#include "mainfunctions.c"

int searchFaculty(databaseType *db, String10 key)
{
    int i, found;

    found = 0;
    i = 0;

    while (!found && i < db->faculty_count)
        if (strcmp(key, db->faculty[i++].id) == 0)
            found = 1;
    

    return found ? --i : -1;
}

int facultyLogIn(databaseType *db)
{
    int idx;
    String10 faculty_id;
    printf("Faculty Information\n");
    printf("Enter ID#: ");
    scanf("%s", faculty_id);
 
    idx = searchStudent(db, faculty_id);

    return idx;
}
void facultyMainMenu(databaseType *db, int *exit_main)
{
    int choice;

    printf("Faculty Menu\n");
    printf("1] Enter Faculty Deloading Units\n");
    printf("2] Select Course Load\n");
    printf("3] Print Course Load\n");
    printf("4] Print Student List\n");
    printf("5] Exit to Return to Main Menu\n");
    printf("Answer: ");
    scanf("%d", &choice);

   switch (choice) {
        case 1: facultyDeloadingUnits(); break;
        case 2: facultyCourseLoad(); break;
        case 3: facultyPrintCourse(); break;
        case 4: printStudentList(); break;
        case 5: *exit_main = 0; break;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
   }

}

void facultyDeloadingUnits() 
{

 /* teacher adds more units and then does this after
 get deloading_units if it <= MAX_UNITS go and do 12 - deloading units and say blah blah */

}

void facultyCourseLoad()
{
  /*
  Copy paste student enrolls
  */
}

void facultyPrintCourses()
{
    /*
    print
    */

}

void printStudentList()
{
    /* Locate studets and print list*/
}