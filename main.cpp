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
            if (input.empty()) {
                cout << "Input cannot be empty. Try again." << endl;
                attempts++;
                continue;
            }

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

            while (true) {
                cout << "Username: ";
                getline(cin, username);
                if (username.empty()) {
                    cout << "Username cannot be empty. Try again." << endl;
                } else
                    break;
            }

            while (true) {
                cout << "Password: ";
                getline(cin, password);
                if (password.empty()) {
                    cout << "Password cannot be empty. Try again." << endl;
                } else
                    break;
            }

            if (choice == 1) {
                if (manager.admins.contains(username) &&
                manager.admins[username].getPassword() == password) {
                    adminMenu(manager, manager.admins[username]);
                } else {
                    cout << "Invalid admin credentials!" << endl;
                }
            } else {
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

            if (input.empty()) {
                cout << "Input cannot be empty. Please enter a number." << endl;
                attempts++;
                continue;
            }

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
                string input;

                while (true) {
                    cout << "1. Add Grade\n2. Update Grade\nChoice: ";
                    getline(cin, input);

                    if (input.empty()) {
                        cout << "Input cannot be empty. Please try again.\n";
                        continue;
                    }

                    try {
                        gradeChoice = stoi(input);
                        if (gradeChoice == 1) {
                            admin.addgrade(manager);
                            break;
                        } else if (gradeChoice == 2) {
                            admin.updategrade(manager);
                            break;
                        } else {
                            cout << "Invalid choice. Please enter 1 or 2.\n";
                        }
                    } catch (...) {
                        cout << "Invalid input. Please enter digits only.\n";
                    }
                }
                break;
            }
            case 8: {
                int prereqChoice;
                string input;

                while (true) {
                    cout << "1. Add Prerequisite\n2. Remove Prerequisite\nChoice: ";
                    getline(cin, input);

                    if (input.empty()) {
                        cout << "Input cannot be empty. Please enter 1 or 2.\n";
                        continue;
                    }

                    bool valid = true;
                    for (char ch : input) {
                        if (!isdigit(ch)) {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid) {
                        cout << "Invalid input. Please enter digits only.\n";
                        continue;
                    }

                    try {
                        prereqChoice = stoi(input);
                        if (prereqChoice == 1) {
                            admin.addPrereq(manager);
                        } else if (prereqChoice == 2) {
                            admin.removePrereq(manager);
                        } else {
                            cout << "Invalid choice. Please enter 1 or 2.\n";
                            continue;
                        }
                        break;
                    } catch (...) {
                        cout << "Conversion error. Please try again.\n";
                    }
                }

                break;
            }
            case 9: {

                string newPass, confirmPass;

                while (true) {
                    cout << "Enter new password: ";
                    getline(cin, newPass);
                    if (newPass.empty()) {
                        cout << "Password cannot be empty. Please try again.\n";
                        continue;
                    }

                    cout << "Confirm password: ";
                    getline(cin, confirmPass);
                    if (confirmPass.empty()) {
                        cout << "Confirmation cannot be empty. Please try again.\n";
                        continue;
                    }

                    if (newPass == confirmPass) {
                        manager.editAdminPass(admin.getId(), newPass);
                        cout << "Password updated successfully.\n";
                        break;
                    }
                    cout << "Passwords do not match. Please try again.\n";
                }
                break;
            }
            case 10:{
                string id;
                while (true) {
                    cout << "Enter course ID: ";
                    getline(cin, id);

                    if (id.empty()) {
                        cout << "Course ID cannot be empty. Please try again.\n";
                        continue;
                    }

                    if (manager.courses.find(id) != manager.courses.end()) {
                        Course course = manager.getCourse(id);
                        Course pre_update = course.undoupdate(course);
                        course.displayAfterUndo(pre_update);
                        break;
                    } else {
                        cout << "Course not found! Please try again.\n";
                    }
                }

                break;
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

            if (input.empty()) {
                cout << "Input cannot be empty. Please enter a number." << endl;
                attempts++;
                continue;
            }

            if (isValidInteger(input)) {
                try {
                    choice = stoi(input);
                    validInput = true;
                    break;
                } catch (...) {
                    cout << "Conversion error. Try again..." << endl;
                    attempts++;
                }
            } else {
                cout << "Invalid input. Please enter a valid integer (digits only)." << endl;
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
                while (true) {
                    cout << "Enter course ID: ";
                    getline(cin, courseid);

                    if (!courseid.empty()) break;
                    cout << "Course ID cannot be empty. Please try again.\n";
                }

                if (manager.isStudentEligible(student.getId(), courseid)) {
                    cout << "Registered successfully";
                } else {
                    cout << "You are not eligible for this course";
                }

                /*for (Course course:manager.courses[courseid].getPrerequisites()) {
                    cout<<course.getCourseID()<<endl;
                }*/
                break;
            }
            case 4: {
                string courseID;
                while (true) {
                    cout << "Enter course ID to drop: ";
                    getline(cin, courseID);

                    if (!courseID.empty()) break;
                    cout << "Course ID cannot be empty. Please try again.\n";
                }
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
                while (true) {
                    cout << "Enter new password: ";
                    getline(cin, newPass);
                    if (newPass.empty()) {
                        cout << "Password cannot be empty. Please try again.\n";
                        continue;
                    }

                    cout << "Confirm password: ";
                    getline(cin, confirmPass);
                    if (confirmPass.empty()) {
                        cout << "Confirmation cannot be empty. Please try again.\n";
                        continue;
                    }

                    if (newPass == confirmPass) {
                        manager.editStudentPass(student.getId(), newPass);
                        break;
                    }

                    cout << "Passwords do not match. Please try again.\n";
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
