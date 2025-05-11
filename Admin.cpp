#include "Admin.h"
#include <iostream>
#include <limits>
#include <map>
#include "System_Manager.h"
#include <algorithm>

Admin::Admin() {
    id = name = username = password = "";
}

Admin::Admin(const string &id, const string &name, const string &username, const string &password) {
    this->id = id;
    this->name = name;
    this->username = username;
    this->password = password;
}

string Admin::getPassword() {
    return password;
}

void Admin::setPassword(const string &password) {
    this->password = password;
}

string Admin::getUsername() {
    return username;
}

bool Admin::UniqueID(const string &ID, const char type, const System_Manager &manager) {
    if (type == 'C') {
        if (!manager.courses.contains(ID)) {
            return true;
        }
        return false;
    }

    if (type == 'S') {
        if (!manager.students.contains(ID)) {
            return true;
        }
        return false;
    }

    return false;
}

void Admin::addCourse(System_Manager &manager) {
    cout << "Please enter a detailed course description of the following :" << "\n";
    cout << "1. Course id " << "\n" << "2. Title " << "\n" << "3.Syllabus" << "\n" << "4.Credit hours" << "\n" <<
            "5.Instructor Name" << "\n" << "6.Instructor Email " << "\n";
    string courseID, title, syllabus, instructorName, instructorEmail;
    int creditHour;

    cout << "Enter unique course ID:";
    getline(cin, courseID);

    while (!UniqueID(courseID, 'C', manager)) {
        cout << "This ID is already taken. Please Try Again: ";
        cin >> courseID;
    }

    while (true) {
        try {
            cout << "Enter course title:";
            getline(cin, title);
            if (title.empty()) {
                /*throw runtime_error("Input cannot be empty");*/
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    while (true) {
        try {
            cout << "Enter syllabus:";
            getline(cin, syllabus);
            /*if (syllabus.empty()) {
                throw runtime_error("Input cannot be empty");
            }*/
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    cout << "Enter credit hours:";
    while (true) {
        try {
            if (!(cin >> creditHour)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a number.");
            }
            break;
        } catch (const runtime_error &e) {
            cout << e.what() << " Enter credit hours: ";
        }
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        try {
            cout << "Enter instructor name:";
            getline(cin, instructorName);
            if (instructorName.empty()) {
                /*throw runtime_error("Input cannot be empty");*/
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    while (true) {
        try {
            cout << "Enter instructor email: ";
            getline(cin, instructorEmail);
            if (instructorEmail.empty()) {
                /*throw runtime_error("Input cannot be empty");*/
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    Instructor I = {instructorName, instructorEmail};
    CourseDescription desc = {title, syllabus, creditHour, I};
    Course newCourse(courseID, desc);
    manager.addCourse(courseID, newCourse);

    string prereqID;
    cout << "Add prerequisites (course ID, 'done' to finish): ";
    while (true) {
        try {
            if (!(cin >> prereqID)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input");
            }

            if (prereqID == "done") break;

            if (!manager.courses.contains(prereqID)) {
                throw runtime_error("Course '" + prereqID + "' not found");
            }

            if (prereqID == courseID) {
                throw runtime_error("Cannot add self as prerequisite");
            }

            newCourse.addPrerequisite(manager.courses[prereqID], manager);
            cout << "Prerequisite added: " << prereqID << endl;
            cout << "Add another prerequisite or 'done' to finish: ";
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
            cout << "Add prerequisites (course ID, 'done' to finish): ";
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    manager.courses.insert(make_pair(courseID, newCourse));
    addedCourses.push(courseID);
    cout << "The course was added successfully.\n";
}

void Admin::updateCourse(System_Manager &manager) {
    // Get course ID with validation
    string courseID;
    while (true) {
        try {
            cout << "Enter course ID to update: ";
            if (!(cin >> courseID)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input");
            }
            if (!manager.courses.contains(courseID)) {
                throw runtime_error("Course not found");
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    Course &courseToUpdate = manager.getCourse(courseID);

    // Get menu choice with validation
    int choice;
    courseToUpdate.Backupdata(courseToUpdate);
    while (true) {
        try {
            cout << "What would you like to update:\n"
                    << "[1] courseID\n[2] title\n[3] syllabus\n"
                    << "[4] credit hour\n[5] instructor Name\n[6] instructor Email\n"
                    << "Enter choice: ";

            if (!(cin >> choice) || choice < 1 || choice > 6) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid choice (1-6 only)");
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    cin.ignore(); // Clear newline

    switch (choice) {
        case 1: {
            string newID;
            while (true) {
                try {
                    cout << "Enter new course ID: ";
                    if (!(cin >> newID)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Invalid input");
                    }
                    if (!UniqueID(newID, 'C', manager)) {
                        throw runtime_error("ID already taken");
                    }
                    courseToUpdate.setCourseID(newID);
                    cout << "Course ID updated successfully.\n";
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
        case 2: {
            string newTitle;
            while (true) {
                try {
                    cout << "Enter new title: ";
                    getline(cin, newTitle);
                    if (newTitle.empty()) {
                        throw runtime_error("Title cannot be empty");
                    }
                    courseToUpdate.setTitle(newTitle);
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
        case 3: {
            string newSyllabus;
            while (true) {
                try {
                    cout << "Enter new syllabus: ";
                    getline(cin, newSyllabus);
                    if (newSyllabus.empty()) {
                        throw runtime_error("Syllabus cannot be empty");
                    }
                    courseToUpdate.setSyllabus(newSyllabus);
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
        case 4: {
            int newCreditHour;
            while (true) {
                try {
                    cout << "Enter new credit hours: ";
                    if (!(cin >> newCreditHour)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Invalid number");
                    }
                    if (newCreditHour <= 0) {
                        throw runtime_error("Must be positive");
                    }
                    courseToUpdate.setCreditHour(newCreditHour);
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
        case 5: {
            string newInstName;
            while (true) {
                try {
                    cout << "Enter new instructor name: ";
                    getline(cin, newInstName);
                    if (newInstName.empty()) {
                        throw runtime_error("Name cannot be empty");
                    }
                    courseToUpdate.setInstructorName(newInstName);
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
        case 6: {
            string newInstEmail;
            while (true) {
                try {
                    cout << "Enter new instructor email: ";
                    getline(cin, newInstEmail);
                    if (newInstEmail.empty() || newInstEmail.find('@') == string::npos) {
                        throw runtime_error("Invalid email format");
                    }
                    courseToUpdate.setInstructorEmail(newInstEmail);
                    break;
                } catch (const runtime_error &e) {
                    cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
    }

    cout << "Course updated successfully\n";
}

void Admin::removeCourse(System_Manager &manager) {
    vector<string> courseIDs;
    cout << "Please Enter the course IDs you want to remove (type 'done' to finish):\n";

    string input;
    while (true) {
        try {
            if (!(cin >> input)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input");
            }

            if (input == "done") break;

            if (!manager.courses.contains(input)) {
                throw runtime_error("Course not found: " + input);
            }

            courseIDs.push_back(input);
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
            cout << "Enter course ID or 'done' to finish: ";
        }
    }

    char confirm;
    for (const string &id: courseIDs) {
        while (true) {
            try {
                cout << "Are you sure you want to delete course " << id << "? (y/n): ";
                cin >> confirm;

                if (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
                    throw runtime_error("Invalid choice. Please enter 'y' or 'n'.");
                }

                if (confirm == 'y' || confirm == 'Y') {
                    manager.courses.erase(id);
                    cout << "Course removed successfully.\n";
                } else {
                    cout << "Deletion cancelled.\n";
                }
                break;
            } catch (const runtime_error &e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }
}

void Admin::undoLastAddedCourse(System_Manager &manager) {
    if (addedCourses.empty()) {
        cout << "No Recently added courses to undo" << '\n';
        return;
    }

    const string &lastCourseID = addedCourses.top();
    addedCourses.pop();

    // Check if it still exists before erasing
    if (manager.courses.contains(lastCourseID)) {
        manager.courses.erase(lastCourseID);
        cout << "Undo successful. Course with ID " << lastCourseID << " has been removed." << '\n';
    } else {
        cout << "Course ID " << lastCourseID << " was already removed manually." << '\n';
    }
}

void Admin::displayCoursesByCreditHours(System_Manager &manager) {
    if (manager.courses.empty()) {
        cout << "No courses available ." << '\n';
    }

    map<int, vector<string> > coursesByCredit; // <hours, course IDs>

    for (const auto &[courseID , course]: manager.courses) {
        coursesByCredit[course.getCreditHour()].push_back(courseID);
    }

    for (const auto &[credit , courseIDs]: coursesByCredit) {
        cout << "=== " << credit << " Credit Hour Course(s) === " << '\n';
        for (const string &id: courseIDs) {
            cout << "- " << id << ": " << manager.courses[id].getTitle() << '\n';
        }
        cout << '\n';
    }
}

void Admin::addPrereq(System_Manager &manager) {
    string courseID;
    cout << "Enter course ID to add its prerequisites:\n";
    cin >> courseID;

    while (!manager.courses.contains(courseID)) {
        cout << "Error: Course ID not found. Please enter a valid course ID: ";
        cin >> courseID;
    }

    Course &up_course = manager.getCourse(courseID);

    if (!up_course.getPrerequisites().empty()) {
        cout << "Current prerequisites: ";
        for (const Course &c: up_course.getPrerequisites()) {
            cout << c.getTitle() << ", ";
        }
    }

    int x;
    cout << "Enter number of prerequisite courses required: ";
    while (true) {
        try {
            if (!(cin >> x)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input. Please enter a number.");
            }
            if (x < 0) {
                throw runtime_error("Number cannot be negative.");
            }
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << " Try again: ";
        }
    }

    for (int i = 0; i < x; ++i) {
        string id;
        while (true) {
            try {
                cout << "Enter course no " << i + 1 << " ID: ";
                cin >> id;

                if (!manager.courses.contains(id)) {
                    throw runtime_error("Course ID '" + id + "' not found.");
                }

                if (id == courseID) {
                    throw runtime_error("A course cannot be a prerequisite of itself.");
                }

                up_course.addPrerequisite(manager.courses[id], manager);
                break;
            } catch (const runtime_error &e) {
                cout << "Error: " << e.what() << " Please try again.\n";
            }
        }
    }

    cout << "Updated prerequisites list: ";
    for (const Course &c: up_course.getPrerequisites()) {
        cout << c.getTitle() << ", ";
    }
    cout << '\n';

    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

void Admin::removePrereq(System_Manager &manager) {
    string courseID;
    cout << "Enter course id: ";
    cin >> courseID;

    while (!manager.courses.contains(courseID)) {
        cout << "Course ID not found. Please try again: ";
        cin >> courseID;
    }

    Course &up_course = manager.getCourse(courseID);
    vector<Course> prereq_list = up_course.getPrerequisites();

    if (prereq_list.empty()) {
        cout << "This course doesn't have any prerequisites.\n";
        return;
    }

    cout << "Current prerequisites: ";
    for (const Course &c: prereq_list) {
        cout << c.getTitle() << ", ";
    }
    cout << endl;

    string courseTitle;
    cout << "Enter course title that you want to remove: ";
    cin.ignore();
    getline(cin, courseTitle);

    bool found = false;
    for (int i = 0; i < prereq_list.size(); i++) {
        if (prereq_list[i].getTitle() == courseTitle) {
            prereq_list.erase(prereq_list.begin() + i);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Course title not found in prerequisites.\n";
    } else {
        cout << "Updated prerequisites list: ";
        for (const Course &c: prereq_list) {
            cout << c.getTitle() << ", ";
        }
        cout << endl;
    }
}

void Admin::addgrade(System_Manager &manager) {
    string stud_id, course_id, grade, semester;

    cout << "Enter student ID: ";
    cin >> stud_id;
    while (!manager.students.contains(stud_id)) {
        cout << "The student ID is incorrect. Please re-enter: ";
        cin >> stud_id;
    }

    cout << "Enter course ID: ";
    cin >> course_id;
    while (!manager.courses.contains(course_id)) {
        cout << "The course ID is incorrect. Please re-enter: ";
        cin >> course_id;
    }

    auto &reg = manager.students[stud_id].registeredCourses;

    bool found = false;
    int index;
    for (int i = 0; i < reg.size(); i++) {
        if (reg[i].getCourseID() == course_id) found = true, index = i;
    }

    if (!found) {
        cout << "Course ID not found";
        return;
    }

    cout << "Enter course grade: ";
    cin >> grade;


    if (manager.students[stud_id].convertGradeToGPA(grade) == -3) {
        auto &courseToDrop = manager.students[stud_id].registeredCourses;
        for (int i = 0; i < reg.size(); i++) {
            if (reg[i].getCourseID() == course_id) courseToDrop.erase(courseToDrop.begin() + i);
        }
        cout << "Can't add course to completed cause of the grade and this course has been dropped ";
    }

    cout << "Enter course semester: ";
    cin >> semester;

    Student &student = manager.getStudent(stud_id);
    const Course &course = manager.getCourse(course_id);
    const CompletedCourse completed_course = {course, semester, grade};
    student.addCompletedCourse(completed_course);
    reg.erase(reg.begin() + index);

    auto &av = student.availableCourses;

    // Remove from available courses
    for (int i = 0; i < av.size(); ++i) {
        if (av[i].getCourseID() == course_id) {
            av.erase(av.begin() + i);
            break;
        }
    }

    cout << "Student grades:\n";
    for (const CompletedCourse &c: student.getCompletedCourses()) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }
}

void Admin::updategrade(System_Manager &manager) {
    string stud_id, course_id, new_grade;

    cout << "Enter student ID: ";
    cin >> stud_id;
    while (!manager.students.contains(stud_id)) {
        cout << "Student ID not found. Please re-enter: ";
        cin >> stud_id;
    }

    Student &student = manager.getStudent(stud_id);
    vector<CompletedCourse> courses = student.getCompletedCourses();

    if (courses.empty()) {
        cout << "No grades found for this student.\n";
        return;
    }

    cout << "Student grades:\n";
    for (const CompletedCourse &c: courses) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }

    cout << "Enter course ID to update: ";
    cin >> course_id;

    bool found = false;
    for (CompletedCourse &c: courses) {
        if (c.course.getCourseID() == course_id) {
            cout << "Enter new grade: ";
            cin >> new_grade;
            c.grade = new_grade;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "This course ID is not found in the student's completed courses.\n";
        return;
    }

    cout << "Updated student grades:\n";
    for (const CompletedCourse &c: courses) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }
}

void Admin::addStudent(System_Manager &manager) {
    string name, id, year, email;

    cin.ignore();

    while (true) {
        try {
            cout << "Enter student name: ";
            getline(cin, name);
            if (name.empty()) throw runtime_error("Input cannot be empty");
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    cout << "Enter student id: ";
    cin >> id;

    while (manager.students.contains(id)) {
        cout << "This ID is already taken. Please enter a different ID: ";
        cin >> id;
    }

    cout << "Enter student year: ";
    cin >> year;

    cout << "Enter student e-mail: ";
    while (true) {
        try {
            cin >> email;
            if (email.empty()) throw runtime_error("Input cannot be empty");
            break;
        } catch (const runtime_error &e) {
            cout << "Error: " << e.what() << ". Please try again: ";
        }
    }

    Student new_student(name, id, year, email);
    manager.addStudent(id, new_student);

    cout << "Student details: \n"
            << "Name: " << new_student.getName() << "\n"
            << "ID: " << new_student.getId() << "\n"
            << "Year : " << new_student.getYear() << "\n"
            << "E-mail: " << new_student.getEmail();
}

void Admin::addAdmin(System_Manager &manager) {
    string name, id, email, password;
    cout << "Enter Admin name: " << "\n";
    getline(cin, name);
    cout << "Enter Admin id: " << "\n";
    cin >> id;
    cout << "Enter Admin e-mail: " << "\n";
    cin >> email;
    cout << "Enter Admin password: " << "\n";
    cin >> password;

    Admin new_admin(id, name, email, password);
    manager.admins.insert(make_pair(id, new_admin));

    cout << "Admin details: " << "\n"
            << "ID: " << id << "\n"
            << "Name: " << name << "\n"
            << "Email : " << email << "\n"
            << "Password: " << password;
}
