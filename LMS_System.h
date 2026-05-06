#ifndef LMS_H
#define LMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <limits> 
using namespace std;

void pauseConsole() {
    cout << "\n Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void clearScreen() {
    system("cls");
}

int getValidChoice(int minVal, int maxVal) {
    int input;
    while (true) {
        cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " - Error: Invalid input! Please enter a number.\n";
            cout << " Select an option: ";
        }
        else if (input < minVal || input > maxVal) {
            cout << " - Error: Invalid choice! Please select a number between " << minVal << " and " << maxVal << ".\n";
            cout << " Select an option: ";
        }
        else {
            return input;
        }
    }
}

string encodeSpaces(string str) {
    for (char& c : str) {
        if (c == ' ') c = '_';
    }
    return str;
}

string decodeSpaces(string str) {
    for (char& c : str) {
        if (c == '_') c = ' ';
    }
    return str;
}

class StudentGrades {
public:
    string courseID;
    string studentID;
    float quizzes[6] = { -1, -1, -1, -1, -1, -1 };
    float assignments[2] = { -1, -1 };
    float midExam = -1;
    float finalExam = -1;

    float safeMark(float mark) {
        if (mark == -1) {
            return 0;
        }
        else {
            return mark;
        }
    }

    void displayAndCalculate() {
        float qSum = 0, aSum = 0;

        cout << "\n --- Grades for " << courseID << " ---\n";
        cout << " Quizzes (out of 10): ";
        for (int i = 0; i < 6; i++) {
            if (quizzes[i] == -1) {
                cout << "[N/A] ";
            }
            else {
                cout << "[" << quizzes[i] << "] ";
                qSum += quizzes[i];
            }
        }

        cout << "\n Assignments (out of 20): ";
        for (int i = 0; i < 2; i++) {
            if (assignments[i] == -1) {
                cout << "[N/A] ";
            }
            else {
                cout << "[" << assignments[i] << "] ";
                aSum += assignments[i];
            }
        }

        cout << "\n Mid Exam (out of 100): " << ((midExam == -1) ? "N/A" : to_string((int)midExam));
        cout << "\n Final Exam (out of 100): " << ((finalExam == -1) ? "N/A" : to_string((int)finalExam));

        float qPct = (qSum / 60.0) * 100.0;
        float aPct = (aSum / 40.0) * 100.0;
        float mPct = (safeMark(midExam) / 100.0) * 100.0;
        float fPct = (safeMark(finalExam) / 100.0) * 100.0;

        float aggregate = (qSum / 60.0 * 10.0) + (aSum / 40.0 * 15.0) + (safeMark(midExam) / 100.0 * 25.0) + (safeMark(finalExam) / 100.0 * 50.0);

        cout << "\n\n --- Performance Summary ---";
        cout << fixed << setprecision(1);
        cout << "\n Quizzes Total: " << qPct << "%";
        cout << "\n Assignments Total: " << aPct << "%";
        cout << "\n Mid Exam Total: " << mPct << "%";
        cout << "\n Final Exam Total: " << fPct << "%";
        cout << "\n========================================";
        cout << "\n OVERALL COURSE AGGREGATE: " << aggregate << "%\n";
        cout << "========================================\n";
    }
};

class User {
protected:
    string username;
    string password;
    string role;

public:
    User(string u, string p, string r) {
        username = u;
        password = p;
        role = r;
    }

    virtual void showDashboard() = 0;

    string getUsername() {
        return username;
    }

    bool checkPassword(string inputPass) {
        return password == inputPass;
    }
    virtual ~User() {}
};

class Student : public User {
public:
    Student(string u, string p) : User(u, p, "Student") {}

