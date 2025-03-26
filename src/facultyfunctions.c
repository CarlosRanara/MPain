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
 
    idx = searchFaculty(db, faculty_id);

    return idx;
}
void facultyMainMenu(databaseType *db, int *exit_main)
{
    int idx = facultyLogIn(&db);
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
        case 1: facultyDeloadingUnits(&db, idx); break;
        case 2: facultyCourseLoad(&db, idx); break;
        case 3: facultyPrintCourse(); break;
        case 4: printStudentList(); break;
        case 5: *exit_main = 0; break;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
   }
}

void facultyDeloadingUnits(databaseType *db, int idx) 
{
    int toAdd = 0;
    printf("Enter Deloading Units: ");
    scanf("%d", &toAdd);
    db->faculty[idx].deloading_units += toAdd;
    printf("You can teach %d units of courses", MAX_UNITS - db->faculty->deloading_units); 
}

void displayAvailableCourses(databaseType *db) {
    printf("\nAvailable Courses:\n");
    for (int i = 0; i < db->course_count; i++) {
        printf("[%d] %s - %s (%d units)\n", i + 1, db->courses[i].code, db->courses[i].section, db->courses[i].units);
    }
}

int isCourseAlreadyAssigned(databaseType *db, int facultyIdx, int courseIdx) {
    for (int i = 0; i < db->faculty[facultyIdx].deloading_units; i++) {
        if (strcmp(db->faculty[facultyIdx].assigned_courses[i], db->courses[courseIdx].code) == 0) {
            return 1;  // Course is already assigned
        }
    }
    return 0;
}

int isCourseDeletable(databaseType *db, int courseIdx) {
    for (int i = 0; i < db->student_count; i++) {
        for (int j = 0; j < db->students[i].enrolled_count; j++) {
            if (strcmp(db->students[i].enrolled_courses[j], db->courses[courseIdx].code) == 0) {
                return 0;  // Cannot delete, students are enrolled
            }
        }
    }
    return 1;  // Course can be deleted
}

void addCourseToFaculty(databaseType *db, int facultyIdx) {
    int courseIdx;
    printf("Enter Course Index to Add: ");
    scanf("%d", &courseIdx);
    courseIdx--;

    if (isCourseAlreadyAssigned(db, facultyIdx, courseIdx)) {
        printf("Course already assigned!\n");
        return;
    }

    int totalUnits = db->faculty[facultyIdx].deloading_units + db->courses[courseIdx].units;
    if (totalUnits > MAX_UNITS) {
        printf("Cannot add! Overload limit exceeded.\n");
        return;
    }

    strcpy(db->faculty[facultyIdx].assigned_courses[db->faculty[facultyIdx].deloading_units], db->courses[courseIdx].code);
    db->faculty[facultyIdx].deloading_units += db->courses[courseIdx].units;
    printf("Course added successfully!\n");
}

void removeCourseFromFaculty(databaseType *db, int facultyIdx) {
    int courseIdx;
    printf("Enter Course Index to Remove: ");
    scanf("%d", &courseIdx);
    courseIdx--;

    if (!isCourseDeletable(db, courseIdx)) {
        printf("Cannot delete! Students are already enrolled.\n");
        return;
    }

    for (int i = courseIdx; i < db->faculty[facultyIdx].deloading_units - 1; i++) {
        strcpy(db->faculty[facultyIdx].assigned_courses[i], db->faculty[facultyIdx].assigned_courses[i + 1]);
    }
    db->faculty[facultyIdx].deloading_units -= db->courses[courseIdx].units;
    printf("Course removed successfully!\n");
}

void facultyCourseLoad(databaseType *db, int idx) {
    int choice;
    while (1) {
        printf("\n1. Add Course Load\n2. Remove Course Load\n3. Return to Faculty Menu\nChoice: ");
        scanf("%d", &choice);

        if (choice == 3) return;

        switch (choice) {
            case 1:
                displayAvailableCourses(db);
                addCourseToFaculty(db, idx);
                break;

            case 2:
                removeCourseFromFaculty(db, idx);
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }
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