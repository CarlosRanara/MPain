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
    scanf("%d", &choice);

    switch (choice) {
        case 1: newStudentInfo(&db); break;
        case 2: newFacultyInfo(&db); break;
        case 3: newCoursePrereq(&db); break;
        case 4: scheduleRooms(&db); break;
        case 5: displayFacultyList(); break;
        case 6: *exit_main = 0;
        default: fprintf(stderr, "ERROR: Choice %d is invalid.", choice); break;
    }
}

void newStudentInfo(databaseType *db) {
    if (db->student_count >= MAX_STUDENTS) {
        fprintf(stderr, "ERROR: Maximum student capacity reached.\n");
        return;
    }

    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", STUDENT_FILE);
        return;
    }

    char id[11], name[31], program[11];
    printf("Enter student ID: ");
    scanf("%10s", id);
    getchar(); // Consume newline

    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline

    printf("Enter program: ");
    scanf("%10s", program);

    // Writing student details to the file
    fprintf(fp, "STUDENT %d\n%s %s\n", db->student_count + 1, name, id);

    // Handling subjects taken
    printf("Enter subjects taken (Type 'Exit' to stop):\n");
    char course[11];
    while (1) {
        printf("Enter course taken: ");
        scanf("%10s", course);
        if (strcmp(course, "Exit") == 0) {
            break;
        }
        fprintf(fp, "%s\n", course);
    }

    fprintf(fp, ":\nEAF\n");

    db->student_count++;
    fclose(fp);
    printf("Student information added successfully!\n");
}

void newFacultyInfo(databaseType *db) {
    if (db->faculty_count >= MAX_FACULTY) {
        fprintf(stderr, "ERROR: Maximum faculty capacity reached.\n");
        return;
    }

    FILE *fp = fopen(FACULTY_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", FACULTY_FILE);
        return;
    }

    char id[11], name[31], department[31];
    int deloading_units;

    printf("Enter faculty ID: ");
    scanf("%10s", id);
    getchar(); // Consume newline

    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline

    printf("Enter department: ");
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0';

    printf("Enter deloading units: ");
    scanf("%d", &deloading_units);

    // Writing faculty details to file
    fprintf(fp, "FACULTY %d\n%s %s %s %d\n", db->faculty_count + 1, name, id, department, deloading_units);

    db->faculty_count++;
    fclose(fp);
    printf("Faculty information added successfully!\n");
}


void addCourse(databaseType *db) {
    if (db->course_count >= MAX_COURSES) {
        fprintf(stderr, "ERROR: Maximum course capacity reached.\n");
        return;
    }

    FILE *fp = fopen(COURSES_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", COURSES_FILE);
        return;
    }

    char code[11], section[11], day[11], time[31], room[11], faculty[11];
    int units;

    printf("Enter course code: ");
    scanf("%10s", code);
    printf("Enter section: ");
    scanf("%10s", section);
    printf("Enter units: ");
    scanf("%d", &units);
    printf("Enter day: ");
    scanf("%10s", day);
    getchar();
    
    printf("Enter time: ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = '\0';

    printf("Enter room: ");
    scanf("%10s", room);
    printf("Enter faculty: ");
    scanf("%10s", faculty);

    // Writing course details to file
    fprintf(fp, "%s %s %d %s %s %s %s\n", code, section, units, day, time, room, faculty);
    db->course_count++;

    fclose(fp);
    printf("Course added successfully!\n");
}

void addPrerequisites(databaseType *db, char course_code[11]) {
    if (db->prereq_count >= MAX_PREREQUISITES) {
        fprintf(stderr, "ERROR: Maximum prerequisite capacity reached.\n");
        return;
    }

    FILE *fp = fopen(PREREQ_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", PREREQ_FILE);
        return;
    }

    printf("Enter prerequisites for %s (Type 'Exit' to stop):\n", course_code);
    char prereq_code[11];
    
    while (1) {
        printf("Enter prerequisite course code: ");
        scanf("%10s", prereq_code);
        if (strcmp(prereq_code, "Exit") == 0) {
            break;
        }
        fprintf(fp, "%s %s\n", prereq_code, course_code);
        db->prereq_count++;
    }

    fclose(fp);
    printf("Prerequisites added successfully!\n");
}

void newCoursePrereq(databaseType *db) {
    addCourse(db);

    char choice;
    printf("Does the course have prerequisites? (y/n): ");
    do {
        scanf(" %c", &choice);
    } while (choice != 'y' && choice != 'n');

    if (choice == 'y') {
        addPrerequisites(db, db->courses[db->course_count - 1].code);
    }
}

void addRoom(databaseType *db) {
    if (db->room_count >= MAX_ROOMS) {
        fprintf(stderr, "ERROR: Maximum room capacity reached.\n");
        return;
    }

    FILE *fp = fopen(ROOMS_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", ROOMS_FILE);
        return;
    }

    char room_number[11], day[11], time[31], course_code[11], section[11];
    int occupancy;

    printf("Enter room number: ");
    scanf("%10s", room_number);
    printf("Enter day: ");
    scanf("%10s", day);
    getchar(); // Consume newline

    printf("Enter time: ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = '\0'; // Remove newline

    printf("Enter course code: ");
    scanf("%10s", course_code);
    printf("Enter section: ");
    scanf("%10s", section);
    printf("Enter number of occupancy: ");
    scanf("%d", &occupancy);

    // Writing room details to file
    fprintf(fp, "%s %s %s %s %s %d\n", room_number, day, time, course_code, section, occupancy);
    db->room_count++;

    fclose(fp);
    printf("Room added successfully!\n");
}

void displayFacultyList() 
{
    FILE *fp = fopen(FACULTY_FILE, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for reading.\n", FACULTY_FILE);
        return;
    }
    
    printf("\nFaculty List:\n");
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}