    void showDashboard() override {
        bool inDashboard = true;
        while (inDashboard) {
            clearScreen();
            cout << "========================================\n";
            cout << "          STUDENT PORTAL                \n";
            cout << "========================================\n";
            cout << " Welcome, " << username << "\n";
            cout << "----------------------------------------\n";
            cout << " [1] Enroll in a Course\n";
            cout << " [2] View Enrolled Courses\n";
            cout << " [3] View Pending Assignments & Submit\n";
            cout << " [4] View Grades & Aggregate\n";
            cout << " [5] Logout\n";
            cout << "========================================\n";
            cout << " Select an option: ";

            int choice = getValidChoice(1, 5);

            if (choice == 1) {
                string selectedCourse;
                cout << "\n Enter Course ID to enroll: ";
                cin >> selectedCourse;

                bool courseExists = false;
                ifstream cFile("courses.txt");
                string fileCID, fileCName, fileInst;
                while (cFile >> fileCID >> fileCName >> fileInst) {
                    if (fileCID == selectedCourse) {
                        courseExists = true;
                        break;
                    }
                }
                cFile.close();

                if (!courseExists) {
                    cout << " - Error: Course '" << selectedCourse << "' does not exist.\n";
                    pauseConsole();
                    continue;
                }

                bool alreadyEnrolled = false;
                ifstream eFileCheck("enrollments.txt");
                string eUser, eCourse;
                while (eFileCheck >> eUser >> eCourse) {
                    if (eUser == username && eCourse == selectedCourse) {
                        alreadyEnrolled = true;
                        break;
                    }
                }
                eFileCheck.close();

                if (alreadyEnrolled) {
                    cout << " - You are already enrolled in '" << selectedCourse << "'.\n";
                    pauseConsole();
                    continue;
                }

                ofstream enrollFile("enrollments.txt", ios::app);
                enrollFile << username << " " << selectedCourse << "\n";
                enrollFile.close();
                cout << " - Successfully enrolled in " << selectedCourse << "!\n";
                pauseConsole();
            }
            else if (choice == 2) {
                cout << "\n --- Your Enrollments ---\n";
                ifstream enrollFile("enrollments.txt");
                string studentName, courseID;
                bool found = false;
                while (enrollFile >> studentName >> courseID) {
                    if (studentName == username) {
                        cout << " - " << courseID << "\n";
                        found = true;
                    }
                }
                enrollFile.close();
                if (!found) {
                    cout << " You are not enrolled in any courses.\n";
                }
                pauseConsole();
            }
            else if (choice == 3) {
                cout << "\n --- Pending Assignments ---\n";

                vector<string> enrolledCourses;
                ifstream eFile("enrollments.txt");
                string eUser, eCourse;
                while (eFile >> eUser >> eCourse) {
                    if (eUser == username) {
                        enrolledCourses.push_back(eCourse);
                    }
                }
                eFile.close();

                vector<string> subCourses;
                vector<int> subCounts;

                ifstream subFileCheck("submissions.txt");
                string sCID, sUser, sText;
                while (subFileCheck >> sCID >> sUser >> sText) {
                    if (sUser == username) {
                        bool found = false;
                        for (size_t i = 0; i < subCourses.size(); i++) {
                            if (subCourses[i] == sCID) {
                                subCounts[i]++;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            subCourses.push_back(sCID);
                            subCounts.push_back(1);
                        }
                    }
                }
                subFileCheck.close();

                ifstream assignFile("assignments.txt");
                string cID, deadline, promptTxt;
                bool foundPending = false;

                vector<string> validPendingCourses;

                while (assignFile >> cID >> deadline >> promptTxt) {
                    bool isEnrolled = false;
                    for (size_t i = 0; i < enrolledCourses.size(); i++) {
                        if (enrolledCourses[i] == cID) {
                            isEnrolled = true;
                            break;
                        }
                    }

                    if (isEnrolled) {
                        bool skip = false;
                        for (size_t i = 0; i < subCourses.size(); i++) {
                            if (subCourses[i] == cID && subCounts[i] > 0) {
                                subCounts[i]--;
                                skip = true;
                                break;
                            }
                        }

                        if (!skip) {
                            cout << " [" << cID << "] Due: " << deadline << "\n Task: " << decodeSpaces(promptTxt) << "\n\n";
                            foundPending = true;
                            validPendingCourses.push_back(cID);
                        }
                    }
                }
                assignFile.close();

                if (!foundPending) {
                    if (enrolledCourses.empty()) {
                        cout << " - You are not enrolled in any courses yet.\n";
                    }
                    else {
                        cout << " - You have no pending assignments! Great job.\n";
                    }
                    pauseConsole();
                    continue;
                }

                string targetCourse, submissionTemp;
                cout << " Enter Course ID to submit for: "; cin >> targetCourse;

                bool isValidTarget = false;
                for (size_t i = 0; i < validPendingCourses.size(); i++) {
                    if (validPendingCourses[i] == targetCourse) {
                        isValidTarget = true;
                        break;
                    }
                }

                if (!isValidTarget) {
                    cout << " - Error: That course has no pending assignments for you.\n";
                    pauseConsole();
                    continue;
                }

                cout << " Enter Submission Text: ";
                getline(cin >> ws, submissionTemp);

                ofstream subFile("submissions.txt", ios::app);
                subFile << targetCourse << " " << username << " " << encodeSpaces(submissionTemp) << "\n";
                subFile.close();
                cout << " - Assignment submitted successfully!\n";
                pauseConsole();
            }
            else if (choice == 4) {
                string targetCourse;
                cout << "\n Enter Course ID to view grades: ";
                cin >> targetCourse;

                ifstream gradeFile("grades.txt");
                StudentGrades sg;
                bool found = false;

                while (gradeFile >> sg.courseID >> sg.studentID >> sg.quizzes[0] >> sg.quizzes[1] >> sg.quizzes[2] >> sg.quizzes[3] >> sg.quizzes[4] >> sg.quizzes[5] >> sg.assignments[0] >> sg.assignments[1] >> sg.midExam >> sg.finalExam) {

                    if (sg.courseID == targetCourse && sg.studentID == username) {
                        sg.displayAndCalculate();
                        found = true;
                        break;
                    }
                }
                gradeFile.close();
                if (!found) cout << " - No grades published for you in " << targetCourse << " yet.\n";
                pauseConsole();
            }
            else if (choice == 5) {
                inDashboard = false;
            }
        }
    }
};

class Instructor : public User {
private:
    void updateGradeDatabase(StudentGrades newSg) {
        vector<StudentGrades> allGrades;

        ifstream fileIn("grades.txt");
        StudentGrades temp;
        bool recordExists = false;

        while (fileIn >> temp.courseID >> temp.studentID >> temp.quizzes[0] >> temp.quizzes[1] >> temp.quizzes[2] >> temp.quizzes[3] >> temp.quizzes[4] >> temp.quizzes[5] >> temp.assignments[0] >> temp.assignments[1] >> temp.midExam >> temp.finalExam) {

            if (temp.courseID == newSg.courseID && temp.studentID == newSg.studentID) {
                allGrades.push_back(newSg);
                recordExists = true;
            }
            else {
                allGrades.push_back(temp);
            }
        }
        fileIn.close();

        if (!recordExists) {
            allGrades.push_back(newSg);
        }

        ofstream fileOut("grades.txt");
        for (size_t i = 0; i < allGrades.size(); i++) {
            fileOut << allGrades[i].courseID << " " << allGrades[i].studentID << " " << allGrades[i].quizzes[0] << " " << allGrades[i].quizzes[1] << " " << allGrades[i].quizzes[2] << " " << allGrades[i].quizzes[3] << " " << allGrades[i].quizzes[4] << " " << allGrades[i].quizzes[5] << " " << allGrades[i].assignments[0] << " " << allGrades[i].assignments[1] << " " << allGrades[i].midExam << " " << allGrades[i].finalExam << "\n";
        }
        fileOut.close();
    }

public:
    Instructor(string u, string p) : User(u, p, "Instructor") {}

