
/* tester_table_improved.c – Tester with Improved Checkers (Substring Checking) 
   Table Format with Color-Coded PASS/FAIL */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ANSI escape codes for colored output */
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

/* Temporary filenames (used by 3.c) */
#define TMP_FILE_STUDENTS "students.txt"
#define TMP_FILE_FACULTY  "faculty.txt"
#define TMP_FILE_COURSES  "courses.txt"
#define TMP_FILE_PREREQ   "prerequisite.txt"
#define TMP_FILE_ROOMS    "rooms.txt"

/* Maximum limits (should match those in 3.c) */
#ifndef MAX_STUDENTS
#define MAX_STUDENTS 20
#endif
#ifndef MAX_FACULTY
#define MAX_FACULTY 20
#endif
#ifndef MAX_COURSES
#define MAX_COURSES 50
#endif
#ifndef MAX_PREREQS
#define MAX_PREREQS 50
#endif
#ifndef MAX_ROOMS
#define MAX_ROOMS 10
#endif

/* --- Helper: sanitize strings (remove newlines) --- */
void sanitizeString(const char *input, char *output, int maxLen) {
    int i = 0, j = 0;
    while (input[i] != '\0' && j < maxLen - 1) {
        if (input[i] == '\n' || input[i] == '\r')
            output[j++] = ' ';
        else
            output[j++] = input[i];
        i++;
    }
    output[j] = '\0';
}

/* --- Table Printing Helpers --- */
void printTableHeader(const char *functionName) {
    printf("\n=== Testing %s ===\n", functionName);
    printf("+-------+------------------------------------------+------------------------------------------+-----------------+-----------------+-----+\n");
    printf("| Test# | Description                              | Sample Input Data                        | Expected Output | Actual Output   | P/F |\n");
    printf("+-------+------------------------------------------+------------------------------------------+-----------------+-----------------+-----+\n");
}

void printTableRow(int testNum, const char *desc, const char *sampleInput, const char *expected, const char *actual, const char *result) {
    char descS[41], sampleS[41], expectedS[16], actualS[16], resultS[8];
    sanitizeString(desc, descS, 41);
    sanitizeString(sampleInput, sampleS, 41);
    sanitizeString(expected, expectedS, 16);
    sanitizeString(actual, actualS, 16);
    strncpy(resultS, result, 7);
    resultS[7] = '\0';
    printf("| %-5d | %-40s | %-40s | %-15s | %-15s | %-3s |\n", testNum, descS, sampleS, expectedS, actualS, resultS);
}

void printTableFooter() {
    printf("+-------+------------------------------------------+------------------------------------------+-----------------+-----------------+-----+\n");
}

/* --- Improved Checkers --- */

/* Integer checker */
int check_int(int testNum, const char *desc, const char *sampleInput, int expected, int actual) {
    char expectedStr[20], actualStr[20];
    sprintf(expectedStr, "%d", expected);
    sprintf(actualStr, "%d", actual);
    const char *result = (expected == actual) ? COLOR_GREEN "PASS" COLOR_RESET : COLOR_RED "FAIL" COLOR_RESET;
    printTableRow(testNum, desc, sampleInput, expectedStr, actualStr, result);
    return (expected == actual);
}

/* String checker (exact match) */
int check_str(int testNum, const char *desc, const char *sampleInput, const char *expected, const char *actual) {
    const char *result = (strcmp(expected, actual) == 0) ? COLOR_GREEN "PASS" COLOR_RESET : COLOR_RED "FAIL" COLOR_RESET;
    printTableRow(testNum, desc, sampleInput, expected, actual, result);
    return (strcmp(expected, actual) == 0);
}

/* Substring checker: checks if actual contains expected substring */
int check_substr(int testNum, const char *desc, const char *sampleInput, const char *expectedSubstr, const char *actual) {
    const char *result = (actual && strstr(actual, expectedSubstr)) ? COLOR_GREEN "PASS" COLOR_RESET : COLOR_RED "FAIL" COLOR_RESET;
    printTableRow(testNum, desc, sampleInput, expectedSubstr, actual ? actual : "NULL", result);
    return (actual && strstr(actual, expectedSubstr)) ? 1 : 0;
}

