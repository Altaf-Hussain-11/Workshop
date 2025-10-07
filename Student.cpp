```cpp
// File: main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
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
        // TODO: implement CSV or other format for saving
        // Example placeholder (not safe for commas): name,roll,department,gpa,contact
        ostringstream oss;
        oss << name << ',' << roll << ',' << department << ',' << gpa << ',' << contact;
        return oss.str();
    }

    static Student deserialize(const string &line) {
        // TODO: implement robust parsing matching the chosen serialize format
        Student s;
        // placeholder simple parse (expects no commas in fields)
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
        // Optional: loadFromFile();
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
        // TODO: validate fields as needed
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
                // TODO: update selected fields instead of full replace if desired
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
        // TODO: replace with std::sort for efficiency; using simple lambda here
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
    store.loadFromFile();

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
            // TODO: collect inputs, validate, call addStudent
            cout << "[TODO] Add New Student - implement input collection and validation.\n";
        }
        else if (choice == 2) {
            // TODO: display all in tabular form
            cout << "[TODO] View All Students - implement display.\n";
        }
        else if (choice == 3) {
            // TODO: ask roll, call viewByRoll, show details
            cout << "[TODO] View Student by Roll - implement lookup and display.\n";
        }
        else if (choice == 4) {
            // TODO: update student fields
            cout << "[TODO] Update Student - implement editing flow.\n";
        }
        else if (choice == 5) {
            // TODO: delete by roll
            cout << "[TODO] Delete Student - implement deletion.\n";
        }
        else if (choice == 6) {
            // TODO: search by name and show results
            cout << "[TODO] Search by Name - implement search and display.\n";
        }
        else if (choice == 7) {
            // TODO: search by department and show results
            cout << "[TODO] Search by Department - implement search and display.\n";
        }
        else if (choice == 8) {
            // TODO: sort by GPA (ask asc/desc)
            cout << "[TODO] Sort by GPA - implement choice and sorting.\n";
        }
        else if (choice == 9) {
            // TODO: sort by Roll (ask asc/desc)
            cout << "[TODO] Sort by Roll - implement choice and sorting.\n";
        }
        else {
            cout << "Invalid option.\n";
        }

        pauseScreen();
    }

    cout << "Goodbye!\n";
    return 0;
}
```
