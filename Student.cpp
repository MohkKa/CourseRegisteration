#include "Student.h"
#include <algorithm>
#include <iostream>

Student::Student(const std::string &name, const std::string &id,
                 const std::string &year, const std::string &email) {
    this->name = name;
    this->id = id;
    this->year = year;
    this->email = email;
}

Student::Student(const std::string &name, const std::string &id,
                 const std::string &year, const std::string &email, const std::string &password) {
    this->name = name;
    this->id = id;
    this->year = year;
    this->email = email;
    this->password = password;
}

double Student::totalCompletedCreditHours(Student s) {
    double totalHours = 0;
    for (const auto &completedCourse: s.completedCourses) {
        totalHours += completedCourse.course.getCreditHour();
    }
    return totalHours;
}

void Student::addCompletedCourse(const CompletedCourse &course) {
    if (convertGradeToGPA(course.grade) == -1) {
        std::cerr << "Invalid grade: '" << course.grade << "'\n"; // ADD THIS LINE
        throw std::invalid_argument("Invalid grade provided for completed course");
    }
    completedCourses.push_back(course);
}

void Student::addAvailableCourse(const Course &course) {
    availableCourses.push_back(course);
}

void Student::viewGrade() const {
    if (completedCourses.empty()) {
        std::cout << "No Courses Completed.\n";
        return;
    }

    int maxCourseNameWidth = 12;
    int maxSemesterWidth = 10;
    int maxGradeWidth = 7;

    for (const auto &[course, semester, grade]: completedCourses) {
        maxCourseNameWidth = std::max(maxCourseNameWidth, static_cast<int>(course.getTitle().length()));
        maxSemesterWidth = std::max(maxSemesterWidth, static_cast<int>(semester.length()));
        maxGradeWidth = std::max(maxGradeWidth, static_cast<int>(grade.length()));
    }

    std::cout << std::left << std::setw(maxCourseNameWidth) << "Course Name" << " | "
            << std::left << std::setw(maxSemesterWidth) << "Semester" << " | "
            << std::left << std::setw(maxGradeWidth) << "Grade" << "\n";

    std::cout << std::string(maxCourseNameWidth + maxSemesterWidth + maxGradeWidth + 6, '-') << "\n";

    for (const auto &completedCourse: completedCourses) {
        std::cout << std::left << std::setw(maxCourseNameWidth) << completedCourse.course.getTitle() << " | "
                << std::left << std::setw(maxSemesterWidth) << completedCourse.semester << " | "
                << std::left << std::setw(maxGradeWidth) << completedCourse.grade << "\n";
    }
}

double Student::convertGradeToGPA(const std::string grade) {
    if (grade == "A+" || grade == "A") return 4.0;
    if (grade == "A-") return 3.8;
    if (grade == "B+") return 3.7;
    if (grade == "B") return 3.4;
    if (grade == "B-") return 3.1;
    if (grade == "C+") return 2.8;
    if (grade == "C") return 2.5;
    if (grade == "C-") return 2.2;
    if (grade == "D+") return 1.8;
    if (grade == "D") return 1.5;
    if (grade == "D-") return 1.2;
    if (grade == "F") {
        warnings++;
        if (warnings == 6) cout << "Please go to Students Affair ,because you are about to get kicked out";
        else
            warnings++;
        return -3;
    }
    return -1; // Invalid grade
}

double Student::calcGPA()  {
    double totalGradePoints = 0;
    double totalCreditHours = 0;

    for (const auto &completedCourse: completedCourses) {
        double gpaGrade = convertGradeToGPA(completedCourse.grade);
        if (gpaGrade != -1) {
            double creditHour = completedCourse.course.getCreditHour();
            totalGradePoints += gpaGrade * creditHour;
            totalCreditHours += creditHour;
        }
    }

    if (totalCreditHours == 0) {
        return 0.0;
    }

    return totalGradePoints / totalCreditHours;
}

void Student::showAvailableCourses() const {
    if (availableCourses.empty()) {
        std::cout << "No available courses to show.\n";
        return;
    }

    for (const auto &course: availableCourses) {
        std::cout << "\n";
        course.showCourseDescription();
    }
}

