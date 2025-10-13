// File: main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>   // <--- added for sort
using namespace std;

// -------------------- Student --------------------
class Student {
public:
    string name;
    string roll;
    string department;
    double gpa;
    string contact;

    Student() : name(""), roll(""), department(""), gpa(0.0), contact("") {}
    Student(string n, string r, string d, double gp, string c)
        : name(n), roll(r), department(d), gpa(gp), contact(c) {}

    string serialize() const {
        ostringstream oss;
        oss << name << ',' << roll << ',' << department << ',' << gpa << ',' << contact;
        return oss.str();
    }

    static Student deserialize(const string &line) {
        Student s;
        istringstream iss(line);
        string token;
        if (getline(iss, token, ',')) s.name = token;
        if (getline(iss, token, ',')) s.roll = token;
        if (getline(iss, token, ',')) s.department = token;
        if (getline(iss, token, ',')) s.gpa = token.empty() ? 0.0 : stod(token);
        if (getline(iss, token, ',')) s.contact = token;
        return s;
    }
};

// -------------------- Storage --------------------
class Storage {
public:
    vector<Student> students;
    string filename;

    Storage(const string &fname = "students.txt") : filename(fname) {
        loadFromFile();
    }

    void loadFromFile() {
        students.clear();
        ifstream in(filename);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            students.push_back(Student::deserialize(line));
        }
        in.close();
    }

    void saveToFile() {
        ofstream out(filename);
        if (!out.is_open()) return;
        for (const auto &s : students) out << s.serialize() << '\n';
        out.close();
    }

    bool rollExists(const string &roll) const {
        for (const auto &s : students) if (s.roll == roll) return true;
        return false;
    }

    bool addStudent(const Student &s) {
        if (rollExists(s.roll)) return false;
        students.push_back(s);
        saveToFile();
        return true;
    }

    vector<Student> viewAll() const {
        return students;
    }

    bool viewByRoll(const string &roll, Student &out) const {
        for (const auto &s : students) if (s.roll == roll) { out = s; return true; }
        return false;
    }

    bool updateStudent(const string &roll, const Student &updated) {
        for (auto &s : students) {
            if (s.roll == roll) {
                s = updated;
                saveToFile();
                return true;
            }
        }
        return false;
    }

    bool deleteStudent(const string &roll) {
        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].roll == roll) {
                students.erase(students.begin() + i);
                saveToFile();
                return true;
            }
        }
        return false;
    }

    vector<Student> searchByName(const string &namePart) const {
        vector<Student> res;
        for (const auto &s : students) if (s.name.find(namePart) != string::npos) res.push_back(s);
        return res;
    }

    vector<Student> searchByDepartment(const string &deptPart) const {
        vector<Student> res;
        for (const auto &s : students) if (s.department.find(deptPart) != string::npos) res.push_back(s);
        return res;
    }

    void sortByGPA(bool descending = false) {
        sort(students.begin(), students.end(), [descending](const Student &a, const Student &b) {
            return descending ? (a.gpa > b.gpa) : (a.gpa < b.gpa);
        });
        saveToFile();
    }

    void sortByRoll(bool ascending = true) {
        sort(students.begin(), students.end(), [ascending](const Student &a, const Student &b) {
            return ascending ? (a.roll < b.roll) : (a.roll > b.roll);
        });
        saveToFile();
    }
};

// -------------------- Helpers --------------------
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printTableHeader() {
    cout << left << setw(20) << "Name"
         << setw(12) << "Roll"
         << setw(15) << "Department"
         << setw(6)  << "GPA"
         << setw(20) << "Contact" << '\n';
    cout << string(73, '-') << '\n';
}

void printStudentRow(const Student &s) {
    cout << left << setw(20) << s.name
         << setw(12) << s.roll
         << setw(15) << s.department
         << setw(6)  << s.gpa
         << setw(20) << s.contact << '\n';
}

// -------------------- Main --------------------
int main() {
    Storage store("students.txt");

    while (true) {
        cout << "\n===== Student Record Management =====\n";
        cout << "1. Add New Student\n";
        cout << "2. View All Students\n";
        cout << "3. View Student by Roll\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Search by Name\n";
        cout << "7. Search by Department\n";
        cout << "8. Sort by GPA\n";
        cout << "9. Sort by Roll\n";
        cout << "0. Exit\n";
        cout << "Select option: ";

        int choice = -1;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;

        if (choice == 1) {
            Student s;
            cout << "Name: "; getline(cin, s.name);
            cout << "Roll: "; getline(cin, s.roll);
            cout << "Department: "; getline(cin, s.department);
            cout << "GPA: "; cin >> s.gpa; cin.ignore();
            cout << "Contact: "; getline(cin, s.contact);
            if (store.addStudent(s))
                cout << "Student added successfully.\n";
            else
                cout << "Roll number already exists!\n";
        }
        else if (choice == 2) {
            auto all = store.viewAll();
            printTableHeader();
            for (const auto &s : all) printStudentRow(s);
        }
        else if (choice == 3) {
            string roll;
            cout << "Enter Roll: "; getline(cin, roll);
            Student s;
            if (store.viewByRoll(roll, s)) printStudentRow(s);
            else cout << "Student not found!\n";
        }
        else if (choice == 4) {
            string roll;
            cout << "Enter Roll to Update: "; getline(cin, roll);
            Student s;
            if (!store.viewByRoll(roll, s)) { cout << "Student not found!\n"; }
            else {
                cout << "New Name (" << s.name << "): "; string tmp; getline(cin, tmp); if (!tmp.empty()) s.name = tmp;
                cout << "New Department (" << s.department << "): "; getline(cin, tmp); if (!tmp.empty()) s.department = tmp;
                cout << "New GPA (" << s.gpa << "): "; getline(cin, tmp); if (!tmp.empty()) s.gpa = stod(tmp);
                cout << "New Contact (" << s.contact << "): "; getline(cin, tmp); if (!tmp.empty()) s.contact = tmp;
                if (store.updateStudent(roll, s)) cout << "Updated successfully.\n";
            }
        }
        else if (choice == 5) {
            string roll;
            cout << "Enter Roll to Delete: "; getline(cin, roll);
            if (store.deleteStudent(roll)) cout << "Deleted successfully.\n";
            else cout << "Student not found!\n";
        }
        else if (choice == 6) {
            string namePart;
            cout << "Enter Name (partial or full): "; getline(cin, namePart);
            auto res = store.searchByName(namePart);
            if (res.empty()) cout << "No students found.\n";
            else { printTableHeader(); for (auto &s : res) printStudentRow(s); }
        }
        else if (choice == 7) {
            string deptPart;
            cout << "Enter Department (partial or full): "; getline(cin, deptPart);
            auto res = store.searchByDepartment(deptPart);
            if (res.empty()) cout << "No students found.\n";
            else { printTableHeader(); for (auto &s : res) printStudentRow(s); }
        }
        else if (choice == 8) {
            int opt;
            cout << "1. Ascending  2. Descending: "; cin >> opt; cin.ignore();
            store.sortByGPA(opt != 1);
            cout << "Sorted by GPA.\n";
        }
        else if (choice == 9) {
            int opt;
            cout << "1. Ascending  2. Descending: "; cin >> opt; cin.ignore();
            store.sortByRoll(opt == 1);
            cout << "Sorted by Roll.\n";
        }
        else {
            cout << "Invalid option.\n";
        }

        pauseScreen();
    }

    cout << "Goodbye...G!\n";
    return 0;
}