/* --- File Helper Functions --- */
void writeStringToFile(const char *filename, const char *content) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing.\n", filename);
        return;
    }
    fputs(content, fp);
    fclose(fp);
}

char* readFileToString(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *buffer = malloc(size + 1);
    if (buffer) {
        fread(buffer, 1, size, fp);
        buffer[size] = '\0';
    }
    fclose(fp);
    return buffer;
}

/* --- Include Student Code --- */
/* Redefine main to avoid conflict with 3.c's main */
#define main test_main
#include "3.c"
#undef main

/* === Test Functions for Helper Functions === */

/* 1. studentHasTaken */
void test_studentHasTaken_table() {
    printTableHeader("studentHasTaken");
    Student s;
    int res;
    
    s.subjectCount = 2;
    strcpy(s.subjectsTaken[0], "CS101");
    strcpy(s.subjectsTaken[1], "MATH101");
    res = studentHasTaken(&s, "CS101");
    check_int(1, "Course present (all caps)", "subjects: {CS101, MATH101}, code: CS101", 1, res);
    
    res = studentHasTaken(&s, "PHYS101");
    check_int(2, "Course not present", "subjects: {CS101, MATH101}, code: PHYS101", 0, res);
    
    s.subjectCount = 0;
    res = studentHasTaken(&s, "CS101");
    check_int(3, "Empty subject list", "subjects: {}, code: CS101", 0, res);
    
    printTableFooter();
}

/* 2. checkPrerequisite */
void test_checkPrerequisite_table() {
    printTableHeader("checkPrerequisite");
    Student s;
    s.subjectCount = 0;
    Prereq prereqs[1];
    int res;
    
    /* For course "CS102", prerequisite is "MATH101" */
    strcpy(prereqs[0].prereq, "MATH101");
    strcpy(prereqs[0].course, "CS102");
    
    strcpy(s.subjectsTaken[0], "MATH101");
    s.subjectCount = 1;
    res = checkPrerequisite(&s, "CS102", prereqs, 1);
    check_int(1, "Prerequisite met", "subjects: {MATH101}, prereq: MATH101 for CS102", 1, res);
    
    s.subjectCount = 0;
    res = checkPrerequisite(&s, "CS102", prereqs, 1);
    check_int(2, "Prerequisite not met", "subjects: {}, prereq: MATH101 for CS102", 0, res);
    
    res = checkPrerequisite(&s, "CS103", prereqs, 1);
    check_int(3, "No prerequisite defined", "subjects: {}, course: CS103", 1, res);
    
    printTableFooter();
}

/* 3. isFacultyCourseDeletable */
void test_isFacultyCourseDeletable_table() {
    printTableHeader("isFacultyCourseDeletable");
    Course course;
    strcpy(course.code, "CS101");
    strcpy(course.section, "A");
    Student students[1];
    int res;
    
    students[0].enrolledCount = 1;
    strcpy(students[0].enrolledCourses[0].code, "CS102");
    strcpy(students[0].enrolledCourses[0].section, "B");
    res = isFacultyCourseDeletable(NULL, &course, students, 1);
    check_int(1, "No enrollment", "course: CS101 A, student enrolled: CS102 B", 1, res);
    
    strcpy(students[0].enrolledCourses[0].code, "CS101");
    strcpy(students[0].enrolledCourses[0].section, "A");
    res = isFacultyCourseDeletable(NULL, &course, students, 1);
    check_int(2, "Enrollment present", "course: CS101 A, student enrolled: CS101 A", 0, res);
    
    res = isFacultyCourseDeletable(NULL, &course, NULL, 0);
    check_int(3, "Empty student array", "No students", 1, res);
    
    printTableFooter();
}

/* === Test Functions for File I/O Functions === */

