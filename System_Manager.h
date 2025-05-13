#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <unordered_map>
#include <string>
#include "Admin.h"
#include "Student.h"

using namespace std;

class System_Manager {
    std::string username;
    std::string password;

public:
    std::unordered_map<std::string, Student> students;
    std::unordered_map<std::string, Admin> admins;
    std::unordered_map<std::string, Course> courses;

    System_Manager();

    void addCourse(const string &courseCode, const Course &course);

    void removeCourse(const string &courseCode);

    void editAdminPass(const string &id, const string &password);

    void editStudentPass(const string &id, const string &password);

    void showAvailableCourses() const;

    void registerCourse(string studentid,string courseid);

    void showEligibleCourses(const string &id);

    bool isStudentEligible(const string &id, const string &courseCode);

    /*void updateCourse(const string &courseID, const Course &updatedCourse);*/

    void addStudent(const string &studentID, const Student &student);

    void removeStudent(const string &studentID);

    Student &getStudent(const std::string &id); // Return reference

    Course &getCourse(const std::string &courseID); // Return reference

    void showCompletedCourses(const string &studentId);

    Admin getAdmin(const string &username);

    void readStudentsFromFile();

    void writeStudentsToFile();

    void readAdminsFromFile();

    void writeAdminsToFile();

    void readCoursesFromFile();

    void writeCoursesToFile();

    /*

    void setAdmins(const unordered_map<string, Admin> &newAdmins);
    void addAdmin(const string &username, const Admin &admin);
    void removeAdmin(const string &username);
    void setCourses(const unordered_map<string, Course> &newCourses);
    void setStudents(const unordered_map<string, Student> &newStudents);

    */
};

#endif //SYSTEM_MANAGER_H
