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

void newStudentInfo(databaseType *db) 
{
    if (db->student_count >= MAX_STUDENTS) {
        fprintf(stderr, "ERROR: Maximum student capacity reached.\n");
        return;
    }
    
    studentType new_student;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", STUDENT_FILE);
        return;
    }
    
    printf("Enter student ID: ");
    scanf("%10s", new_student.id);
    printf("Enter name: ");
    getchar(); // To consume any leftover newline character
    fgets(new_student.name, sizeof(new_student.name), stdin);
    new_student.name[strcspn(new_student.name, "\n")] = '\0'; // Remove newline
    
    printf("Enter course: ");
    scanf("%10s", new_student.program);
    
    new_student.enrolled_count = 0;
    char temp_course[11];
    printf("Enter subjects taken (Type Exit to stop the entry)\n");
    while (1) {
        printf("Enter course taken: ");
        scanf("%10s", temp_course);
        if (strcmp(temp_course, "Exit") == 0) {
            break;
        }
        strcpy(new_student.enrolled_courses[new_student.enrolled_count], temp_course);
        new_student.enrolled_count++;
    }
    
    db->students[db->student_count] = new_student;
    db->student_count++;
    
    fprintf(fp, "STUDENT %d\n", db->student_count);
    fprintf(fp, "%s %s\n", new_student.name, new_student.id);
    
    for (int i = 0; i < new_student.enrolled_count; i++) {
        fprintf(fp, "%s\n", new_student.enrolled_courses[i]);
    }
    
    fprintf(fp, ":\nEAF\n");
 
    fclose(fp);
    printf("Student information added successfully!\n");
}

void newFacultyInfo(databaseType *db) 
{
    if (db->faculty_count >= MAX_FACULTY) {
        fprintf(stderr, "ERROR: Maximum faculty capacity reached.\n");
        return;
    }
    
    FILE *fp = fopen(FACULTY_FILE, "a");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", FACULTY_FILE);
        return;
    }
    
    facultyType new_faculty;
    printf("Enter faculty ID: ");
    scanf("%10s", new_faculty.id);
    printf("Enter name: ");
    getchar();
    fgets(new_faculty.name, sizeof(new_faculty.name), stdin);
    new_faculty.name[strcspn(new_faculty.name, "\n")] = '\0';
    printf("Enter department: ");
    scanf("%30s", new_faculty.department);
 
    
   
    db->faculty[db->faculty_count] = new_faculty;
    db->faculty_count++;
    
    fprintf(fp, "FACULTY %d\n", db->faculty_count);
    fprintf(fp, "%s %s %s\n", new_faculty.name, new_faculty.id, new_faculty.department);
     
    fclose(fp);
    printf("Faculty information added successfully!\n");
}

void addCourse(databaseType *db) 
{
    if (db->course_count >= MAX_COURSES) {
        fprintf(stderr, "ERROR: Maximum course capacity reached.\n");
        return;
    }
    
    FILE *course_fp = fopen(COURSES_FILE, "a");
    if (!course_fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", COURSES_FILE);
        return;
    }
    
    courseType new_course;
    printf("Enter course code: ");
    scanf("%10s", new_course.code);
    printf("Enter section: ");
    scanf("%10s", new_course.section);
    printf("Enter units: ");
    scanf("%d", &new_course.units);
    getchar();

    printf("Enter day: ");
    scanf("%10s", new_course.day);
    getchar();

    printf("Enter time: ");
    fgets(new_course.time, sizeof(new_course.time), stdin);
    new_course.time[strcspn(new_course.time, "\n")] = 0;

    printf("Enter room: ");
    fgets(new_course.room, sizeof(new_course.room), stdin);
    new_course.room[strcspn(new_course.room, "\n")] = 0;

    printf("Enter faculty: ");
    fgets(new_course.assigned_faculty, sizeof(new_course.assigned_faculty), stdin);
    new_course.assigned_faculty[strcspn(new_course.assigned_faculty, "\n")] = 0;

    db->courses[db->course_count] = new_course;
    db->course_count++;
    
    fprintf(course_fp, "%s %s %d %s %s %s %s\n", new_course.code, new_course.section, new_course.units, new_course.day, new_course.time, new_course.room, new_course.assigned_faculty);

    fclose(course_fp);
    printf("Course added successfully!\n");
}

void addPrerequisites(databaseType *db, String10 course_code) 
{
    FILE *prereq_fp = fopen(PREREQ_FILE, "a");
    if (!prereq_fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", PREREQ_FILE);
        return;
    }
    
    printf("Enter prerequisites (Type Exit to stop the entry)\n");
    char prereq_code[11];
    int hasPrereq = 0;
    while (db->prereq_count < MAX_PREREQUISITES) {
        printf("Enter prerequisite course code: ");
        scanf("%10s", prereq_code);
        if (strcmp(prereq_code, "Exit") == 0) {
            break;
        }
        if (strlen(prereq_code) == 0) {
            continue; // Skip empty input
        }
        fprintf(prereq_fp, "%s %s\n", prereq_code, course_code);
        db->prereq_count++;
        hasPrereq = 1;
    }
    
    fclose(prereq_fp);
    if (hasPrereq) {
        printf("Prerequisites added successfully!\n");
    } else {
        printf("No prerequisites entered.\n");
    }
}

void newCoursePrereq(databaseType *db) 
{
    char choice;
    addCourse(db);
    printf("Does the course have a prerequisite? (y/n): ");
    
    do {
        scanf(" %c", &choice);
    } while (choice != 'y' && choice != 'n');
    
    if (choice == 'y') {
        addPrerequisites(db, db->courses[db->course_count - 1].code);
    }
}

void addRoom(databaseType *db) 
{
    if (db->room_count >= MAX_ROOMS) {
        fprintf(stderr, "ERROR: Maximum room capacity reached.\n");
        return;
    }
    
    FILE *room_fp = fopen(ROOMS_FILE, "a");
    if (!room_fp) {
        fprintf(stderr, "ERROR: Could not open %s for writing.\n", ROOMS_FILE);
        return;
    }
    
    roomSchedType new_room;
    printf("Enter room number: ");
    scanf("%10s", new_room.room_number);
    printf("Enter day: ");
    scanf("%10s", new_room.day);
    printf("Enter time: ");
    scanf("%30s", new_room.time);
    printf("Enter course code: ");
    scanf("%10s", new_room.course_code);
    printf("Enter section: ");
    scanf("%10s", new_room.section);
    printf("Enter number of occupancy: ");
    scanf("%d", &new_room.occupancy);
    
    db->rooms[db->room_count] = new_room;
    db->room_count++;
    
    fprintf(room_fp, "%s %s %s %s %s %d\n", new_room.room_number, new_room.day, new_room.time, new_room.course_code, new_room.section, new_room.occupancy);
    
    fclose(room_fp);
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