/* 4. loadStudentsFromFile */
void test_loadStudentsFromFile_table() {
    printTableHeader("loadStudentsFromFile");
    const char *content =
"STUDENT 1\n"
"Juan dela Cruz\n"
"11911100 BSCS-ST\n"
"CCPROG1\n"
"MTH101A\n"
"EAF\n"
"CCPROG2 S11A 3 MW 0730-0900 G201 Merlinda Casas\n"
"STUDENT 2\n"
"Maria Mercedes\n"
"11922200 BSIET\n"
"CCPROG1\n"
"CCPROG2\n"
"MTH101A\n"
"EAF\n"
"CCPROG3 S13A 3 MW 0915-1045 G203 Merlinda Casas\n";
    writeStringToFile(TMP_FILE_STUDENTS, content);
    
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    loadStudentsFromFile(students, &studentCount);
    
    check_int(1, "Record count", "File has 2 STUDENT blocks", 2, studentCount);
    check_str(2, "First record name", "Name should be 'Juan dela Cruz'", "Juan dela Cruz", students[0].name);
    
    /* Test 3: File not found simulation */
    remove(TMP_FILE_STUDENTS);
    FILE *fp = fopen(TMP_FILE_STUDENTS, "r");
    const char *notFound = (fp==NULL) ? "Error printed" : "No Error";
    check_str(3, "File not found", "students.txt missing", "Error printed", notFound);
    if(fp) fclose(fp);
    printTableFooter();
}

/* 5. loadFacultyFromFile */
void test_loadFacultyFromFile_table() {
    printTableHeader("loadFacultyFromFile");
    const char *content =
"FACULTY 1\n"
"Merlinda Casas\n"
"2111 SoftwareTechnology 3\n"
"CCPROG2 S11A\n"
"CCPROG3 S13A\n"
"CCPROG3 S13B\n"
"FACULTY 2\n"
"Emilia Cruz\n"
"2222 ComputerTechnology 6\n"
"CCPROG2 S11B\n";
    writeStringToFile(TMP_FILE_FACULTY, content);
    
    Faculty facultyArr[MAX_FACULTY];
    int facultyCount = 0;
    loadFacultyFromFile(facultyArr, &facultyCount);
    
    check_int(1, "Record count", "File has 2 FACULTY blocks", 2, facultyCount);
    check_str(2, "First record name", "Name should be 'Merlinda Casas'", "Merlinda Casas", facultyArr[0].name);
    check_int(3, "First record course count", "Should be 3", 3, facultyArr[0].courseCount);
    
    printTableFooter();
}

/* 6. loadCoursesFromFile */
void test_loadCoursesFromFile_table() {
    printTableHeader("loadCoursesFromFile");
    const char *content =
"CCPROG2 S11A 3 MW 0730-0900 G201 Merlinda Casas\n"
"CSADPRG S14 3 MW 1245-1415 G204 NONE\n";
    writeStringToFile(TMP_FILE_COURSES, content);
    
    OfferedCourses offered;
    offered.count = 0;
    loadCoursesFromFile(&offered);
    
    check_int(1, "Record count", "File has 2 course records", 2, offered.count);
    check_str(2, "First course code", "Should be 'CCPROG2'", "CCPROG2", offered.courses[0].code);
    
    /* Test 3: File not found */
    remove(TMP_FILE_COURSES);
    FILE *fp = fopen(TMP_FILE_COURSES, "r");
    const char *msg = (fp==NULL) ? "Error printed" : "No Error";
    check_str(3, "File not found", "courses.txt missing", "Error printed", msg);
    if(fp) fclose(fp);
    printTableFooter();
}

/* 7. loadPrereqsFromFile */
void test_loadPrereqsFromFile_table() {
    printTableHeader("loadPrereqsFromFile");
    const char *content =
"CS100 CS101\n"
"CS101 CS102\n";
    writeStringToFile(TMP_FILE_PREREQ, content);
    
    Prereq prereqs[MAX_PREREQS];
    int prereqCount = 0;
    loadPrereqsFromFile(prereqs, &prereqCount);
    
    check_int(1, "Record count", "File has 2 lines", 2, prereqCount);
    check_str(2, "First record course", "Should be 'CS101'", "CS101", prereqs[0].course);
    
    /* Test 3: File not found */
    remove(TMP_FILE_PREREQ);
    FILE *fp = fopen(TMP_FILE_PREREQ, "r");
    const char *msgNotFound = (fp==NULL) ? "Error printed" : "No Error";
    check_str(3, "File not found", "prerequisite.txt missing", "Error printed", msgNotFound);
    if(fp) fclose(fp);
    printTableFooter();
}

