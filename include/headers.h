/**
 * Macro definitions
 */

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


#define MAX_UNTIS (12)

/**
 * Type declarations
 */

typedef char String10[11];
typedef char String30[31];
typedef char String50[51];

/**
 * Structure declarations
 */

struct studentTag {
    String30 name;
    String10 id;
    String10 program;
    String10 prereq[MAX_PREREQUISITES];
    courseType enrolled_courses[MAX_COURSES];
    int enrolled_count;
};
typedef struct studentTag studentType;

struct facultyTag {
    String30 name;
    String10 id;
    String30 department;
    int deloading_units;
    String10 assigned_courses[MAX_COURSES];
};
typedef struct facultyTag facultyType;

struct courseTag {
    String10 code;
    int units;
    String10 assigned_faculty;
    roomSchedType room;
};
typedef struct courseTag courseType;

struct prerequisiteTag {
    String10 prereq_code;
    String10 course_code;
};
typedef struct prerequisiteTag prerequisiteType;

struct roomSchedTag {
    String10 room_number;
    String10 day;
    String10 time;
    String10 course_code;
    String10 section;
    int occupancy;
};
typedef struct roomSchedTag roomSchedType;

struct databaseTag {
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
};
typedef struct databaseTag databaseType;

/**
 * Function Declarations
 */

void mainMenu(databaseType *db, int *exit_flag);

void initData(databaseType *db);
void loadData(databaseType *db);
void saveData(databaseType *db);

void studentMenu(databaseType *db);
int searchStudent(databaseType *db, String10 key);
void studentAccountMenu(studentType student, int *exit_flag);
void studentEnroll(studentType *db);
void studentEAF(studentType *db);
void studentAddCouses(studentType *db);

void facultyMenu(databaseType *db);
int searchFaculty(databaseType *db, String10 key);
int facultyLogIn(databaseType *db);
int facultyLogIn(databaseType *db);

void assistantMenu(databaseType *db);