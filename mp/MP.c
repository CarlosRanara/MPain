/***********************************************************************************
This is to certify that this project is my own work, based on my personal efforts in 
studying and applying the concepts learned. I have constructed the functions and 
their respective algorithms and corresponding code by myself. The program was run, 
tested, and debugged by my own efforts. I further certify that I have not copied in part 
or whole or otherwise plagiarized the work of other students and/or persons.
    Ramil Carlos Ranara, DLSU ID# 12415758
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Definitions --- */
#define MAX_STUDENTS 20
#define MAX_FACULTY 20
#define MAX_COURSES 50
#define MAX_PREREQS 50
#define MAX_ROOMS 10
#define MAX_ENROLLED 10
#define MAX_SUBJECTS 10

/* --- Structure Declarations --- */
typedef struct {
    char code[10];
    char section[5];
    int units;
    char day[10];
    char time[15];
    char room[10];
    char faculty[50];
} Course;

typedef struct {
    char id[15];
    char name[50];
    char program[20];
    char subjectsTaken[MAX_SUBJECTS][10];  // subjects/courses already taken
    int subjectCount;
    Course enrolledCourses[MAX_ENROLLED];
    int enrolledCount;
    int eafNumber;  // Enrollment Approval Form number
} Student;

typedef struct {
    char id[15];
    char name[50];
    char dept[30];
    int deloadingUnits;  // Faculty-specified deloading (max total teaching units = 12 - deload)
    Course courseLoad[MAX_ENROLLED];
    int courseCount;
} Faculty;

typedef struct {
    Course courses[MAX_COURSES];
    int count;
} OfferedCourses;

typedef struct {
    char prereq[10];
    char course[10];   // The course that requires the prerequisite
} Prereq;

typedef struct {
    char roomNo[10];
    char day[10];
    char time[15];
    char courseCode[10];
    char section[5];
    int occupancy;
} RoomAssignment;

/* --- Function Prototypes --- */
void mainMenu(Student students[], int *studentCount, Faculty faculty[], int *facultyCount, 
              OfferedCourses *offered, Prereq prereqs[], int *prereqCount, 
              RoomAssignment rooms[], int *roomCount);
void studentMenu(Student students[], int studentCount, OfferedCourses *offered, Prereq prereqs[], int prereqCount);
void facultyMenu(Faculty faculty[], int facultyCount, OfferedCourses *offered, Student students[], int studentCount);
void academicAssistantMenu(Student students[], int *studentCount, Faculty faculty[], int *facultyCount, 
                           OfferedCourses *offered, Prereq prereqs[], int *prereqCount, 
                           RoomAssignment rooms[], int *roomCount);

int studentHasTaken(Student *student, char *courseCode);
int checkPrerequisite(Student *student, char *courseCode, Prereq prereqs[], int prereqCount);
int isFacultyCourseDeletable(Faculty *fac, Course *course, Student students[], int studentCount);

void saveStudentsToFile(Student students[], int studentCount);
void saveFacultyToFile(Faculty faculty[], int facultyCount);
void saveCoursesToFile(OfferedCourses *offered);
void savePrereqsToFile(Prereq prereqs[], int prereqCount);
void saveRoomsToFile(RoomAssignment rooms[], int roomCount);

/* File-loading function prototypes */
void loadStudentsFromFile(Student students[], int *studentCount);
void loadFacultyFromFile(Faculty faculty[], int *facultyCount);
void loadCoursesFromFile(OfferedCourses *offered);
void loadPrereqsFromFile(Prereq prereqs[], int *prereqCount);
void loadRoomsFromFile(RoomAssignment rooms[], int *roomCount);

/* --- Main Function --- */
/* main: Loads data and launches the main menu.
   @return 0 upon successful termination
*/
int main() {
    Student students[MAX_STUDENTS];
    Faculty faculty[MAX_FACULTY];
    OfferedCourses offered;
    offered.count = 0;
    Prereq prereqs[MAX_PREREQS];
    int prereqCount = 0;
    RoomAssignment rooms[MAX_ROOMS];
    int roomCount = 0;
    int studentCount = 0;
    int facultyCount = 0;

    /* Load initial data from files */
    loadStudentsFromFile(students, &studentCount);
    loadFacultyFromFile(faculty, &facultyCount);
    loadCoursesFromFile(&offered);
    loadPrereqsFromFile(prereqs, &prereqCount);
    loadRoomsFromFile(rooms, &roomCount);

    mainMenu(students, &studentCount, faculty, &facultyCount, &offered, prereqs, &prereqCount, rooms, &roomCount);
    return 0;
}

/* --- Updated File-Loading Functions --- */

