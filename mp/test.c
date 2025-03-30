
/* test_save.c – Checker for Save Functions in 3.c with Detailed Descriptions
   This tester verifies the save functions (saveStudentsToFile, saveFacultyToFile,
   saveCoursesToFile, savePrereqsToFile, and saveRoomsToFile) using three tests per function.
   Each test row includes a detailed description with sample input data.
   Output is printed in a table-like format with color-coded PASS (green) and FAIL (red) messages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ANSI color codes */
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

/* Temporary filenames (should match those used in 3.c) */
#define TMP_STUDENTS "students.txt"
#define TMP_FACULTY  "faculty.txt"
#define TMP_COURSES  "courses.txt"
#define TMP_PREREQ   "prerequisite.txt"
#define TMP_ROOMS    "rooms.txt"

/* --- Prototypes for File Helper Functions --- */
void writeStringToFile(const char *filename, const char *content);
char *readFileToString(const char *filename);

/* --- Table Printing Functions --- */
void printTableHeader(const char *funcName) {
    printf("\n=== Testing %s ===\n", funcName);
    printf("+-------+------------------------------------------------------+------------------------------+------------------------------+-----+\n");
    printf("| Test# | Description                                          | Expected                     | Actual                       | P/F |\n");
    printf("+-------+------------------------------------------------------+------------------------------+------------------------------+-----+\n");
}

void printTableRow(int testNum, const char *desc, const char *expected, const char *actual, const char *result) {
    printf("| %-5d | %-54s | %-28s | %-28s | %-3s |\n", testNum, desc, expected, actual, result);
}

void printTableFooter(void) {
    printf("+-------+------------------------------------------------------+------------------------------+------------------------------+-----+\n");
}

/* --- Checker for Substring Comparison ---
   Checks that 'actual' contains the substring 'expectedSub'. Returns 1 if found, 0 otherwise.
*/
int checkSubstr(int testNum, const char *desc, const char *expectedSub, const char *actual) {
    const char *result = (actual && strstr(actual, expectedSub)) ? GREEN "PASS" : RED "FAIL";
    printTableRow(testNum, desc, expectedSub, actual ? actual : "NULL", result);
    return (actual && strstr(actual, expectedSub)) ? 1 : 0;
}

/* --- File Helper Functions Implementation --- */
void writeStringToFile(const char *filename, const char *content) {
    FILE *fp = fopen(filename, "w");
    if(fp) {
        fputs(content, fp);
        fclose(fp);
    } else {
        printf("Error: Could not open %s for writing.\n", filename);
    }
}

char *readFileToString(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *buffer = malloc(size + 1);
    if(buffer) {
        fread(buffer, 1, size, fp);
        buffer[size] = '\0';
    }
    fclose(fp);
    return buffer;
}

/* --- Include 3.c --- */
/* Redefine main so that our tester's main() is used */
#define main test_main
#include "3.c"
#undef main

/* --- Test Functions for Save Functions --- */

/* Test saveStudentsToFile:
   Sample Input Data: A Student structure with:
      - Header: STUDENT 1
      - Name: "Test Student"
      - ID & Program: "T001 TEST"
      - Subjects: "SUBJ1", "SUBJ2"
      - Enrolled Course: "COURSE1 A 3 MW 0900-1000 R101 Prof. Test"
*/
void test_saveStudentsToFile() {
    printTableHeader("saveStudentsToFile");
    
    Student s[1];
    s[0].eafNumber = 1;
    strcpy(s[0].name, "Test Student");
    strcpy(s[0].id, "T001");
    strcpy(s[0].program, "TEST");
    s[0].subjectCount = 2;
    strcpy(s[0].subjectsTaken[0], "SUBJ1");
    strcpy(s[0].subjectsTaken[1], "SUBJ2");
    s[0].enrolledCount = 1;
    strcpy(s[0].enrolledCourses[0].code, "COURSE1");
    strcpy(s[0].enrolledCourses[0].section, "A");
    s[0].enrolledCourses[0].units = 3;
    strcpy(s[0].enrolledCourses[0].day, "MW");
    strcpy(s[0].enrolledCourses[0].time, "0900-1000");
    strcpy(s[0].enrolledCourses[0].room, "R101");
    strcpy(s[0].enrolledCourses[0].faculty, "Prof. Test");

    saveStudentsToFile(s, 1);
    char *content = readFileToString(TMP_STUDENTS);
    checkSubstr(1, "Header (Sample Input: 'STUDENT 1' from student record)", "STUDENT 1", content);
    checkSubstr(2, "Student name field (Sample Input: 'Test Student')", "Test Student", content);
    checkSubstr(3, "ID and Program (Sample Input: 'T001 TEST')", "T001 TEST", content);
    free(content);
    printTableFooter();
}

