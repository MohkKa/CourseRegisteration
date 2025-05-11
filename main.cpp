#include "System_Manager.h"
#include "Admin.h"
#include "Student.h"
#include <iostream>
#include <limits>

using namespace std;

void adminMenu(System_Manager &manager, Admin &admin);

void studentMenu(System_Manager &manager, Student &student);

void loginScreen(System_Manager &manager);

bool isValidInteger(const string &input) {
    if (input.empty()) return false;
    for (char ch: input) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

int main() {
    System_Manager manager;

    loginScreen(manager);

    return 0;
}

void loginScreen(System_Manager &manager) {
    manager.readAdminsFromFile();
    manager.readCoursesFromFile();
    manager.readStudentsFromFile();

    while (true) {
        cout << "\n=== University Course Management System ===" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Student Login" << endl;
        cout << "3. Exit" << endl;

        int choice = 0;
        int attempts = 0;
        bool validInput = false;

        while (attempts < 3) {
            string input;
            cout << "Enter choice: ";
            getline(cin, input);

            if (isValidInteger(input)) {
                try {
                    choice = stoi(input);
                    validInput = true;
                    break;
                } catch (...) {
                    cout << "Conversion error. Try again...." << endl;
                    attempts++;
                }
            } else {
                if (attempts < 2) {
                    cout << "Invalid input. Please enter a valid integer (digits only)." << endl;
                }
                attempts++;
            }
        }

        if (!validInput) {
            cout << "Too many invalid attempts. Try again later...." << endl;
            return;
        }

        if (choice == 3) {
            cout << "Exiting system..." << endl;
            manager.writeAdminsToFile();
            manager.writeStudentsToFile();
            manager.writeCoursesToFile();

            break;
        }
        if (choice == 1 || choice == 2) {
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
            } else if (choice == 2) {
                if (manager.students.contains(username) &&
                    manager.students[username].getPassword() == password) {
                    studentMenu(manager, manager.students[username]);
                } else {
                    cout << "Invalid student credentials!" << endl;
                }
            }
        } else {
            cout << "Invalid choice!" << endl;
        }
    }
}

void adminMenu(System_Manager &manager, Admin &admin) {
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
        cout<< "10.undo update"<<endl;
        cout << "11. Logout" << endl;


        int choice = 0;
        int attempts = 0;
        bool validInput = false;

        while (attempts < 3) {
            string input;
            cout << "Enter choice: ";
            getline(cin, input);

            if (isValidInteger(input)) {
                try {
                    choice = stoi(input);
                    validInput = true;
                    break;
                } catch (...) {
                    cout << "Conversion error. Try again...." << endl;
                    attempts++;
                }
            } else {
                if (attempts < 2) {
                    cout << "Invalid input. Please enter a valid integer (digits only)." << endl;
                }
                attempts++;
            }
        }

        if (!validInput) {
            cout << "Too many invalid attempts. Returning to main menu...." << endl;
            return;
        }

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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (prereqChoice == 1) {
                    admin.addPrereq(manager);
                } else if (prereqChoice == 2) {
                    admin.removePrereq(manager);
                }
                break;
            }
            case 9: {
                string newPass, confirmPass;
                cout << "Enter new password: ";
                getline(cin, newPass);
                cout << "Confirm password: ";
                getline(cin, confirmPass);
                if (newPass == confirmPass) {
                    manager.editAdminPass(admin.getId(), newPass);
                } else {
                    cout << "Passwords do not match." << endl;
                }
                break;
            }
            case 10:{
                string  id;
                cout<<"enter course id";
                cin>>id;
                if (manager.courses.find(id) != manager.courses.end()) {
                    Course course = manager.getCourse(id);
                    Course pre_update = course.undoupdate(course);
                    course.displayAfterUndo(pre_update);
                } else
                {
                    cout << "Course not found!" << endl;
                }

            }
            case 11:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

void studentMenu(System_Manager &manager, Student &student) {
    for (const auto &[courseID, course]: manager.courses) {
        bool alreadyRegistered = false;
        for (const auto &regCourse: student.registeredCourses) {
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
        cout << "8. View Completed Courses" << endl;
        cout << "9. logout" << endl;

        int choice = 0;
        int attempts = 0;
        bool validInput = false;

        while (attempts < 3) {
            string input;
            cout << "Enter choice: ";
            getline(cin, input);

            if (isValidInteger(input)) {
                try {
                    choice = stoi(input);
                    validInput = true;
                    break;
                } catch (...) {
                    cout << "Conversion error. Try again...." << endl;
                    attempts++;
                }
            } else {
                if (attempts < 2) {
                    cout << "Invalid input. Please enter a valid integer (digits only)." << endl;
                }
                attempts++;
            }
        }

        if (!validInput) {
            cout << "Too many invalid attempts. Returning to main menu...." << endl;
            return;
        }

        switch (choice) {
            case 1:
                student.showAvailableCourses();
                break;
            case 2:
                manager.showEligibleCourses(student.getId());
                break;
            case 3: {
                /*student.registerCourse();*/
                string courseid;
                cout<<"enter course id";
                cin>>courseid;
                if (manager.isStudentEligible(student.getId(),courseid)) {
                    cout<<"registered successfully";
                }else {
                    cout<<"you are not eligible for this course";
                }
                /*for (Course course:manager.courses[courseid].getPrerequisites()) {
                    cout<<course.getCourseID()<<endl;
                }*/
                break;
            }
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
                string newPass, confirmPass;
                cout << "Enter new password: ";
                getline(cin, newPass);
                cout << "Confirm password: ";
                getline(cin, confirmPass);
                if (newPass == confirmPass) {
                    manager.editStudentPass(student.getId(), newPass);
                } else {
                    cout << "Passwords do not match." << endl;
                }
                break;
            }
            case 8:
                manager.showCompletedCourses(student.getId());
                break;
            case 9:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}
