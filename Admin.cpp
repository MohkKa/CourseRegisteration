#include "Admin.h"
#include <iostream>
#include <limits>
#include <map>
#include "System_Manager.h"

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
    cout << "Please Enter a Detailed course Description of the following :" << "\n";
    cout << "1. Course id " << "\n" << "2. Title " << "\n" << "3.Syllabus" << "\n" << "4.Credit hours" << "\n" <<
            "5.InstructorName" << "\n" << "6.Instructor Email " << "\n";
    string courseID, title, syllabus, instructorName, instructorEmail;
    int creditHour;

    cout << "Enter unique course ID: ";
    cin >> courseID;

    while (!UniqueID(courseID, 'C', manager)) {
        cout << "This ID is already taken. Please Try Again: ";
        cin >> courseID;
    }

    cin.ignore(); // Clear newline buffer
    cout << "Enter course title: ";
    getline(cin, title);

    cout << "Enter syllabus: ";
    getline(cin, syllabus);

    cout << "Enter credit hours: ";
    while (!(cin >> creditHour)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter credit hours: ";
    }

    cout << "Enter instructor name: ";
    getline(cin, instructorName);

    cout << "Enter instructor email: ";
    getline(cin, instructorEmail);


    Instructor I = {instructorName, instructorEmail};
    CourseDescription desc = {title, syllabus, creditHour, I};
    Course newCourse(courseID, desc);
         manager.addCourse(courseID, newCourse);

    string prereqID;
    cout << "Add prerequisites (course ID, 'done' to finish): ";
    while (cin >> prereqID && prereqID != "done") {
        if (manager.courses.contains(prereqID)) {
            newCourse.addPrerequisite(manager.courses[prereqID], manager);
            cout << "Prerequisite added: " << prereqID << endl;
        } else {
            cout << "Prerequisite course " << prereqID << " not found.\n";
        }
    }

    // After adding prerequisites:
    manager.courses[courseID] = newCourse; // Overwrite the course in the manager
    addedCourses.push(courseID);
    cout << "The course was added successfully. " << "\n";
}

void Admin::updateCourse(System_Manager &manager) {
    cout << "Please Enter the course ID you want to Update  " << "\n";
    string courseID;
    cin >> courseID;

    if (!manager.courses.contains(courseID)) {
        cout << "Course not found." << '\n';
        return;
    }

    Course courseToUpdate = manager.getCourse(courseID);

    cout << "What would you like to update " << "\n";
    cout << " [1] courseID " << "\n" << "[2] title " << "\n" << "[3] syllabus " << "\n" <<
            "[4] credit hour " << "\n" << "[5] instructor Name " << "\n" << "[6] instructor Email" << "\n";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1: {
            string newID;
            cout << "Enter new course ID: ";
            cin >> newID;
            if (UniqueID(newID, 'C', manager)) {
                courseToUpdate.setCourseID(newID);
                 manager.addCourse(newID ,courseToUpdate);
                manager.courses.erase(courseID);
                cout << "Course ID updated successfully." << "\n";
            } else {
                cout << "This Course ID is already taken " << "\n";
            }
            break;
        }
        case 2: {
            string newTitle;
            cout << "Enter new title: ";
            getline(cin, newTitle);
            courseToUpdate.setTitle(newTitle);
            break;
        }
        case 3: {
            string newSyllabus;
            cout << "Enter new syllabus: ";
            getline(cin, newSyllabus);
            courseToUpdate.setSyllabus(newSyllabus);
            break;
        }
        case 4: {
            int newCreditHour;
            cout << "Enter new credit hours: ";
            cin >> newCreditHour;
            courseToUpdate.setCreditHour(newCreditHour);
            break;
        }
        case 5: {
            string newInstName;
            cout << "Enter new instructor name: ";
            getline(cin, newInstName);
            courseToUpdate.setInstructorName(newInstName);
            break;
        }
        case 6: {
            string newInstEmail;
            cout << "Enter new instructor email: ";
            getline(cin, newInstEmail);
            courseToUpdate.setInstructorEmail(newInstEmail);
            break;
        }
        default:
            cout << "Invalid choice." << '\n';
            return;
    }

     manager.updateCourse(courseID, courseToUpdate);
    cout << "The course was updated successfully" << "\n";
}

void Admin::removeCourse(System_Manager &manager) {
    vector<string> courseIDs;
    cout << "Please Enter the course IDs you want to remove (type 'done' to finish):\n";

    string input;
    while (cin >> input && input != "done") {
        if (!manager.courses.contains(input))
            cout << "Course not found: " << input << '\n';
        else
            courseIDs.push_back(input);
    }

    char confirm;
    for (const string &id: courseIDs) {
        cout << "Are you sure you want to delete course " << id << "? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            manager.courses.erase(id);
            cout << "Course removed successfully.\n";
        } else {
            cout << "Deletion cancelled.\n";
        }
    }
}

