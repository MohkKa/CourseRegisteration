#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stack>
#include <vector>

// Forward declaration of Course
class Course;

using namespace std;

class System_Manager;

struct CourseState {
    string courseID;
    string title;
    string syllabus;
    int creditHour;
    string instructorName;
    string instructorEmail;
    std::vector<Course> prerequisites;

    CourseState(const Course& course);
};

class Admin {
    string id;
    string name;
    string username;
    string password;
    stack<string> addedCourses;
    stack<CourseState> courseHistory;

public:
    Admin();
    Admin(const string &id, const string &name, const string &username, const string &password);

    void addCourse(System_Manager &manager);
    void updateCourse(System_Manager &manager);
    void removeCourse(System_Manager &manager);
    bool UniqueID(const string &ID, char type, const System_Manager &);
    void undoLastAddedCourse(System_Manager &manager);
    void displayCoursesByCreditHours(System_Manager &manager);
    void addPrereq(System_Manager &manager);
    void removePrereq(System_Manager &manager);
    void addgrade(System_Manager &manager);
    void updategrade(System_Manager &manager);
    void addStudent(System_Manager &manager);
    void setPassword(const string &password);
    void undoLastUpdate(System_Manager &manager);

    string getUsername();
    string getPassword();
    string getId() { return this->id; }
    string getName() { return this->name; }

    void addAdmin(System_Manager &manager);
};

#endif // ADMIN_H
