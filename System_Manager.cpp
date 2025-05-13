#include "System_Manager.h"
#include <iostream>
#include<sstream>
#include <fstream>
#include "Student.h"

System_Manager::System_Manager() = default;

void System_Manager::editAdminPass(const string &id, const string &password) {
    int i = 3;
    while (i--) {
        if (!admins.contains(id)) {
            cout << "This username does not exist." << endl;
            return;
        }

        string oldPassword;
        cout << "Enter your old password:" << endl;
        getline(cin, oldPassword);

        if (oldPassword.empty()) {
            cout << "Password cannot be empty. Try again." << endl;
            i++; // empty inputs are not counted as an attempt
            continue;
        }

        if (admins[id].getPassword() == oldPassword) {
            admins[id].setPassword(password);
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
        getline(cin, oldPassword);
        if (oldPassword.empty()) {
            cout << "Password cannot be empty. Try again." << endl;
            i++;
            continue;
        }

        if (students[id].getPassword() == oldPassword) {
            students[id].setPassword(password);
            cout << "Password successfully changed." << endl;
            return;
        }
        cout << "Password is incorrect." << endl;
    }

    cout << "Too many tries, try again later." << endl;
}
void System_Manager::registerCourse(string studentid,string courseid) {
    if (isStudentEligible(studentid,courseid)) {
        students[studentid].registeredCourses.push_back(getCourse(courseid));
        cout<<"registered successfully";
    }else {
        cout<<"You are not eligible for this course"<<endl;
        std::deque<Course> leftCourses;
        for (const Course &prereq: courses[courseid].getPrerequisites()) {
            bool foundPrereq = false;

            for (const auto &completed: students[studentid].getCompletedCourses()) {
                if (completed.course.getCourseID() == prereq.getCourseID()) {
                    foundPrereq = true;
                    break;
                }
            }

            if (!foundPrereq) {
                leftCourses.push_back(prereq);
            }
        }

        students[studentid].leftPrerequisites(leftCourses);
    }
}
/*cout<<"students registered courses";
for (Course course:student.getRegisteredCourses()) {
    cout<<course.getCourseID()<<endl;
}*/

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
    Course course = courses[courseCode];
    for (Course c : course.getPrerequisites()) {
        string prereqID = c.getCourseID();
          if (!student.FindCompletedCourse(prereqID)) {
            return false;
        }
    }

    return true;
}

/*
bool checkPrerequisite(const Course &course) const {
    std::vector<Course> prerequisites = course.getPrerequisites();

    for (const Course &prereq: prerequisites) {
        bool found = false;

        for (const auto &completed: completedCourses) {
            if (completed.course.getCourseID() == prereq.getCourseID()) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}
*/


void System_Manager::removeStudent(const string &studentID) {
    students.erase(studentID);
}

void System_Manager::addCourse(const string &courseCode, const Course &course) {
    courses[courseCode] = course;
}

/*void System_Manager::updateCourse(const string &courseID, const Course &updatedCourse) {
    if (courses.contains(courseID)) {
        courses[courseID] = updatedCourse;
    } else {
        cout << "This course is not found " << '\n';
    }

}*/

void System_Manager::addStudent(const string &studentID, const Student &student) {
    students[studentID] = student;
}

void System_Manager::removeCourse(const string &courseCode) {
    courses.erase(courseCode);
}

void System_Manager::showCompletedCourses(const string &studentId) {
    Student student = students[studentId];
    cout << "You completed the following courses: " << endl;
    for (CompletedCourse crs: student.getCompletedCourses()) {
        cout << "CourseID:  " << crs.course.getCourseID() << endl;
        cout << "Course Grade: " << crs.grade << endl;
    }
}

void System_Manager::readAdminsFromFile() {
    std::ifstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\admins.csv");
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
        } else {
            cout << "Skipping invalid entry: " << line << endl;
        }
    }

    file.close();
}

