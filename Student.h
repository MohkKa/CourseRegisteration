#ifndef STUDENT_H
#define STUDENT_H

#include "Course.h"
#include <deque>

struct CompletedCourse {
    Course course;
    std::string semester;
    std::string grade;
};

class Student {
    std::string name;
    std::string year;
    std::string email;
    std::string password;

    std::vector<CompletedCourse> completedCourses;
    std::vector<Course> availableCourses;

public:
    std::vector<Course> registeredCourses;
    std::string id;

    Student() = default;

    Student(const std::string &name, const std::string &id,
            const std::string &year, const std::string &email);

    void viewGrade() const;

    static double convertGradeToGPA(const std::string &grade);

    double calcGPA() const;

    void showAvailableCourses() const;

    bool checkPrerequisite(const Course &course) const;

    static void leftPrerequisites(std::deque<Course> leftCourses);

    void registerCourse();

    std::string getName() const { return name; }
    std::string getId() const { return id; }
    std::string getYear() const { return year; }
    std::string getEmail() const { return email; }

    void addCompletedCourse(const CompletedCourse &course);

    void addAvailableCourse(const Course &course);

    void dropCourse(const std::string &courseID);

    void generateTranscript() const;

    std::vector<Course> getRegisteredCourses() const { return registeredCourses; }
    std::vector<Course> getAvailableCourses() const { return availableCourses; }
    std::vector<CompletedCourse> getCompletedCourses() const { return completedCourses; }
    std::string getPassword() const { return password; }
    void setPassword(const std::string &newPassword) { password = newPassword; }

    bool FindCompletedCourse(const std::string &id) const;
};

#endif // STUDENT_H