bool Student::checkPrerequisite(const Course &course) const {
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

void Student::leftPrerequisites(std::deque<Course> leftCourses) {
    if (leftCourses.empty()) {
        std::cout << "You have no remaining prerequisites.\n";
        return;
    }

    std::cout << "You still need to complete the following prerequisites:\n";
    while (!leftCourses.empty()) {
        Course c = leftCourses.front();
        leftCourses.pop_front();
        std::cout << " - " << c.getTitle() << " (ID: " << c.getCourseID() << ")\n";
    }
}

void Student::registerCourse() {
    if (availableCourses.empty()) {
        std::cout << "No courses available for registration.\n";
        return;
    }

    showAvailableCourses();

    std::string courseID;
    std::cout << "Please enter the course ID: ";
    std::cin >> courseID;


    bool found = false;

    for (const auto &course: availableCourses) {
        if (course.getCourseID() == courseID) {
            found = true;

            if (checkPrerequisite(course)) {
                registeredCourses.push_back(course);
                std::cout << "Course \"" << course.getTitle() << "\" registered successfully!\n";
                for (int i = 0; i < availableCourses.size(); ++i) {
                    if (availableCourses[i].getCourseID() == courseID) {
                        availableCourses.erase(availableCourses.begin() + i);
                        break;
                    }
                }
            } else {
                std::cout << "You do not meet the prerequisites for \"" << course.getTitle() << "\".\n";

                std::deque<Course> leftCourses;
                for (const Course &prereq: course.getPrerequisites()) {
                    bool foundPrereq = false;

                    for (const auto &completed: completedCourses) {
                        if (completed.course.getCourseID() == prereq.getCourseID()) {
                            foundPrereq = true;
                            break;
                        }
                    }

                    if (!foundPrereq) {
                        leftCourses.push_back(prereq);
                    }
                }

                leftPrerequisites(leftCourses);
            }
            break;
        }
    }

    if (!found) {
        std::cout << "Course ID not found in available courses.\n";
    }
}

void Student::dropCourse(const std::string &courseID) {
    const auto it = std::ranges::find_if(registeredCourses, [&](const Course &course) {
        return course.getCourseID() == courseID;
    });

    for (int i = 0; i < registeredCourses.size(); ++i) {
        if (registeredCourses[i].getCourseID() == courseID) {
            addAvailableCourse(registeredCourses[i]);
            break;
        }
    }

    if (it != registeredCourses.end()) {
        std::cout << " Course dropped: " << it->getTitle() << " (ID: " << it->getCourseID() << ")\n";
        registeredCourses.erase(it);
    } else {
        std::cout << "⚠️ You are not registered in a course with ID: " << courseID << "\n";
    }
}

void Student::generateTranscript()  {
    std::cout << "\n=========== STUDENT TRANSCRIPT ===========\n";
    std::cout << " Name  : " << name << "\n";
    std::cout << " ID    : " << id << "\n";
    std::cout << " Email : " << email << "\n";
    std::cout << " Year  : " << year << "\n";
    std::cout << " GPA   : " << std::fixed << std::setprecision(2) << calcGPA() << "\n";

    std::cout << "\n Completed Courses:\n";
    if (completedCourses.empty()) {
        std::cout << " --- No courses completed yet---\n";
    } else {
        std::cout << std::left << std::setw(25) << "Course Name"
                << std::setw(15) << "Semester"
                << std::setw(10) << "Grade" << "\n";

        std::cout << std::string(50, '-') << "\n";
        for (const auto &course: completedCourses) {
            std::cout << std::left << std::setw(25) << course.course.getTitle()
                    << std::setw(15) << course.semester
                    << std::setw(10) << course.grade << "\n";
        }
    }

    std::cout << "\n Registered Courses:\n";
    if (registeredCourses.empty()) {
        std::cout << " - No courses currently registered.\n";
    } else {
        for (const auto &course: registeredCourses) {
            std::cout << " - " << course.getTitle() << " (ID: " << course.getCourseID()
                    << ", Credit Hours: " << course.getCreditHour() << ")\n";
        }
    }

    std::cout << "===========================================\n";
}

bool Student::FindCompletedCourse(const std::string &id) const {
    for (auto &c: completedCourses) {
        if (c.course.getCourseID() == id) return true;
    }

    return false;
}

std::vector<CompletedCourse>& Student::getCompletedCoursesRef() {
    return completedCourses;
}