void System_Manager::writeAdminsToFile() {
    std::ofstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\admins.csv");
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
    std::ifstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\courses.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open courses file for reading.\n";
        return;
    }

    std::string line;
    unordered_map<string,string>prereqforlater;
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

        try {
            CourseDescription desc{title, syllabus, std::stoi(creditHourStr), {instructorName, instructorEmail}};
            Course course(id, desc);
            courses[id] = course;

            prereqforlater[id] = prereqStr;
        } catch (std::exception &e) {
            std::cerr << "Error parsing course line: " << line << "\n";
            std::cerr << "Reason: " << e.what() << "\n";
        }
    }

    file.close();  //close it now we read all the file


    //now lets assign all prerequisites to their courses
    for (auto &[id, pre]: prereqforlater) {
        if (pre.empty()) continue;
        std::stringstream prereqStream(pre);
        std::string prereqID;
        while (std::getline(prereqStream, prereqID, '-')) {
            if (courses.contains(prereqID)) {
                courses[id].addPrerequisite(courses.at(prereqID), *this);
            } else {
                std::cout << "Warning: Prerequisite course ID '" << prereqID
                          << "' not found for course '" << id << "'.\n";
            }
        }
    }

}

void System_Manager::writeCoursesToFile() {
    std::ofstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\courses.csv");

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

        std::vector<Course> prereqs = course.getPrerequisites();
        for (size_t i = 0; i < prereqs.size(); ++i) {
            file << prereqs[i].getCourseID();
            if (i < prereqs.size() - 1) {
                file << "-";
            }
        }
        file << "\n";
    }

    file.close();
}

void System_Manager::readStudentsFromFile() {
    std::ifstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\students.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open students file for reading.\n";
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, id, yearStr, email, password, completedCoursesStr, registeredCoursesStr;

        std::getline(ss, name, ',');
        std::getline(ss, id, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, email, ',');
        std::getline(ss, password, ',');
        std::getline(ss, completedCoursesStr, ',');
        std::getline(ss, registeredCoursesStr, ',');

        Student student(name, id, yearStr, email, password);

        if (!completedCoursesStr.empty()) {
            std::stringstream completedStream(completedCoursesStr);
            std::string courseData;

            while (std::getline(completedStream, courseData, '-')) {
                std::stringstream courseGradeStream(courseData);
                std::string courseId, semester, gradeStr;

                std::getline(courseGradeStream, courseId, ':');
                std::getline(courseGradeStream, semester, ':');
                std::getline(courseGradeStream, gradeStr);

                if (courses.contains(courseId)) {
                    Course CC = courses[courseId];
                    CompletedCourse C = {CC, semester, gradeStr};

                    try {
                        student.addCompletedCourse(C);
                    } catch (const std::invalid_argument &e) {
                        std::cerr << "Invalid grade: '" << gradeStr << "'\n";
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                } else {
                    std::cout << "Warning: Course ID '" << courseId << "' not found.\n";
                }
            }
        }

        if (!registeredCoursesStr.empty()) {
            std::stringstream regStream(registeredCoursesStr);
            std::string courseId;
            while (std::getline(regStream, courseId, '-')) {
                if (courses.contains(courseId)) {
                    student.registeredCourses.push_back(courses[courseId]);
                } else {
                    std::cout << "Warning: Registered course ID '" << courseId << "' not found.\n";
                }
            }
        }

        students[id] = student;
    }

    file.close();
}

void System_Manager::writeStudentsToFile() {
      std::ofstream file("C:\\Users\\Dell\\CourseRegisteration\\Data\\students.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open students file for writing.\n";
        return;
    }


    file << "name,id,year,email,password,completedCourses,registeredCourses\n";

    for (auto &[fst, snd]: students) {
        file << snd.getName() << ","
                << snd.getId() << ","
                << snd.getYear() << ","
                << snd.getEmail() << ","
                << snd.getPassword() << ",";


        std::vector<CompletedCourse> completedCourses = snd.getCompletedCourses();
        for (size_t i = 0; i < completedCourses.size(); ++i) {
            file << completedCourses[i].course.getCourseID() << ":" << completedCourses[i].semester << ':' << completedCourses[i].grade;
            if (i < completedCourses.size() - 1) {
                file << "-";
            }
        }
        file << ",";


        std::vector<Course>registeredCourses = snd.getRegisteredCourses();
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
