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
    std::string password = "123";

    std::vector<CompletedCourse> completedCourses;

public:
    std::vector<Course> availableCourses;
    std::vector<Course> registeredCourses;
    std::string id;

    Student() = default;

    Student(const std::string &name, const std::string &id,
            const std::string &year, const std::string &email);
    Student(const std::string &name, const std::string &id,
           const std::string &year, const std::string &email,const std::string &password);

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
    double totalCompletedCreditHours(Student s);
    std::vector<Course> getRegisteredCourses() const { return registeredCourses; }
    std::vector<Course> getAvailableCourses() const { return availableCourses; }
    std::vector<CompletedCourse> getCompletedCourses() const { return completedCourses; }
    std::string getPassword() const { return password; }
    void setPassword(const std::string &newPassword) { password = newPassword; }

    bool FindCompletedCourse(const std::string &id) const;

    std::string getAcademicYear() {
        return this->year;
    }

    std::string getID() {
        return this->id;
    }
};

#endif // STUDENT_H
