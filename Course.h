#ifndef COURSE_H
#define COURSE_H
#include<stack>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
#include <string>
#include <vector>
#include <iomanip>

class System_Manager;

struct Instructor {
    std::string name;
    std::string email;
};

struct CourseDescription {
    std::string title;
    std::string syllabus;
    int creditHour;
    Instructor instructor;
};

struct CourseState {
    string ID;
    CourseDescription description;
};

class Course {
    std::string courseID;
    CourseDescription courseDescription;
    std::vector<Course> prerequisites;
    stack<CourseState> history;

public:
    Course();

    Course(std::string id, CourseDescription desc);

    std::string getCourseID() const;

    CourseDescription getDescription() const;

    std::string getTitle() const;

    std::string getSyllabus() const;

    int getCreditHour() const;

    Instructor getInstructor() const;

    std::string getInstructorName() const;

    std::string getInstructorEmail() const;

    const std::vector<Course> &getPrerequisites() const;

    void setCourseID(const std::string &id);

    void setDescription(const CourseDescription &desc);

    void setTitle(const std::string &t);

    void setSyllabus(const std::string &s);

    void setCreditHour(int ch);

    void setInstructor(const Instructor &inst);

    void setInstructorName(const std::string &name);

    void setInstructorEmail(const std::string &email);

    void addPrerequisite(const Course &course, const System_Manager &manager);

    void clearPrerequisites();
    void Backupdata(Course);

    void showCourseDescription() const;

    void showPrerequisites() const;
    Course undoupdate(Course);
    void displayAfterUndo(Course) const;
    void addCourse(Course course);
};

#endif // COURSE_H
