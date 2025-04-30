#include "System_Manager.h"
#include <iostream>
#include <fstream>
#include "Student.h"

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

Student &System_Manager::getStudent(const std::string &id) {
    return students[id]; // Return by reference
}

Course &System_Manager::getCourse(const std::string &courseID) {
    return courses[courseID]; // Return by reference
}

Admin System_Manager::getAdmin(const string &username) {
    return admins[username];
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
    if (courses.contains(courseID)) {
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

void System_Manager::showCompletedCourses(string studentId) {
    Student student=students[studentId];
    cout<<"You completed the following courses: "<<endl;
    for (CompletedCourse crs:student.getCompletedCourses()) {
        cout<<"CourseID:  " <<crs.course.getCourseID()<<endl;
        cout<<"Course Grade: "<<crs.grade<<endl;
    }
}

void System_Manager::readAdminsFromFile() {
    ifstream file("Data/admins.csv");
    if (!file.is_open()) {
        cerr << "Error opening admins.csv file." << endl;
        return;
    }

    admins.clear();

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, username, password;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');

        if (!id.empty() && !name.empty() && !username.empty() && !password.empty()) {
            admins.emplace(id, Admin(id, name, username, password));
        }
    }

    file.close();
}

void System_Manager::writeAdminsToFile() {
    ofstream file("Data/admins.csv");
    if (!file.is_open()) {
        cerr << "Error opening admins.csv file for writing." << endl;
        return;
    }

    for (auto &[fst, snd]: admins) {
        file << snd.getId() << ","
                << snd.getName() << ","
                << snd.getUsername() << ","
                << snd.getPassword() << "\n";
    }

    file.close();
}

void System_Manager::readCoursesFromFile() {
    std::ifstream file("Data/courses.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open courses file for reading.\n";
        return;
    }

    courses.clear();
    std::vector<std::pair<std::string, std::string> > pendingPrerequisites;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, syllabus, creditHourStr, instructorName, instructorEmail, prereqStr;

        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, syllabus, ',');
        std::getline(ss, creditHourStr, ',');
        std::getline(ss, instructorName, ',');
        std::getline(ss, instructorEmail, ',');
        std::getline(ss, prereqStr);

        CourseDescription desc{
            title,
            syllabus,
            std::stoi(creditHourStr),
            {instructorName, instructorEmail}
        };
        Course course(id, desc);

        if (!prereqStr.empty()) {
            pendingPrerequisites.emplace_back(id, prereqStr);
        }

        courses[id] = course;
    }

    for (const auto &[courseID, prereqStr]: pendingPrerequisites) {
        std::stringstream prereqStream(prereqStr);
        std::string prereqID;

        while (std::getline(prereqStream, prereqID, ',')) {
            if (courses.contains(prereqID)) {
                courses[courseID].addPrerequisite(courses.at(prereqID), *this);
            } else {
                std::cerr << "Prerequisite " << prereqID
                        << " not found for course " << courseID << "\n";
            }
        }
    }

    file.close();
}

void System_Manager::writeCoursesToFile() {
    std::ofstream file("Data/courses.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open courses file for writing.\n";
        return;
    }

    for (const auto &pair: courses) {
        const Course &course = pair.second;
        file << course.getCourseID() << ","
                << course.getTitle() << ","
                << course.getSyllabus() << ","
                << course.getCreditHour() << ","
                << course.getInstructorName() << ","
                << course.getInstructorEmail() << ",";

        const auto &prereqs = course.getPrerequisites();
        for (size_t i = 0; i < prereqs.size(); ++i) {
            file << prereqs[i].getCourseID();
            if (i < prereqs.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

void System_Manager::readStudentsFromFile() {
    std::ifstream file("Data/students.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open students file for reading.\n";
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, id, yearStr, email, gpaStr, completedCoursesStr, registeredCoursesStr, creditHoursStr,
                semester;

        std::getline(ss, name, ',');
        std::getline(ss, id, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, email, ',');
        std::getline(ss, completedCoursesStr, ',');
        std::getline(ss, registeredCoursesStr, ',');
        std::getline(ss, creditHoursStr, ',');
        getline(ss, semester);

        Student student(name, id, yearStr, email);

        if (!completedCoursesStr.empty()) {
            std::stringstream completedStream(completedCoursesStr);
            std::string courseData;
            while (std::getline(completedStream, courseData, '-')) {
                std::stringstream courseGradeStream(courseData); // Inside completed course parsing:

                std::string courseId, semesterS, gradeStr;
                std::getline(courseGradeStream, courseId, ':');
                std::getline(courseGradeStream, semester, ':');
                std::getline(courseGradeStream, gradeStr);

                if (courses.contains(courseId)) {
                    Course CC = courses[courseId]; // Use course's actual credit hour
                    CompletedCourse C = {CC, semesterS, gradeStr};
                    student.addCompletedCourse(C);
                }
            }
        }


        if (!registeredCoursesStr.empty()) {
            std::stringstream regStream(registeredCoursesStr);
            std::string courseId;
            while (std::getline(regStream, courseId, '-')) {
                if (courses.contains(courseId)) {
                    student.registeredCourses.push_back(courses[courseId]);
                }
            }
        }

        students[id] = student;
    }

    file.close();
}

void System_Manager::writeStudentsToFile() {
    std::ofstream file("Data/students.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open students file for writing.\n";
        return;
    }


    file << "name,id,year,email,gpa,completedCourses,registeredCourses\n";
    for (auto &[fst, snd]: students) {
        file << snd.getName() << ","
                << snd.getID() << ","
                << snd.getAcademicYear() << ","
                << snd.getEmail() << ",";

        const auto &completedCourses = snd.getCompletedCourses();
        for (size_t i = 0; i < completedCourses.size(); ++i) {
            // Inside the completed courses loop:
            file << completedCourses[i].course.getCourseID()
                    << ":" << completedCourses[i].semester
                    << ":" << completedCourses[i].grade; // Add grade
            if (i < completedCourses.size() - 1) {
                file << "-";
            }
        }
        file << ",";


        const auto &registeredCourses = snd.getRegisteredCourses();
        for (size_t i = 0; i < registeredCourses.size(); ++i) {
            file << registeredCourses[i].getCourseID();
            if (i < registeredCourses.size() - 1) {
                file << "-";
            }
        }

        file << "\n";
    }

    file.close();
}
