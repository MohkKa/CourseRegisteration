#include "System_Manager.h"
#include <iostream>
#include <fstream>

System_Manager::System_Manager() = default;

void System_Manager::editAdminPass(const string &username, const string &password) {
    int i = 3;
    while (i--) {
        if (!admins.contains(username)) {
            cout << "This username does not exist." << endl;
            return;
        }

        string oldPassword;
        cout << "Enter your old password:" << endl;
        cin >> oldPassword;

        if (admins[username].getPassword() == oldPassword) {
            admins[username].setPassword(password);
            cout << "Password successfully changed." << endl;
            return;
        }
        cout << "Password is incorrect." << endl;
    }

    cout << "Too many tries, try again later." << endl;
}

void System_Manager::editStudentPass(const string &id, const string &password) {
    int i = 3;
    while (i--) {
        if (!students.contains(id)) {
            cout << "This ID does not exist." << endl;
            return;
        }
        string oldPassword;
        cout << "Enter your old password:" << endl;
        cin >> oldPassword;
        if (students[id].getPassword() == oldPassword) {
            students[id].setPassword(password);
            cout << "Password successfully changed." << endl;
            return;
        }
        cout << "Password is incorrect." << endl;
    }

    cout << "Too many tries, try again later." << endl;
}

Student System_Manager::getStudent(const string &id) {
    return students[id];
}

Admin System_Manager::getAdmin(const string &username) {
    return admins[username];
}

Course System_Manager::getCourse(const string &courseID) {
    return courses[courseID];
}

void System_Manager::showAvailableCourses() const {
    cout << "Available courses:" << endl;
    for (const auto &[fst, snd]: courses) {
        cout << "Course ID: " << snd.getCourseID() << endl;
        cout << "Title: " << snd.getTitle() << endl;
        cout << "Credit Hours: " << snd.getCreditHour() << endl;
        cout << "Instructor: " << snd.getInstructorName() << endl;
        cout << "Syllabus: " << snd.getSyllabus() << endl;
    }
}

void System_Manager::showEligibleCourses(const string &id) {
    if (!students.contains(id)) {
        cout << "Invalid entry." << endl;
    } else {
        stack<Course> eligibleCourses;

        for (const auto &[courseID, course]: courses) {
            bool eligible = true;

            for (const auto &prereq: course.getPrerequisites()) {
                bool found = false;

                if (string prereqID = prereq.getCourseID(); students[id].FindCompletedCourse(prereqID)) {
                    found = true;
                }

                if (!found) {
                    eligible = false;
                    break;
                }
            }

            if (eligible) {
                eligibleCourses.push(course);
            }
        }

        // Display eligible courses
        if (eligibleCourses.empty()) {
            std::cout << "No eligible courses found for " << students[id].getName() << ".\n";
            return;
        }

        cout << "Eligible courses for " << students[id].getName() << ":\n";
        while (!eligibleCourses.empty()) {
            eligibleCourses.top().showCourseDescription();
            eligibleCourses.pop();
        }
    }
}

bool System_Manager::isStudentEligible(const string &id, const string &courseCode) {
    if (!students.contains(id) || !courses.contains(courseCode)) {
        return false;
    }

    Student student = students[id];

    for (const Course &course = courses[courseCode]; const Course &prereq: course.getPrerequisites()) {
        if (string prereqID = prereq.getCourseID(); !student.FindCompletedCourse(prereqID)) {
            return false;
        }
    }

    return true;
}

void System_Manager::removeStudent(const string &studentID) {
    students.erase(studentID);
}

void System_Manager::addCourse(const string &courseCode, const Course &course) {
    courses[courseCode] = course;
}

void System_Manager::updateCourse(const string &courseID, const Course &updatedCourse) {
    if (const auto it = courses.find(courseID); it != courses.end()) {
        courses[courseID] = updatedCourse;
    } else {
        cout << "This course is not found " << '\n';
    }
}

void System_Manager::addStudent(const string &studentID, const Student &student) {
    students[studentID] = student;
}

void System_Manager::removeCourse(const string &courseCode) {
    courses.erase(courseCode);
}

/*

void System_Manager::setAdmins(const unordered_map<string, Admin> &newAdmins) {
    // Update existing admins or add new ones
    for (const auto &pair: newAdmins) {
        admins[pair.first] = pair.second;
    }

    // Remove any admins that are not in the newAdmins map
    for (auto it = admins.begin(); it != admins.end();) {
        if (newAdmins.find(it->first) == newAdmins.end()) {
            it = admins.erase(it);
        } else {
            ++it;
        }
    }
}

void System_Manager::addAdmin(const string &username, const Admin &admin) {
    admins[username] = admin;
}

void System_Manager::removeAdmin(const string &username) {
    admins.erase(username);
}

void System_Manager::setCourses(const unordered_map<string, Course> &newCourses) {
    // Update existing courses or add new ones
    for (const auto &pair: newCourses) {
        courses[pair.first] = pair.second;
    }

    // Remove any courses that are not in the newCourses map
    for (auto it = courses.begin(); it != courses.end();) {
        if (newCourses.find(it->first) == newCourses.end()) {
            it = courses.erase(it);
        } else {
            ++it;
        }
    }
}


void System_Manager::setStudents(const unordered_map<string, Student> &newStudents) {
    // Update existing students or add new ones
    for (const auto &pair: newStudents) {
        students[pair.first] = pair.second;
    }

    // Remove any students that are not in the newStudents map
    for (auto it = students.begin(); it != students.end();) {
        if (newStudents.find(it->first) == newStudents.end()) {
            it = students.erase(it);
        } else {
            ++it;
        }
    }
}

*/