/* loadStudentsFromFile loads student data from "students.txt".
   @param students     - array of Student to be filled
   @param studentCount - pointer to an integer to store the number of students loaded
   Pre-condition: If file is missing, an error is printed and studentCount remains unchanged
*/
void loadStudentsFromFile(Student students[], int *studentCount) {
     int count = 0, dummy;
 char header[20];
 char temp[50];
 FILE *fp = fopen("students.txt", "r");
 if (fp == NULL) {
 printf("Error: students.txt not found.\n");
 } else {
 while (fscanf(fp, "%s", header) != EOF && count < MAX_STUDENTS) {
 if (strcmp(header, "STUDENT") == 0) {
 fscanf(fp, "%d", &dummy); // Skip student number
 fscanf(fp, " %[^\n]", students[count].name); // Read full name
 fscanf(fp, "%s %s", students[count].id, students[count].program); //
Read ID and program
 /* Read subjects taken until marker "EAF" */
 students[count].subjectCount = 0;
 while (fscanf(fp, "%s", temp) != EOF && strcmp(temp, "EAF") != 0) {
 strcpy(students[count].subjectsTaken[students[count].subjectCount],
temp);
 students[count].subjectCount++;
 }
 /* Read enrolled course information */
 if (fscanf(fp, "%s", temp) == EOF) break;
 if (strcmp(temp, "NONE") == 0) {
 students[count].enrolledCount = 0;
 } else {
 students[count].enrolledCount = 1;
 strcpy(students[count].enrolledCourses[0].code, temp);
 fscanf(fp, "%s", students[count].enrolledCourses[0].section);
 fscanf(fp, "%d", &students[count].enrolledCourses[0].units);
 fscanf(fp, "%s", students[count].enrolledCourses[0].day);
 fscanf(fp, "%s", students[count].enrolledCourses[0].time);
 fscanf(fp, "%s", students[count].enrolledCourses[0].room);
 fscanf(fp, " %[^\n]", students[count].enrolledCourses[0].faculty);
 }
 students[count].eafNumber = count + 1;
 count++;
 }
 }
 *studentCount = count;
 fclose(fp);
 }
}

/* loadFacultyFromFile loads faculty data from "faculty.txt".
   @param faculty      - array of Faculty to be filled
   @param facultyCount - pointer to an integer to store the number of faculty loaded
   Pre-condition: If file is missing, an error is printed and facultyCount remains unchanged
*/
void loadFacultyFromFile(Faculty faculty[], int *facultyCount) {

    int count = 0;
    char header[20], token[50];
    int dummy;
    

    FILE *fp = fopen("faculty.txt", "r");
    if(fp == NULL) {
        printf("Error: faculty.txt not found.\n");
    } else {
        while(fscanf(fp, "%s", header) != EOF && count < MAX_FACULTY) {
        if(strcmp(header, "FACULTY") == 0) {
            fscanf(fp, "%d", &dummy);  // Skip faculty number
            fscanf(fp, " %[^\n]", faculty[count].name); // Read full name
            fscanf(fp, "%s %s %d", faculty[count].id, faculty[count].dept, &faculty[count].deloadingUnits);
            faculty[count].courseCount = 0;
            
            int ret;
            int continueLoop = 1;  // flag for inner loop
            while(continueLoop) {
                long pos = ftell(fp);
                ret = fscanf(fp, "%s", token);
                if(ret == EOF) {
                    continueLoop = 0;
                } else if(strcmp(token, "FACULTY") == 0) {
                    fseek(fp, pos, SEEK_SET);
                    continueLoop = 0;
                } else if(strcmp(token, "NONE") == 0) {
                    continueLoop = 0;
                } else {
                    strcpy(faculty[count].courseLoad[faculty[count].courseCount].code, token);
                    fscanf(fp, "%s", faculty[count].courseLoad[faculty[count].courseCount].section);
                    faculty[count].courseCount++;
                }
            }
            count++;
        }
    }
    *facultyCount = count;
    fclose(fp);
    }
}

/* loadCoursesFromFile loads courses from "courses.txt".
   @param offered - pointer to OfferedCourses structure to be filled
   Pre-condition: If file is missing, an error is printed and offered->count is set to 0
*/
void loadCoursesFromFile(OfferedCourses *offered) {
    FILE *fp = fopen("courses.txt", "r");
    int count = 0;
    if (fp == NULL) {
        printf("Error: courses.txt not found.\n");
    } else {
    while(count < MAX_COURSES &&
          fscanf(fp, "%s %s %d %s %s %s", 
                 offered->courses[count].code,
                 offered->courses[count].section,
                 &offered->courses[count].units,
                 offered->courses[count].day,
                 offered->courses[count].time,
                 offered->courses[count].room) != EOF) {
         fscanf(fp, " %[^\n]", offered->courses[count].faculty);
         count++;
    }
    offered->count = count;
    fclose(fp);
    }
}

/* loadPrereqsFromFile loads prerequisite relations from "prerequisite.txt".
   @param prereqs     - array of Prereq to be filled
   @param prereqCount - pointer to an integer to store the number of prerequisites loaded
   Pre-condition: If file is missing, an error is printed and prereqCount remains unchanged
*/
void loadPrereqsFromFile(Prereq prereqs[], int *prereqCount) {

    int count = 0;

    FILE *fp = fopen("prerequisite.txt", "r");
    if (fp == NULL) {
        printf("Error: prerequisite.txt not found.\n");
    } else {
    while(count < MAX_PREREQS &&
          fscanf(fp, "%s %s", prereqs[count].prereq, prereqs[count].course) != EOF) {
        count++;
    }
    *prereqCount = count;
    fclose(fp);
    }
}

/* loadRoomsFromFile loads room assignment data from "rooms.txt".
   @param rooms     - array of RoomAssignment to be filled
   @param roomCount - pointer to an integer to store the number of room assignments loaded
   Pre-condition: If file is missing, an error is printed and roomCount remains unchanged
*/
void loadRoomsFromFile(RoomAssignment rooms[], int *roomCount) {

    int count = 0;

    FILE *fp = fopen("rooms.txt", "r");
    if (fp == NULL) {
        printf("Error: rooms.txt not found.\n");
    } else {
    while(count < MAX_ROOMS &&
          fscanf(fp, "%s %s %s %s %s %d",
                 rooms[count].roomNo,
                 rooms[count].day,
                 rooms[count].time,
                 rooms[count].courseCode,
                 rooms[count].section,
                 &rooms[count].occupancy) != EOF) {
        count++;
    }
    *roomCount = count;
    fclose(fp);
    }
}


