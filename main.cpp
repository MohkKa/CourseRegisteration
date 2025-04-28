#include "System_Manager.h"
#include "Admin.h"
#include "Student.h"
#include <iostream>

using namespace std;

void adminMenu(System_Manager& manager, Admin& admin);
void studentMenu(System_Manager& manager, Student& student);
void loginScreen(System_Manager& manager);

int main() {
    System_Manager manager;

    // Load initial data (uncomment when file I/O is implemented)
    // manager.readAdminsFromFile();
    // manager.readStudentsFromFile();
    // manager.readCoursesFromFile();

    if (manager.admins.empty()) {
        Admin defaultAdmin("ADM001", "Default Admin", "admin", "admin123");
        manager.admins["admin"] = defaultAdmin;
    }

    if (manager.students.empty()) {
        Student defaultStudent("John Doe", "STU001", "2023", "john@uni.edu");
        defaultStudent.setPassword("student123");
        manager.students["STU001"] = defaultStudent;
    }

    if (manager.courses.empty()) {
        // Add some sample courses if none exist
        Instructor inst1{"Dr. Smith", "smith@uni.edu"};
        CourseDescription desc1{"Introduction to Programming", "Basic programming concepts", 3, inst1};
        Course course1("CS101", desc1);

        Instructor inst2{"Dr. Johnson", "johnson@uni.edu"};
        CourseDescription desc2{"Data Structures", "Advanced programming concepts", 4, inst2};
        Course course2("CS201", desc2);

        manager.addCourse("CS101", course1);
        manager.addCourse("CS201", course2);

        course2.addPrerequisite(course1, manager);
    }

    loginScreen(manager);

    // Save data before exiting (uncomment when file I/O is implemented)
    // manager.writeAdminsToFile();
    // manager.writeStudentsToFile();
    // manager.writeCoursesToFile();

    return 0;
}

void loginScreen(System_Manager& manager) {
    while (true) {
        cout << "\n=== University Course Management System ===" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Student Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 3) {
            cout << "Exiting system..." << endl;
            break;
        }

        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (choice == 1) {
            if (manager.admins.contains(username) &&
                manager.admins[username].getPassword() == password) {
                adminMenu(manager, manager.admins[username]);
                } else {
                    cout << "Invalid admin credentials!" << endl;
                }
        }
        else if (choice == 2) {
            if (manager.students.contains(username) &&
                manager.students[username].getPassword() == password) {
                studentMenu(manager, manager.students[username]);
                } else {
                    cout << "Invalid student credentials!" << endl;
                }
        }
    }
}

void adminMenu(System_Manager& manager, Admin& admin) {
    while (true) {
        cout << "\n=== ADMIN MENU ===" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Update Course" << endl;
        cout << "3. Remove Course" << endl;
        cout << "4. Undo Last Added Course" << endl;
        cout << "5. Display Courses by Credit Hours" << endl;
        cout << "6. Add Student" << endl;
        cout << "7. Add/Update Grades" << endl;
        cout << "8. Manage Prerequisites" << endl;
        cout << "9. Change Password" << endl;
        cout << "10. Logout" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                admin.addCourse(manager);
                break;
            case 2:
                admin.updateCourse(manager);
                break;
            case 3:
                admin.removeCourse(manager);
                break;
            case 4:
                admin.undoLastAddedCourse(manager);
                break;
            case 5:
                admin.displayCoursesByCreditHours(manager);
                break;
            case 6:
                admin.addStudent(manager);
                break;
            case 7: {
                int gradeChoice;
                cout << "1. Add Grade\n2. Update Grade\nChoice: ";
                cin >> gradeChoice;
                if (gradeChoice == 1) {
                    admin.addgrade(manager);
                } else if (gradeChoice == 2) {
                    admin.updategrade(manager);
                }
                break;
            }
            case 8: {
                int prereqChoice;
                cout << "1. Add Prerequisite\n2. Remove Prerequisite\nChoice: ";
                cin >> prereqChoice;
                if (prereqChoice == 1) {
                    admin.addPrereq(manager);
                } else if (prereqChoice == 2) {
                    admin.removePrereq(manager);
                }
                break;
            }
            case 9: {
                string newPass;
                cout << "Enter new password: ";
                getline(cin, newPass);
                manager.editAdminPass(admin.getUsername(), newPass);
                break;
            }
            case 10:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

void studentMenu(System_Manager& manager, Student& student) {
    // Update available courses for the student
    for (const auto& [courseID, course] : manager.courses) {
        bool alreadyRegistered = false;
        for (const auto& regCourse : student.registeredCourses) {
            if (regCourse.getCourseID() == courseID) {
                alreadyRegistered = true;
                break;
            }
        }

        if (!alreadyRegistered && student.checkPrerequisite(course)) {
            student.addAvailableCourse(course);
        }
    }

    while (true) {
        cout << "\n=== STUDENT MENU ===" << endl;
        cout << "1. View Available Courses" << endl;
        cout << "2. View Eligible Courses" << endl;
        cout << "3. Register for Course" << endl;
        cout << "4. Drop Course" << endl;
        cout << "5. View Grades" << endl;
        cout << "6. Generate Transcript" << endl;
        cout << "7. Change Password" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                student.showAvailableCourses();
            break;
            case 2:
                manager.showEligibleCourses(student.getId());
            break;
            case 3:
                student.registerCourse();
            break;
            case 4: {
                string courseID;
                cout << "Enter course ID to drop: ";
                getline(cin, courseID);
                student.dropCourse(courseID);
                break;
            }
            case 5:
                student.viewGrade();
            break;
            case 6:
                student.generateTranscript();
            break;
            case 7: {
                string newPass;
                cout << "Enter new password: ";
                getline(cin, newPass);
                manager.editStudentPass(student.getId(), newPass);
                break;
            }
            case 8:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}