void Admin::undoLastAddedCourse(System_Manager &manager) {
    if (addedCourses.empty()) {
        cout << "No  Recently added courses to undo" << '\n';
        return;
    }

    const string lastCourseID = addedCourses.top();
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

void Admin::addPrereq(System_Manager& manager) {
    string courseID;
    cout << "Enter course id to add its prerequisites: " << "\n";
    cin >> courseID;
    Course up_course = manager.getCourse(courseID);

    if (!up_course.getPrerequisites().empty()) {
        cout << "Current prerequisites:  ";
        for (Course c : up_course.getPrerequisites()) {
            cout << c.getTitle() << ", ";
        }
    }
    else {
        int x;
        cout << "Enter number of prerequisite courses required: ";
        cin >> x;
        for (int i = 0; i < x; i++) {
            string id;
            cout << "Enter course no " << i + 1 << " id:";
            cin >> id;
            up_course.addPrerequisite(manager.courses[id], manager);
        }
    }

    cout << "Updated prerequisites list: ";
    for (Course c : up_course.getPrerequisites()) {
        cout << c.getTitle() << ", ";
    }
}

void Admin::removePrereq(System_Manager& manager) {
    string courseID;
    cout << "Enter course id: " << "\n";
    cin >> courseID;

    Course up_course = manager.getCourse(courseID);
    vector<Course> prereq_list = up_course.getPrerequisites();


    if (!prereq_list.empty()) {
        cout << "Current prerequisites:  ";
        for (Course c : prereq_list) {
            cout << c.getTitle() << ", ";
        }
    }
    else {
        cout << "This course doesn't have any prerequisites.";
    }

    string courseTitle;
    cout << "Enter course title that you want to remove: ";
    cin >> courseTitle;
    int index;

    for (int i = 0; i < prereq_list.size(); i++) {
        if (prereq_list[i].getTitle() == courseTitle) {
            index = i;
            for (int j = index; j < prereq_list.size() - 1; j++) {
                prereq_list[j] = prereq_list[j + 1];
            }
            break;
        }
    }
    cout << "Updated prerequisites list: ";
    for (Course c : up_course.getPrerequisites()) {
        cout << c.getTitle() << ", ";
    }

}

void Admin::addgrade(System_Manager& manager)
{
    string stud_id, course_id;
    string grade, semester;

    cout << "Enter student id: ";
    cin >> stud_id;
    cout << "Enter course id: ";
    cin >> course_id;
    cout << "Enter course grade: ";
    cin >> grade;
    cout << "Enter course semester: ";
    cin >> semester;

    Student student = manager.getStudent(stud_id);
    Course course = manager.getCourse(course_id);

    CompletedCourse completed_course;
    completed_course.grade = grade;
    completed_course.course = course;
    completed_course.semester = semester;

    student.addCompletedCourse(completed_course);

    cout << "Student grades: \n";
    for (CompletedCourse c : student.getCompletedCourses()) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }

}

void Admin::updategrade(System_Manager& manager)
{
    string stud_id, course_id;
    string new_grade;
    cout << "Enter student id: ";
    cin >> stud_id;

    Student student = manager.getStudent(stud_id);
    vector<CompletedCourse> courses = student.getCompletedCourses();

    cout << "Student grades: \n";
    for (CompletedCourse c : courses) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }

    cout << "Enter course id: ";
    cin >> course_id;
    cout << "Enter new grade: ";
    cin >> new_grade;

    for (CompletedCourse c : courses) {
        if (c.course.getCourseID() == course_id) {
            c.grade = new_grade;
            break;
        }
    }

    cout << "Updated student grades: \n";
    for (CompletedCourse c : courses) {
        cout << c.course.getCourseID() << ": " << c.semester << ", " << c.grade << "\n";
    }

}

void Admin::addStudent(System_Manager& manager)
{
    string name, id, year, email;
    cout << "Enter student name: " << "\n";
    cin >> name;
    cout << "Enter student id: " << "\n";
    cin >> id;
    cout << "Enter student year: " << "\n";
    cin >> year;
    cout << "Enter student e-mail: " << "\n";
    cin >> email;

    Student new_student(name, id, year, email);
    manager.addStudent(id, new_student);

    cout << "Student details: " << "\n"
        << "Name: " << new_student.getName() << "\n"
        << "ID: " << new_student.getId() << "\n"
        << "Year : " << new_student.getYear() << "\n"
        << "E-mail: " << new_student.getEmail() << "\n";
}