/* mainMenu displays the main options for Student, Faculty, or Academic Assistant.
   @param students     - array of Student
   @param studentCount - pointer to number of students
   @param faculty      - array of Faculty
   @param facultyCount - pointer to number of faculty
   @param offered      - pointer to OfferedCourses structure
   @param prereqs      - array of Prereq
   @param prereqCount  - pointer to number of prerequisite relations
   @param rooms        - array of RoomAssignment
   @param roomCount    - pointer to number of room assignments
   Pre-condition: Data has been loaded from files (if available)
*/
void mainMenu(Student students[], int *studentCount, Faculty faculty[], int *facultyCount, 
              OfferedCourses *offered, Prereq prereqs[], int *prereqCount, 
              RoomAssignment rooms[], int *roomCount) {
    int choice;
    do {
        printf("\nMain Menu\n");
        printf("1] Student\n");
        printf("2] Faculty\n");
        printf("3] Academic Assistant\n");
        printf("4] Exit\n");
        printf("Answer: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1:
                studentMenu(students, *studentCount, offered, prereqs, *prereqCount);
                break;
            case 2:
                facultyMenu(faculty, *facultyCount, offered, students, *studentCount);
                break;
            case 3:
                academicAssistantMenu(students, studentCount, faculty, facultyCount, offered, prereqs, prereqCount, rooms, roomCount);
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while(choice != 4);

    /* Save all information to files before exit */
    saveStudentsToFile(students, *studentCount);
    saveFacultyToFile(faculty, *facultyCount);
    saveCoursesToFile(offered);
    savePrereqsToFile(prereqs, *prereqCount);
    saveRoomsToFile(rooms, *roomCount);
}

/* studentMenu handles student transactions: course enrollment and EAF printing.
   @param students     - array of Student
   @param studentCount - number of students
   @param offered      - pointer to OfferedCourses
   @param prereqs      - array of Prereq
   @param prereqCount  - number of prerequisites
   Pre-condition: Valid studentCount and data loaded
*/
void studentMenu(Student students[], int studentCount, OfferedCourses *offered, Prereq prereqs[], int prereqCount) {
    char id[15];
    printf("\nStudent Menu\nEnter Student ID: ");
    scanf("%s", id);
    getchar();

    int found = -1;
    int i = 0;
    while (i < studentCount && found == -1) {
        if (strcmp(students[i].id, id) == 0) {
            found = i;
        }
        i++;
    }

    if(found == -1) {
        printf("Student not found. Please contact the Academic Assistant.\n");
    } else {
        int choice;
        do {
            printf("\nStudent Transactions:\n");
            printf("1] Enroll Courses\n");
            printf("2] Print EAF\n");
            printf("3] Return to Main Menu\n");
            printf("Answer: ");
            scanf("%d", &choice);
            getchar();
            if(choice == 1) {
                int subChoice;
                printf("\nEnroll Courses:\n");
                printf("1] Add Courses\n");
                printf("2] Edit Courses\n");
                printf("3] Delete Courses\n");
                printf("Answer: ");
                scanf("%d", &subChoice);
                getchar();
                if(subChoice == 1) {
                    char courseCode[10], section[5];
                    printf("Available Courses:\n");
                    printf("Code   Section   Units   Day   Time   Room   Faculty\n");
                    for (i = 0; i < offered->count; i++) {
                        printf("%s   %s   %d   %s   %s   %s   %s\n", offered->courses[i].code, 
                               offered->courses[i].section, offered->courses[i].units,
                               offered->courses[i].day, offered->courses[i].time, 
                               offered->courses[i].room, offered->courses[i].faculty);
                    }
                    printf("Enter courses to enroll (type EXIT as Course Code to finish):\n");
                    printf("Course Code: ");
                    scanf("%s", courseCode);
                    getchar();
                    while(strcmp(courseCode, "EXIT") != 0) {
                        printf("Section: ");
                        scanf("%s", section);
                        getchar();
                        int foundCourse = -1;
                        int j = 0;
                        while(j < offered->count && foundCourse == -1) {
                            if ((strcmp(offered->courses[j].code, courseCode) == 0) && 
                                (strcmp(offered->courses[j].section, section) == 0))
                            {
                                foundCourse = j;
                            }
                            j++;
                        }
                        if(foundCourse == -1) {
                            printf("Course not found or incorrect section.\n");
                        } else {
                            if(checkPrerequisite(&students[found], courseCode, prereqs, prereqCount)) {
                                if(students[found].enrolledCount < MAX_ENROLLED) {
                                    students[found].enrolledCourses[students[found].enrolledCount] = offered->courses[foundCourse];
                                    students[found].enrolledCount++;
                                    printf("Enrolled: %s %s\n", courseCode, section);
                                } else {
                                    printf("Maximum enrolled courses reached.\n");
                                }
                            } else {
                                printf("%s: Prerequisite not met.\n", courseCode);
                            }
                        }
                        printf("Course Code: ");
                        scanf("%s", courseCode);
                        getchar();
                    }
                }
                else if(subChoice == 2) { /* Edit Courses */
                    if(students[found].enrolledCount == 0) {
                        printf("No enrolled courses to edit.\n");
                    } else {
                        printf("Enrolled Courses:\n");
                        for(i = 0; i < students[found].enrolledCount; i++) {
                            printf("%d] %s   %s\n", i + 1, students[found].enrolledCourses[i].code, 
                                   students[found].enrolledCourses[i].section);
                        }
                        int num;
                        printf("Enter the number of the course to edit: ");
                        scanf("%d", &num);
                        getchar();
                        if(num < 1 || num > students[found].enrolledCount) {
                            printf("Invalid selection.\n");
                        } else {
                            char newCode[10], newSection[5];
                            printf("Enter new Course Code: ");
                            scanf("%s", newCode);
                            getchar();
                            printf("Enter new Section: ");
                            scanf("%s", newSection);
                            getchar();
                            int foundNew = -1;
                            int k = 0;
                            while(k < offered->count && foundNew == -1) {
                                if ((strcmp(offered->courses[k].code, newCode) == 0) &&
                                    (strcmp(offered->courses[k].section, newSection) == 0))
                                {
                                    foundNew = k;
                                }
                                k++;
                            }
                            if(foundNew == -1) {
                                printf("Course not found or incorrect section.\n");
                            } else {
                                if(checkPrerequisite(&students[found], newCode, prereqs, prereqCount)) {
                                    students[found].enrolledCourses[num - 1] = offered->courses[foundNew];
                                    printf("Course updated to: %s %s\n", newCode, newSection);
                                } else {
                                    printf("Prerequisite not met for %s\n", newCode);
                                }
                            }
                        }
                    }
                }
                else if(subChoice == 3) { /* Delete Courses */
                    if(students[found].enrolledCount == 0) {
                        printf("No enrolled courses to delete.\n");
                    } else {
                        printf("Enrolled Courses:\n");
                        for(i = 0; i < students[found].enrolledCount; i++) {
                            printf("%d] %s   %s\n", i + 1, students[found].enrolledCourses[i].code, 
                                   students[found].enrolledCourses[i].section);
                        }
                        int num;
                        printf("Enter the number of the course to delete: ");
                        scanf("%d", &num);
                        getchar();
                        if(num < 1 || num > students[found].enrolledCount) {
                            printf("Invalid selection.\n");
                        } else {
                            int j;
                            for(j = num - 1; j < students[found].enrolledCount - 1; j++) {
                                students[found].enrolledCourses[j] = students[found].enrolledCourses[j + 1];
                            }
                            students[found].enrolledCount--;
                            printf("Course deleted.\n");
                        }
                    }
                }
            }
            else if(choice == 2) {
                /* Print Enrollment Approval Form (EAF) */
                printf("\n--- Enrollment Approval Form (EAF) ---\n");
                printf("Name: %s\n", students[found].name);
                printf("ID: %s\n", students[found].id);
                printf("Program: %s\n", students[found].program);
                printf("EAF No: %d\n", students[found].eafNumber);
                printf("\nEnrolled Courses:\n");
                printf("Code   Section   Units   Day   Time   Room   Faculty\n");
                for(i = 0; i < students[found].enrolledCount; i++) {
                    Course c = students[found].enrolledCourses[i];
                    printf("%s   %s   %d   %s   %s   %s   %s\n", c.code, c.section, c.units, c.day, c.time, c.room, c.faculty);
                }
            }
        } while(choice != 3);
    }
}

/* facultyMenu handles faculty options: setting deloading units, selecting course load, and printing lists.
   @param faculty      - array of Faculty
   @param facultyCount - number of faculty
   @param offered      - pointer to OfferedCourses
   @param students     - array of Student
   @param studentCount - number of students
   Pre-condition: Valid data loaded
*/
void facultyMenu(Faculty faculty[], int facultyCount, OfferedCourses *offered, Student students[], int studentCount) {
    char id[15];
    printf("\nFaculty Menu\nEnter Faculty ID: ");
    scanf("%s", id);
    getchar();
    int found = -1;
    int i = 0;
    while(i < facultyCount && found == -1) {
        if (strcmp(faculty[i].id, id) == 0) {
            found = i;
        }
        i++;
    }
    if(found == -1) {
        printf("Faculty not found. Please contact the Academic Assistant.\n");
    } else {
        int choice;
        do {
            printf("\nFaculty Options:\n");
            printf("1] Enter Faculty Deloading Units\n");
            printf("2] Select Course Load\n");
            printf("3] Print Course Load\n");
            printf("4] Print Student List per Subject\n");
            printf("5] Return to Main Menu\n");
            printf("Answer: ");
            scanf("%d", &choice);
            getchar();
            if(choice == 1) {
                printf("Enter Deloading Units: ");
                scanf("%d", &faculty[found].deloadingUnits);
                getchar();
                printf("You can teach up to %d units of courses.\n", 12 - faculty[found].deloadingUnits);
            }
            else if(choice == 2) {
                char courseCode[10], section[5];
                printf("Available Courses:\n");
                printf("Code   Section   Units   Day   Time   Room   Faculty\n");
                for(i = 0; i < offered->count; i++) {
                    printf("%s   %s   %d   %s   %s   %s   %s\n", offered->courses[i].code, offered->courses[i].section, 
                           offered->courses[i].units, offered->courses[i].day, offered->courses[i].time,
                           offered->courses[i].room, offered->courses[i].faculty);
                }
                printf("Enter courses for your load (type EXIT as Course Code to finish):\n");
                printf("Course Code: ");
                scanf("%s", courseCode);
                getchar();
                while(strcmp(courseCode, "EXIT") != 0) {
                    printf("Section: ");
                    scanf("%s", section);
                    getchar();
                    int foundCourse = -1;
                    int j = 0;
                    while(j < offered->count && foundCourse == -1) {
                        if ((strcmp(offered->courses[j].code, courseCode) == 0) &&
                            (strcmp(offered->courses[j].section, section) == 0))
                        {
                            foundCourse = j;
                        }
                        j++;
                    }
                    if(foundCourse == -1) {
                        printf("Course not found.\n");
                    }
                    else {
                        int currentUnits = 0;
                        int k;
                        for(k = 0; k < faculty[found].courseCount; k++) {
                            currentUnits += faculty[found].courseLoad[k].units;
                        }
                        if(currentUnits + offered->courses[foundCourse].units > (12 - faculty[found].deloadingUnits))
                            printf("Adding this course exceeds your allowed load.\n");
                        else {
                            faculty[found].courseLoad[faculty[found].courseCount] = offered->courses[foundCourse];
                            faculty[found].courseCount++;
                            printf("Added: %s   %s\n", courseCode, section);
                        }
                    }
                    printf("Course Code: ");
                    scanf("%s", courseCode);
                    getchar();
                }
            }
            else if(choice == 3) {
                printf("\nYour Course Load:\n");
                printf("Code   Section   Units   Day   Time   Room   Faculty\n");
                for(i = 0; i < faculty[found].courseCount; i++) {
                    Course c = faculty[found].courseLoad[i];
                    printf("%s   %s   %d   %s   %s   %s   %s\n", c.code, c.section, c.units, c.day, c.time, c.room, c.faculty);
                }
            }
            else if(choice == 4) {
                int j, k;
                for(i = 0; i < faculty[found].courseCount; i++) {
                    Course c = faculty[found].courseLoad[i];
                    printf("\nStudents enrolled in %s %s:\n", c.code, c.section);
                    int studentFound = 0;
                    for(j = 0; j < studentCount; j++) {
                        for(k = 0; k < students[j].enrolledCount; k++) {
                            if((strcmp(students[j].enrolledCourses[k].code, c.code) == 0) &&
                               (strcmp(students[j].enrolledCourses[k].section, c.section) == 0))
                            {
                                printf("%s (%s)\n", students[j].name, students[j].id);
                                studentFound = 1;
                            }
                        }
                    }
                    if(!studentFound)
                        printf("No students enrolled.\n");
                }
            }
        } while(choice != 5);
    }
}

/* academicAssistantMenu handles administrative tasks such as managing student/faculty records,
   courses, prerequisites, and room assignments.
   @param students     - array of Student
   @param studentCount - pointer to number of students
   @param faculty      - array of Faculty
   @param facultyCount - pointer to number of faculty
   @param offered      - pointer to OfferedCourses
   @param prereqs      - array of Prereq
   @param prereqCount  - pointer to number of prerequisites
   @param rooms        - array of RoomAssignment
   @param roomCount    - pointer to number of room assignments
   Pre-condition: Data loaded from files
*/
void academicAssistantMenu(Student students[], int *studentCount, Faculty faculty[], int *facultyCount, 
                           OfferedCourses *offered, Prereq prereqs[], int *prereqCount, 
                           RoomAssignment rooms[], int *roomCount) {
    int choice;
    do {
        printf("\nAcademic Assistant Menu:\n");
        printf("1] Enter student information\n");
        printf("2] Enter faculty information\n");
        printf("3] Enter courses and prerequisites\n");
        printf("4] Schedule and Room Assignment\n");
        printf("5] Display Faculty List\n");
        printf("6] Return to Main Menu\n");
        printf("Answer: ");
        scanf("%d", &choice);
        getchar();
        if(choice == 1) {
            int subChoice;
            printf("\nStudent Information:\n");
            printf("1] Add Student\n2] Edit Student\n3] Delete Student\n");
            printf("Answer: ");
            scanf("%d", &subChoice);
            getchar();
            if(subChoice == 1) {
                if(*studentCount >= MAX_STUDENTS)
                    printf("Maximum number of students reached.\n");
                else {
                    Student s;
                    printf("Enter Student ID: ");
                    scanf("%s", s.id);
                    getchar();
                    printf("Enter Name: ");
                    fgets(s.name, sizeof(s.name), stdin);
                    s.name[strcspn(s.name, "\n")] = '\0';
                    printf("Enter Program: ");
                    scanf("%s", s.program);
                    getchar();
                    s.subjectCount = 0;
                    s.enrolledCount = 0;
                    s.eafNumber = *studentCount + 1;
                    printf("Enter subjects taken (type EXIT to finish):\n");
                    char subj[10];
                    printf("Enter subject: ");
                    scanf("%s", subj);
                    getchar();
                    while(strcmp(subj, "EXIT") != 0 && s.subjectCount < MAX_SUBJECTS) {
                        strcpy(s.subjectsTaken[s.subjectCount], subj);
                        s.subjectCount++;
                        printf("Enter subject: ");
                       	scanf("%s", subj);
                        getchar();
                    }
                    students[*studentCount] = s;
                    (*studentCount)++;
                    printf("Student added.\n");
                }
            }
            else if(subChoice == 2) {
                char sid[15];
                printf("Enter Student ID to edit: ");
                scanf("%s", sid);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *studentCount && idx == -1) {
                    if(strcmp(students[i].id, sid) == 0) {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Student not found.\n");
                else {
                    printf("Editing Student %s\n", students[idx].name);
                    printf("Enter new Name: ");
                    fgets(students[idx].name, sizeof(students[idx].name), stdin);
                    students[idx].name[strcspn(students[idx].name, "\n")] = '\0';
                    printf("Enter new Program: ");
                    scanf("%s", students[idx].program);
                    getchar();
                    students[idx].subjectCount = 0;
                    printf("Enter subjects taken (type EXIT to finish):\n");
                    char subj[10];
                    printf("Enter subject: ");
                    scanf("%s", subj);
                    getchar();
                    while(strcmp(subj, "EXIT") != 0 && students[idx].subjectCount < MAX_SUBJECTS) {
                        strcpy(students[idx].subjectsTaken[students[idx].subjectCount], subj);
                        students[idx].subjectCount++;
                        printf("Enter subject: ");
                        scanf("%s", subj);
                        getchar();
                    }
                    printf("Student information updated.\n");
                }
            }
            else if(subChoice == 3) {
                char sid[15];
                printf("Enter Student ID to delete: ");
                scanf("%s", sid);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *studentCount && idx == -1) {
                    if(strcmp(students[i].id, sid) == 0) {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Student not found.\n");
                else {
                    int j;
                    for(j = idx; j < *studentCount - 1; j++) {
                        students[j] = students[j + 1];
                    }
                    (*studentCount)--;
                    printf("Student deleted.\n");
                }
            }
        }
        else if(choice == 2) {
            int subChoice;
            printf("\nFaculty Information:\n");
            printf("1] Add Faculty\n2] Edit Faculty\n3] Delete Faculty\n");
            printf("Answer: ");
            scanf("%d", &subChoice);
            getchar();
            if(subChoice == 1) {
                if(*facultyCount >= MAX_FACULTY)
                    printf("Maximum number of faculty reached.\n");
                else {
                    Faculty f;
                    printf("Enter Faculty ID: ");
                    scanf("%s", f.id);
                    getchar();
                    printf("Enter Name: ");
                    fgets(f.name, sizeof(f.name), stdin);
                    f.name[strcspn(f.name, "\n")] = '\0';
                    printf("Enter Department: ");
                    scanf("%s", f.dept);
                    getchar();
                    f.deloadingUnits = 0;
                    f.courseCount = 0;
                    faculty[*facultyCount] = f;
                    (*facultyCount)++;
                    printf("Faculty added.\n");
                }
            }
            else if(subChoice == 2) {
                char fid[15];
                printf("Enter Faculty ID to edit: ");
                scanf("%s", fid);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *facultyCount && idx == -1) {
                    if(strcmp(faculty[i].id, fid) == 0) {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Faculty not found.\n");
                else {
                    printf("Editing Faculty %s\n", faculty[idx].name);
                    printf("Enter new Name: ");
                    fgets(faculty[idx].name, sizeof(faculty[idx].name), stdin);
                    faculty[idx].name[strcspn(faculty[idx].name, "\n")] = '\0';
                    printf("Enter new Department: ");
                    scanf("%s", faculty[idx].dept);
                    getchar();
                    printf("Enter new Deloading Units: ");
                    scanf("%d", &faculty[idx].deloadingUnits);
                    getchar();
                    printf("Faculty information updated.\n");
                }
            }
            else if(subChoice == 3) {
                char fid[15];
                printf("Enter Faculty ID to delete: ");
                scanf("%s", fid);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *facultyCount && idx == -1) {
                    if(strcmp(faculty[i].id, fid) == 0) {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Faculty not found.\n");
                else {
                    int j;
                    for(j = idx; j < *facultyCount - 1; j++) {
                        faculty[j] = faculty[j + 1];
                    }
                    (*facultyCount)--;
                    printf("Faculty deleted.\n");
                }
            }
        }
        else if(choice == 3) {
            int subChoice;
            printf("\nCourses and Prerequisites:\n");
            printf("1] Add Course\n2] Edit Course\n3] Delete Course\n4] Add Prerequisite\n5] Delete Prerequisite\n");
            printf("Answer: ");
            scanf("%d", &subChoice);
            getchar();
            if(subChoice == 1) {
                if(offered->count >= MAX_COURSES)
                    printf("Maximum courses reached.\n");
                else {
                    Course c;
                    printf("Enter Course Code: ");
                    scanf("%s", c.code);
                    getchar();
                    printf("Enter Section: ");
                    scanf("%s", c.section);
                    getchar();
                    printf("Enter Units: ");
                    scanf("%d", &c.units);
                    getchar();
                    printf("Enter Day: ");
                    scanf("%s", c.day);
                    getchar();
                    printf("Enter Time: ");
                    scanf("%s", c.time);
                    getchar();
                    strcpy(c.room, "TBA");    /* Room to be assigned later */
                    strcpy(c.faculty, "TBA"); /* Faculty to be assigned later */
                    offered->courses[offered->count] = c;
                    offered->count++;
                    printf("Course added.\n");
                }
            }
            else if(subChoice == 2) {
                char code[10], section[5];
                printf("Enter Course Code to edit: ");
                scanf("%s", code);
                getchar();
                printf("Enter Section: ");
                scanf("%s", section);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < offered->count && idx == -1) {
                    if ((strcmp(offered->courses[i].code, code) == 0) && 
                        (strcmp(offered->courses[i].section, section) == 0))
                    {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Course not found.\n");
                else {
                    printf("Editing Course %s %s\n", offered->courses[idx].code, offered->courses[idx].section);
                    printf("Enter new Units: ");
                    scanf("%d", &offered->courses[idx].units);
                    getchar();
                    printf("Enter new Day: ");
                    scanf("%s", offered->courses[idx].day);
                    getchar();
                    printf("Enter new Time: ");
                    scanf("%s", offered->courses[idx].time);
                    getchar();
                    printf("Course updated.\n");
                }
            }
            else if(subChoice == 3) {
                char code[10], section[5];
                printf("Enter Course Code to delete: ");
                scanf("%s", code);
                getchar();
                printf("Enter Section: ");
                scanf("%s", section);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < offered->count && idx == -1) {
                    if ((strcmp(offered->courses[i].code, code) == 0) &&
                        (strcmp(offered->courses[i].section, section) == 0))
                    {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Course not found.\n");
                else {
                    int canDelete = 1;
                    int j, k;
                    for(j = 0; j < *studentCount && canDelete; j++) {
                        for(k = 0; k < students[j].enrolledCount; k++) {
                            if ((strcmp(students[j].enrolledCourses[k].code, code) == 0) &&
                                (strcmp(students[j].enrolledCourses[k].section, section) == 0))
                            {
                                canDelete = 0;
                            }
                        }
                    }
                    if(!canDelete)
                        printf("Cannot delete course; students are enrolled.\n");
                    else {
                        for(j = idx; j < offered->count - 1; j++) {
                            offered->courses[j] = offered->courses[j + 1];
                        }
                        offered->count--;
                        printf("Course deleted.\n");
                    }
                }
            }
            else if(subChoice == 4) {
                if(*prereqCount >= MAX_PREREQS)
                    printf("Maximum prerequisites reached.\n");
                else {
                    Prereq p;
                    printf("Enter prerequisite course code: ");
                    scanf("%s", p.prereq);
                    getchar();
                    printf("Enter course code that requires the prerequisite: ");
                    scanf("%s", p.course);
                    getchar();
                    prereqs[*prereqCount] = p;
                    (*prereqCount)++;
                    printf("Prerequisite added.\n");
                }
            }
            else if(subChoice == 5) {
                char pre[10], course[10];
                printf("Enter prerequisite course code to delete: ");
                scanf("%s", pre);
                getchar();
                printf("Enter course code: ");
                scanf("%s", course);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *prereqCount && idx == -1) {
                    if ((strcmp(prereqs[i].prereq, pre) == 0) && (strcmp(prereqs[i].course, course) == 0))
                    {
                        idx = i;
                    }
                    i++;
                }
                if(idx == -1)
                    printf("Prerequisite not found.\n");
                else {
                    for(i = idx; i < *prereqCount - 1; i++) {
                        prereqs[i] = prereqs[i + 1];
                    }
                    (*prereqCount)--;
                    printf("Prerequisite deleted.\n");
                }
            }
        }
        else if(choice == 4) {
            int subChoice;
            printf("\nRoom Assignment:\n");
            printf("1] Add Room Assignment\n2] Edit Room Assignment\n3] Delete Room Assignment\n");
            printf("Answer: ");
            scanf("%d", &subChoice);
            getchar();
            if(subChoice == 1) {
                if(*roomCount >= MAX_ROOMS)
                    printf("Maximum rooms reached.\n");
                else {
                    RoomAssignment r;
                    printf("Enter Room No.: ");
                    scanf("%s", r.roomNo);
                    getchar();
                    printf("Enter Day: ");
                    scanf("%s", r.day);
                    getchar();
                    printf("Enter Time: ");
                    scanf("%s", r.time);
                    getchar();
                    printf("Enter Course Code: ");
                    scanf("%s", r.courseCode);
                    getchar();
                    printf("Enter Section: ");
                    scanf("%s", r.section);
                    getchar();
                    printf("Enter Occupancy: ");
                    scanf("%d", &r.occupancy);
                    getchar();
                    rooms[*roomCount] = r;
                    (*roomCount)++;
                    printf("Room assignment added.\n");
                }
            }
            else if(subChoice == 2) {
                char roomNo[10];
                printf("Enter Room No. to edit: ");
                scanf("%s", roomNo);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *roomCount && idx == -1) {
                    if(strcmp(rooms[i].roomNo, roomNo) == 0)
                        idx = i;
                    i++;
                }
                if(idx == -1)
                    printf("Room assignment not found.\n");
                else {
                    printf("Editing Room %s\n", rooms[idx].roomNo);
                    printf("Enter new Day: ");
                    scanf("%s", rooms[idx].day);
                    getchar();
                    printf("Enter new Time: ");
                    scanf("%s", rooms[idx].time);
                    getchar();
                    printf("Enter new Course Code: ");
                    scanf("%s", rooms[idx].courseCode);
                    getchar();
                    printf("Enter new Section: ");
                    scanf("%s", rooms[idx].section);
                    getchar();
                    printf("Enter new Occupancy: ");
                    scanf("%d", &rooms[idx].occupancy);
                    getchar();
                    printf("Room assignment updated.\n");
                }
            }
            else if(subChoice == 3) {
                char roomNo[10];
                printf("Enter Room No. to delete: ");
                scanf("%s", roomNo);
                getchar();
                int idx = -1;
                int i = 0;
                while(i < *roomCount && idx == -1) {
                    if(strcmp(rooms[i].roomNo, roomNo) == 0)
                        idx = i;
                    i++;
                }
                if(idx == -1)
                    printf("Room assignment not found.\n");
                else {
                    int i;
                    for(i = idx; i < *roomCount - 1; i++) {
                        rooms[i] = rooms[i + 1];
                    }
                    (*roomCount)--;
                    printf("Room assignment deleted.\n");
                }
            }
        }
        else if(choice == 5) {
            int i;
            printf("\nFaculty List:\n");
            for(i = 0; i < *facultyCount; i++) {
                printf("%s   %s   %s   Deloading Units: %d\n", faculty[i].name, faculty[i].id, faculty[i].dept, faculty[i].deloadingUnits);
            }
        }
    } while(choice != 6);
}

/* --- Helper Functions --- */
/* studentHasTaken checks if the student has taken a specific course.
   @param student    - pointer to a Student
   @param courseCode - the course code to check
   @return 1 if the course is found in the student's subjectsTaken, 0 otherwise
   Pre-condition: student's subjectCount is valid
*/
int studentHasTaken(Student *student, char *courseCode) {
    int i, taken = 0;
    for (i = 0; i < student->subjectCount && !taken; i++) {
        if(strcmp(student->subjectsTaken[i], courseCode) == 0)
            taken = 1;
    }
    return taken;
}

/* checkPrerequisite verifies that the student has met the prerequisite for a given course.
   @param student     - pointer to a Student
   @param courseCode  - the target course code
   @param prereqs     - array of Prereq relations
   @param prereqCount - number of prerequisite records
   @return 1 if the student meets all prerequisites for the course (or if no prerequisite is required),
           0 if any required prerequisite is missing
   Pre-condition: studentHasTaken works correctly
*/
int checkPrerequisite(Student *student, char *courseCode, Prereq prereqs[], int prereqCount) {
    int i;
    for (i = 0; i < prereqCount; i++) {
        if (strcmp(prereqs[i].course, courseCode) == 0) {
            // Check if the student has taken the required prerequisite
            if (!studentHasTaken(student, prereqs[i].prereq))
                return 0;  /* missing the required prerequisite */
        }
    }
    return 1;
}

/* isFacultyCourseDeletable checks if the given course can be deleted by verifying that no student
   is enrolled in it.
   @param fac          - pointer to Faculty (not used in current logic)
   @param course       - pointer to the Course to check
   @param students     - array of Student
   @param studentCount - number of students
   @return 1 if no student is enrolled in the course, 0 otherwise
   Pre-condition: studentCount is accurate
*/

int isFacultyCourseDeletable(Faculty *fac, Course *course, Student students[], int studentCount) {
    int i, j;
    for (i = 0; i < studentCount; i++) {
        for (j = 0; j < students[i].enrolledCount; j++) {
            if((strcmp(students[i].enrolledCourses[j].code, course->code) == 0) &&
               (strcmp(students[i].enrolledCourses[j].section, course->section) == 0))
                return 0;
        }
    }
    return 1;
}

/* --- File Saving Functions --- */
/* saveStudentsToFile writes the student data to "students.txt".
   @param students     - array of Student
   @param studentCount - number of students to save
   Pre-condition: students array is populated
*/
void saveStudentsToFile(Student students[], int studentCount) {
    FILE *fp = fopen("students.txt", "w");
    int i, j;
    for (i = 0; i < studentCount; i++) {
        fprintf(fp, "STUDENT %d\n", i + 1);
        fprintf(fp, "%s\n", students[i].name);
        fprintf(fp, "%s %s\n", students[i].id, students[i].program);
        for (j = 0; j < students[i].subjectCount; j++)
            fprintf(fp, "%s\n", students[i].subjectsTaken[j]);
        fprintf(fp, "EAF\n");
        for (j = 0; j < students[i].enrolledCount; j++) {
            Course c = students[i].enrolledCourses[j];
            fprintf(fp, "%s %s %d %s %s %s %s\n", c.code, c.section, c.units, c.day, c.time, c.room, c.faculty);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/* saveFacultyToFile writes the faculty data to "faculty.txt".
   @param faculty      - array of Faculty
   @param facultyCount - number of faculty to save
   Pre-condition: faculty array is populated
*/
void saveFacultyToFile(Faculty faculty[], int facultyCount) {
    FILE *fp = fopen("faculty.txt", "w");
    int i, j;
    for (i = 0; i < facultyCount; i++) {
        fprintf(fp, "FACULTY %d\n", i + 1);
        fprintf(fp, "%s\n", faculty[i].name);
        fprintf(fp, "%s %s %d\n", faculty[i].id, faculty[i].dept, faculty[i].deloadingUnits);
        for (j = 0; j < faculty[i].courseCount; j++)
            fprintf(fp, "%s %s\n", faculty[i].courseLoad[j].code, faculty[i].courseLoad[j].section);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/* saveCoursesToFile writes the offered courses data to "courses.txt".
   @param offered - pointer to the OfferedCourses structure
   Pre-condition: offered->count is valid
*/
void saveCoursesToFile(OfferedCourses *offered) {
    FILE *fp = fopen("courses.txt", "w");
    int i;
    for (i = 0; i < offered->count; i++) {
        Course c = offered->courses[i];
        fprintf(fp, "%s %s %d %s %s %s %s\n", c.code, c.section, c.units, c.day, c.time, c.room, c.faculty);
    }
    fclose(fp);
}

/* savePrereqsToFile writes the prerequisite relations to "prerequisite.txt".
   @param prereqs     - array of Prereq structures
   @param prereqCount - number of prerequisite records
   Pre-condition: prereqCount is valid
*/
void savePrereqsToFile(Prereq prereqs[], int prereqCount) {
    FILE *fp = fopen("prerequisite.txt", "w");
    int i;
    for (i = 0; i < prereqCount; i++) {
        fprintf(fp, "%s %s\n", prereqs[i].prereq, prereqs[i].course);
    }
    fclose(fp);
}

/* saveRoomsToFile writes the room assignment data to "rooms.txt".
   @param rooms     - array of RoomAssignment structures
   @param roomCount - number of room assignments to save
   Pre-condition: roomCount is valid
*/
void saveRoomsToFile(RoomAssignment rooms[], int roomCount) {
    FILE *fp = fopen("rooms.txt", "w");
    int i;
    for (i = 0; i < roomCount; i++) {
        fprintf(fp, "%s %s %s %s %s %d\n", 
                rooms[i].roomNo, rooms[i].day, rooms[i].time, 
                rooms[i].courseCode, rooms[i].section, rooms[i].occupancy);
    }
    fclose(fp);
}