    void showDashboard() override {
        bool inDashboard = true;
        while (inDashboard) {
            clearScreen();
            cout << "========================================\n";
            cout << "         INSTRUCTOR PORTAL              \n";
            cout << "========================================\n";
            cout << " Welcome, " << username << "\n";
            cout << "----------------------------------------\n";
            cout << " [1] Create New Course\n";
            cout << " [2] Upload Course Assignment\n";
            cout << " [3] View Student Submissions\n";
            cout << " [4] Grade a Student\n";
            cout << " [5] Logout\n";
            cout << "========================================\n";
            cout << " Select an option: ";

            int choice = getValidChoice(1, 5);

            if (choice == 1) {
                string cID, cNameTemp;
                cout << "\n Enter Course ID: ";
                cin >> cID;

                bool courseExists = false;
                ifstream checkFile("courses.txt");
                string fileCID, dummy1, dummy2;
                while (checkFile >> fileCID >> dummy1 >> dummy2) {
                    if (fileCID == cID) { courseExists = true; break; }
                }
                checkFile.close();

                if (courseExists) {
                    cout << " - Error: Course ID '" << cID << "' is already taken.\n";
                    pauseConsole();
                    continue;
                }

                cout << " Enter Course Name: ";
                getline(cin >> ws, cNameTemp);

                ofstream file("courses.txt", ios::app);
                file << cID << " " << encodeSpaces(cNameTemp) << " " << username << "\n";
                file.close();
                cout << " - Course Created!\n";
                pauseConsole();
            }
            else if (choice == 2) {
                string cID, deadline, promptTemp;
                cout << "\n Enter Course ID: "; cin >> cID;

                bool ownsCourse = false;
                ifstream cFile("courses.txt");
                string fileCID, fileCName, fileInst;
                while (cFile >> fileCID >> fileCName >> fileInst) {
                    if (fileCID == cID && fileInst == username) {
                        ownsCourse = true; break;
                    }
                }
                cFile.close();

                if (!ownsCourse) {
                    cout << " - Error: You do not own Course '" << cID << "'.\n";
                    pauseConsole();
                    continue;
                }

                cout << " Enter Deadline (YYYY-MM-DD): ";
                cin >> deadline;
                cout << " Enter Assignment Prompt: ";
                getline(cin >> ws, promptTemp);

                ofstream file("assignments.txt", ios::app);
                file << cID << " " << deadline << " " << encodeSpaces(promptTemp) << "\n";
                file.close();
                cout << " - Assignment uploaded successfully!\n";
                pauseConsole();
            }
            else if (choice == 3) {
                cout << "\n --- Submissions for Your Courses ---\n";

                vector<string> myCourses;

                ifstream cFile("courses.txt");
                string fileCID, fileCName, fileInst;
                while (cFile >> fileCID >> fileCName >> fileInst) {
                    if (fileInst == username) {
                        myCourses.push_back(fileCID);
                    }
                }
                cFile.close();

                ifstream subFile("submissions.txt");
                string subCID, sID, text;
                bool foundAny = false;

                while (subFile >> subCID >> sID >> text) {
                    bool belongsToMe = false;
                    for (size_t i = 0; i < myCourses.size(); i++) {
                        if (myCourses[i] == subCID) belongsToMe = true;
                    }

                    if (belongsToMe) {
                        cout << " Course: " << subCID << " | Student: " << sID << "\n";
                        cout << " Text: " << decodeSpaces(text) << "\n---\n";
                        foundAny = true;
                    }
                }
                subFile.close();

                if (!foundAny) {
                    cout << " - No submissions found for your courses yet.\n";
                }
                pauseConsole();
            }
            else if (choice == 4) {
                StudentGrades sg;
                cout << "\n Enter Course ID: "; cin >> sg.courseID;

                bool ownsCourse = false;
                ifstream cFile("courses.txt");
                string fileCID, fileCName, fileInst;
                while (cFile >> fileCID >> fileCName >> fileInst) {
                    if (fileCID == sg.courseID && fileInst == username) {
                        ownsCourse = true; break;
                    }
                }
                cFile.close();

                if (!ownsCourse) {
                    cout << " - Error: You do not own Course '" << sg.courseID << "' or it does not exist.\n";
                    pauseConsole();
                    continue;
                }

                cout << " Enter Student Username: "; cin >> sg.studentID;

                bool isEnrolled = false;
                ifstream eFile("enrollments.txt");
                string eUser, eCourse;
                while (eFile >> eUser >> eCourse) {
                    if (eUser == sg.studentID && eCourse == sg.courseID) {
                        isEnrolled = true; break;
                    }
                }
                eFile.close();

                if (!isEnrolled) {
                    cout << " - Error: Student '" << sg.studentID << "' is not enrolled in this course.\n";
                    pauseConsole();
                    continue;
                }

                ifstream fileIn("grades.txt");
                StudentGrades temp;
                while (fileIn >> temp.courseID >> temp.studentID >> temp.quizzes[0] >> temp.quizzes[1] >> temp.quizzes[2] >> temp.quizzes[3] >> temp.quizzes[4] >> temp.quizzes[5] >> temp.assignments[0] >> temp.assignments[1] >> temp.midExam >> temp.finalExam) {
                    if (temp.courseID == sg.courseID && temp.studentID == sg.studentID) {
                        sg = temp;
                    }
                }
                fileIn.close();

                cout << "\n What are you grading?\n 1. Quizzes (1-6)\n 2. Assignments (1-2)\n 3. Mid Exam\n 4. Final Exam\n Choice: ";
                int gChoice = getValidChoice(1, 4);

                if (gChoice == 1) {
                    cout << " Which Quiz (1-6)? ";
                    int idx = getValidChoice(1, 6);
                    cout << " Enter Marks (out of 10): ";
                    cin >> sg.quizzes[idx - 1];
                }
                else if (gChoice == 2) {
                    cout << " Which Assignment (1-2)? ";
                    int idx = getValidChoice(1, 2);
                    cout << " Enter Marks (out of 20): ";
                    cin >> sg.assignments[idx - 1];
                }
                else if (gChoice == 3) {
                    cout << " Enter Mid Marks (out of 100): ";
                    cin >> sg.midExam;
                }
                else if (gChoice == 4) {
                    cout << " Enter Final Marks (out of 100): ";
                    cin >> sg.finalExam;
                }

                updateGradeDatabase(sg);
                cout << " - Grades Updated Successfully!\n";
                pauseConsole();
            }
            else if (choice == 5) {
                inDashboard = false;
            }
        }
    }
};

class LMS_System {
private:
    vector<User*> users;