/* Test saveFacultyToFile:
   Sample Input Data: A Faculty structure with:
      - Header: FACULTY 1
      - Name: "Test Faculty"
      - ID & Department: "F001 TEST"
      - Course Load: "COURSE1 A"
*/
void test_saveFacultyToFile() {
    printTableHeader("saveFacultyToFile");
    
    Faculty f[1];
    strcpy(f[0].name, "Test Faculty");
    strcpy(f[0].id, "F001");
    strcpy(f[0].dept, "TEST");
    f[0].deloadingUnits = 0;
    f[0].courseCount = 1;
    strcpy(f[0].courseLoad[0].code, "COURSE1");
    strcpy(f[0].courseLoad[0].section, "A");

    saveFacultyToFile(f, 1);
    char *content = readFileToString(TMP_FACULTY);
    checkSubstr(1, "Header (Sample Input: 'FACULTY 1')", "FACULTY 1", content);
    checkSubstr(2, "Faculty name field (Sample Input: 'Test Faculty')", "Test Faculty", content);
    checkSubstr(3, "ID and Dept (Sample Input: 'F001 TEST')", "F001 TEST", content);
    free(content);
    printTableFooter();
}

/* Test saveCoursesToFile:
   Sample Input Data: An OfferedCourses structure with one course:
      - Course: "COURSE1 A 3 MW 0900-1000 R101 Prof. Test"
*/
void test_saveCoursesToFile() {
    printTableHeader("saveCoursesToFile");
    
    OfferedCourses off;
    off.count = 1;
    strcpy(off.courses[0].code, "COURSE1");
    strcpy(off.courses[0].section, "A");
    off.courses[0].units = 3;
    strcpy(off.courses[0].day, "MW");
    strcpy(off.courses[0].time, "0900-1000");
    strcpy(off.courses[0].room, "R101");
    strcpy(off.courses[0].faculty, "Prof. Test");

    saveCoursesToFile(&off);
    char *content = readFileToString(TMP_COURSES);
    checkSubstr(1, "Course code field (Sample Input: 'COURSE1')", "COURSE1", content);
    checkSubstr(2, "Time field (Sample Input: '0900-1000')", "0900-1000", content);
    checkSubstr(3, "Units field (Sample Input: '3')", "3", content);
    free(content);
    printTableFooter();
}

/* Test savePrereqsToFile:
   We perform three separate tests, each saving a single prerequisite record.
   Sample Input Data for each:
      Test 1: "PREREQ1 COURSE1"
      Test 2: "MATH101 MATH102"
      Test 3: "ENG101 ENG102"
*/
void test_savePrereqsToFile() {
    printTableHeader("savePrereqsToFile");
    char *content;
    
    /* Test 1 */
    {
        Prereq p[1];
        strcpy(p[0].prereq, "PREREQ1");
        strcpy(p[0].course, "COURSE1");
        savePrereqsToFile(p, 1);
        content = readFileToString(TMP_PREREQ);
        checkSubstr(1, "Single prereq record (Sample Input: 'PREREQ1 COURSE1')", "PREREQ1 COURSE1", content);
        free(content);
    }
    
    /* Test 2 */
    {
        Prereq p[1];
        strcpy(p[0].prereq, "MATH101");
        strcpy(p[0].course, "MATH102");
        savePrereqsToFile(p, 1);
        content = readFileToString(TMP_PREREQ);
        checkSubstr(2, "Single prereq record (Sample Input: 'MATH101 MATH102')", "MATH101 MATH102", content);
        free(content);
    }
    
    /* Test 3 */
    {
        Prereq p[1];
        strcpy(p[0].prereq, "ENG101");
        strcpy(p[0].course, "ENG102");
        savePrereqsToFile(p, 1);
        content = readFileToString(TMP_PREREQ);
        checkSubstr(3, "Single prereq record (Sample Input: 'ENG101 ENG102')", "ENG101 ENG102", content);
        free(content);
    }
    printTableFooter();
}

/* Test saveRoomsToFile:
   Sample Input Data: A RoomAssignment structure with:
      - Room: "R101"
      - Day/Time: "MW 0900-1000"
      - Course: "COURSE1", Section "A", Occupancy 0
*/
void test_saveRoomsToFile() {
    printTableHeader("saveRoomsToFile");
    
    RoomAssignment r[1];
    strcpy(r[0].roomNo, "R101");
    strcpy(r[0].day, "MW");
    strcpy(r[0].time, "0900-1000");
    strcpy(r[0].courseCode, "COURSE1");
    strcpy(r[0].section, "A");
    r[0].occupancy = 0;
    
    saveRoomsToFile(r, 1);
    char *content = readFileToString(TMP_ROOMS);
    checkSubstr(1, "Room number field (Sample Input: 'R101')", "R101", content);
    checkSubstr(2, "Course code field (Sample Input: 'COURSE1')", "COURSE1", content);
    checkSubstr(3, "Occupancy field (Sample Input: '0')", "0", content);
    free(content);
    printTableFooter();
}

/* --- Main Test Runner --- */
int main(void) {
    printf("Checker for Save Functions in 3.c\n");
    
    test_saveStudentsToFile();
    test_saveFacultyToFile();
    test_saveCoursesToFile();
    test_savePrereqsToFile();
    test_saveRoomsToFile();
    
    printf("\nTesting completed.\n");
    return 0;
}