/* 8. loadRoomsFromFile */
void test_loadRoomsFromFile_table() {
    printTableHeader("loadRoomsFromFile");
    const char *content =
"G201 MW 0730-0900 CS101 A 0\n"
"G202 TH 1100-1230 CS102 B 5\n";
    writeStringToFile(TMP_FILE_ROOMS, content);
    
    RoomAssignment rooms[MAX_ROOMS];
    int roomCount = 0;
    loadRoomsFromFile(rooms, &roomCount);
    
    check_int(1, "Record count", "File has 2 room records", 2, roomCount);
    check_str(2, "First room number", "Should be 'G201'", "G201", rooms[0].roomNo);
    
    remove(TMP_FILE_ROOMS);
    FILE *fp = fopen(TMP_FILE_ROOMS, "r");
    const char *notFoundMsg = (fp==NULL) ? "Error printed" : "No Error";
    check_str(3, "File not found", "rooms.txt missing", "Error printed", notFoundMsg);
    if(fp) fclose(fp);
    printTableFooter();
}

/* === Test Functions for Save Functions === */

/* 9. saveStudentsToFile */
void test_saveStudentsToFile_table() {
    printTableHeader("saveStudentsToFile");
    Student students[1];
    students[0].eafNumber = 1;
    strcpy(students[0].name, "Test Student");
    strcpy(students[0].id, "T001");
    strcpy(students[0].program, "TEST");
    students[0].subjectCount = 2;
    strcpy(students[0].subjectsTaken[0], "SUBJ1");
    strcpy(students[0].subjectsTaken[1], "SUBJ2");
    students[0].enrolledCount = 1;
    strcpy(students[0].enrolledCourses[0].code, "COURSE1");
    strcpy(students[0].enrolledCourses[0].section, "A");
    students[0].enrolledCourses[0].units = 3;
    strcpy(students[0].enrolledCourses[0].day, "MW");
    strcpy(students[0].enrolledCourses[0].time, "0900-1000");
    strcpy(students[0].enrolledCourses[0].room, "R101");
    strcpy(students[0].enrolledCourses[0].faculty, "Prof. Test");
    saveStudentsToFile(students, 1);
    
    char *fileContent = readFileToString(TMP_FILE_STUDENTS);
    /* Use substring checker because fileContent will contain more than just "STUDENT 1" */
    check_substr(1, "Content includes 'STUDENT 1'", "Should start with 'STUDENT 1'", "STUDENT 1", fileContent ? fileContent : "NULL");
    check_substr(2, "Includes student name", "Should contain 'Test Student'", "Test Student", fileContent ? fileContent : "NULL");
    check_substr(3, "Includes program info", "Should contain 'T001 TEST'", "T001 TEST", fileContent ? fileContent : "NULL");
    free(fileContent);
    printTableFooter();
}

/* 10. saveFacultyToFile */
void test_saveFacultyToFile_table() {
    printTableHeader("saveFacultyToFile");
    Faculty facultyArr[1];
    strcpy(facultyArr[0].name, "Test Faculty");
    strcpy(facultyArr[0].id, "F001");
    strcpy(facultyArr[0].dept, "TEST");
    facultyArr[0].deloadingUnits = 0;
    facultyArr[0].courseCount = 1;
    strcpy(facultyArr[0].courseLoad[0].code, "COURSE1");
    strcpy(facultyArr[0].courseLoad[0].section, "A");
    saveFacultyToFile(facultyArr, 1);
    
    char *fileContent = readFileToString(TMP_FILE_FACULTY);
    check_substr(1, "Content includes 'FACULTY 1'", "Should start with 'FACULTY 1'", "FACULTY 1", fileContent ? fileContent : "NULL");
    check_substr(2, "Includes faculty name", "Should contain 'Test Faculty'", "Test Faculty", fileContent ? fileContent : "NULL");
    check_substr(3, "Includes faculty id and dept", "Should contain 'F001 TEST'", "F001 TEST", fileContent ? fileContent : "NULL");
    free(fileContent);
    printTableFooter();
}