    void initializeDatabase() {
        ofstream fuser("users.txt", ios::app);
        fuser.close();
        ofstream fcourse("courses.txt", ios::app);
        fcourse.close();
        ofstream fenrollment("enrollments.txt", ios::app);
        fenrollment.close();
        ofstream fassignment("assignments.txt", ios::app);
        fassignment.close();
        ofstream fsubmissions("submissions.txt", ios::app);
        fsubmissions.close();
        ofstream fgrades("grades.txt", ios::app);
        fgrades.close();
    }

    void loadUsers() {
        ifstream file("users.txt");
        string u, p, r;
        while (file >> u >> p >> r) {
            if (r == "Student") {
                users.push_back(new Student(u, p));
            }
            else if (r == "Instructor") {
                users.push_back(new Instructor(u, p));
            }
        }
        file.close();
    }

public:
    LMS_System() {
        initializeDatabase();
        loadUsers();
    }

    ~LMS_System() {
        for (size_t i = 0; i < users.size(); i++) {
            delete users[i];
        }
        users.clear();
    }

    void run() {
        bool running = true;
        while (running) {
            clearScreen();
            cout << "****************************************\n";
            cout << "* NUST CEME - LEARNING MANAGEMENT      *\n";
            cout << "* SYSTEM (LMS)                         *\n";
            cout << "****************************************\n";
            cout << " [1] Login\n";
            cout << " [2] Register\n";
            cout << " [3] Exit\n";
            cout << " Enter: ";

            int choice = getValidChoice(1, 3);

            if (choice == 1) {
                string u, p;
                cout << "\n Username: "; cin >> u;
                cout << " Password: "; cin >> p;
                bool found = false;
                for (size_t i = 0; i < users.size(); i++) {
                    if (users[i]->getUsername() == u) {
                        found = true;
                        if (users[i]->checkPassword(p)) {
                            users[i]->showDashboard();
                        }
                        else {
                            cout << " - Password incorrect.\n";
                            pauseConsole();
                        }
                        break;
                    }
                }
                if (!found) {
                    cout << " - Username not found.\n";
                    pauseConsole();
                }
            }
            else if (choice == 2) {
                string u, p, r;
                cout << " Role (1=Student, 2=Instructor): ";
                int rc = getValidChoice(1, 2);
                r = (rc == 1) ? "Student" : "Instructor";

                cout << " New Username: ";
                cin >> u;

                bool exists = false;
                for (size_t i = 0; i < users.size(); i++) {
                    if (users[i]->getUsername() == u) {
                        exists = true;
                        break;
                    }
                }

                if (exists) {
                    cout << " - Username already taken!\n";
                    pauseConsole();
                    continue;
                }

                cout << " New Password: ";
                cin >> p;

                ofstream file("users.txt", ios::app);
                file << u << " " << p << " " << r << "\n";
                file.close();

                if (r == "Student") {
                    users.push_back(new Student(u, p));
                }
                else {
                    users.push_back(new Instructor(u, p));
                }

                cout << " - Registered!\n";
                pauseConsole();
            }
            else if (choice == 3) running = false;
        }
    }
};

#endif