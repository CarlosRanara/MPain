#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS (20)
#define MAX_FACULTY (20)
#define MAX_COURSES (20)
#define MAX_PREREQUISITES (20)
#define MAX_ROOMS (20)

#define STUDENT_FILE "students.txt"
#define FACULTY_FILE "faculty.txt"
#define COURSES_FILE "courses.txt"
#define PREREQ_FILE "prerequisite.txt"
#define ROOMS_FILE "rooms.txt"

#define MAX_UNITS (12)

typedef char String10[11];
typedef char String30[31];
typedef char String50[51];

typedef struct {
    String30 name;
    String10 id;
    String10 program;
    String10 enrolled_courses[MAX_COURSES];
    int enrolled_count;
} studentType;

typedef struct {
    String30 name;
    String10 id;
    String30 department;
    int deloading_units;
    String10 assigned_courses[MAX_COURSES];
} facultyType;

typedef struct {
    String10 code;
    String10 section;
    int units;
    String10 day;
    String10 time;
    String10 room;
    String10 assigned_faculty;
} courseType;

typedef struct {
    String10 prereq_code;
    String10 course_code;
} prerequisiteType;

typedef struct {
    String10 room_number;
    String10 day;
    String10 time;
    String10 course_code;
    String10 section;
    int occupancy;
} roomSchedType;

typedef struct {
    studentType students[MAX_STUDENTS];
    facultyType faculty[MAX_FACULTY];
    courseType courses[MAX_COURSES];
    prerequisiteType prerequisites[MAX_PREREQUISITES];
    roomSchedType rooms[MAX_ROOMS];
    int student_count;
    int faculty_count;
    int course_count;
    int prereq_count;
    int room_count;
} databaseType;

void newCoursePrereq(databaseType *db);
void displayFacultyList();

void addRoom(databaseType *db) {
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
    scanf("%10s", new_room.time);
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

void displayFacultyList() {
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

int main() {
    databaseType db = {0};
    int exit_main = 1;
    while (exit_main) {
        int choice;
        printf("\n1. Add Student\n2. Add Faculty\n3. Add Course & Prerequisite\n4. Add Room\n5. Display Faculty List\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            // newStudentInfo(&db); // Implement similar to faculty
        } else if (choice == 2) {
            // newFacultyInfo(&db);
        } else if (choice == 3) {
            //newCoursePrereq(&db);
        } else if (choice == 4) {
            //addRoom(&db);
        } else if (choice == 5) {
            displayFacultyList();
        } else if (choice == 6) {
            exit_main = 0;
        } else {
            printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