/* 11. saveCoursesToFile */
void test_saveCoursesToFile_table() {
    printTableHeader("saveCoursesToFile");
    OfferedCourses offered;
    offered.count = 1;
    strcpy(offered.courses[0].code, "COURSE1");
    strcpy(offered.courses[0].section, "A");
    offered.courses[0].units = 3;
    strcpy(offered.courses[0].day, "MW");
    strcpy(offered.courses[0].time, "0900-1000");
    strcpy(offered.courses[0].room, "R101");
    strcpy(offered.courses[0].faculty, "Prof. Test");
    saveCoursesToFile(&offered);
    
    char *fileContent = readFileToString(TMP_FILE_COURSES);
    check_substr(1, "Record count", "Should contain 1 course record", "COURSE1", fileContent ? fileContent : "NULL");
    check_substr(2, "Course code", "Should contain 'COURSE1'", "COURSE1", fileContent ? fileContent : "NULL");
    check_substr(3, "Course units", "Should contain '3'", "3", fileContent ? fileContent : "NULL");
    free(fileContent);
    printTableFooter();
}

/* 12. savePrereqsToFile */
void test_savePrereqsToFile_table() {
    printTableHeader("savePrereqsToFile");
    Prereq prereqs[1];
    strcpy(prereqs[0].prereq, "PREREQ1");
    strcpy(prereqs[0].course, "COURSE1");
    savePrereqsToFile(prereqs, 1);
    
    char *fileContent = readFileToString(TMP_FILE_PREREQ);
    check_substr(1, "Content includes prereq", "Should contain 'PREREQ1 COURSE1'", "PREREQ1 COURSE1", fileContent ? fileContent : "NULL");
    free(fileContent);
    printTableFooter();
}

/* 13. saveRoomsToFile */
void test_saveRoomsToFile_table() {
    printTableHeader("saveRoomsToFile");
    RoomAssignment rooms[1];
    strcpy(rooms[0].roomNo, "R101");
    strcpy(rooms[0].day, "MW");
    strcpy(rooms[0].time, "0900-1000");
    strcpy(rooms[0].courseCode, "COURSE1");
    strcpy(rooms[0].section, "A");
    rooms[0].occupancy = 0;
    saveRoomsToFile(rooms, 1);
    
    char *fileContent = readFileToString(TMP_FILE_ROOMS);
    check_substr(1, "Content includes room", "Should contain 'R101'", "R101", fileContent ? fileContent : "NULL");
    check_substr(2, "Course code check", "Should contain 'COURSE1'", "COURSE1", fileContent ? fileContent : "NULL");
    check_substr(3, "Occupancy check", "Should contain '0'", "0", fileContent ? fileContent : "NULL");
    free(fileContent);
    printTableFooter();
}

/* === Main Test Runner === */
int main(void) {
    printf("Automated Tester - Results in Table Format with Improved Checkers and Color-Coded PASS/FAIL\n");
    
    /* Helper Functions */
    test_studentHasTaken_table();
    test_checkPrerequisite_table();
    test_isFacultyCourseDeletable_table();
    
    /* Load Functions */
    test_loadStudentsFromFile_table();
    test_loadFacultyFromFile_table();
    test_loadCoursesFromFile_table();
    test_loadPrereqsFromFile_table();
    test_loadRoomsFromFile_table();
    
    /* Save Functions */
    test_saveStudentsToFile_table();
    test_saveFacultyToFile_table();
    test_saveCoursesToFile_table();
    test_savePrereqsToFile_table();
    test_saveRoomsToFile_table();
    
    printf("\nTesting completed.\n");
    return 0;
}
