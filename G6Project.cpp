#include <algorithm> // Needed for transform
#include <cctype>    // Needed for tolower
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

// Configurations
string TEACHER_PIN = "654321"; // Teacher PIN (changeable)
int numHalls = 5;
int seatsPerHall = 10;

// Structure to store student information
struct Student {
  string id;
  string name;
  string department;
  int hall;
  int seat;
};

vector<Student> students; // List of all students

// Utility Functions

// Check if string is exactly 6 digits
bool isSixDigitNumber(const string &s) {
  if (s.length() != 6)
    return false;
  for (char c : s)
    if (!isdigit(c))
      return false;
  return true;
}

// Get a valid student ID in format "etsXXXXXX"
string getValidETSID() {
  string id;
  while (true) {
    cout << "\nEnter Student ID (ets + 6 digits): ";
    cin >> id;

    // 1. Get the first 3 characters
    string prefix = id.substr(0, 3);

    // 2. Convert that prefix to lowercase
    transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);

    // 3. Perform the check
    if (id.length() != 9 || prefix != "ets") {
      cout << "Invalid format! ID must be 9 characters long and start with "
              "'ets'\n";
      continue;
    }

    bool ok = true;
    for (int i = 3; i < 9; i++)
      if (!isdigit(id[i]))
        ok = false;

    if (!ok) {
      cout << "Last 6 characters must be digits only\n";
      continue;
    }

    return id;
  }
}

// Get a valid 6-digit PIN
string getSixDigitPIN() {
  string pin;
  while (true) {
    cout << "\nEnter 6-digit PIN: ";
    cin >> pin;

    if (!isSixDigitNumber(pin)) {
      cout << "PIN must be exactly 6 digits\n";
      continue;
    }
    return pin;
  }
}

// Get full name of student
string getFullName() {
  string name;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << "\nEnter Full Name: ";
  getline(cin, name);

  while (name.length() < 3) {
    cout << "Invalid name. Enter again: ";
    getline(cin, name);
  }
  return name;
}

// Get department and validate
string getDepartment() {
  string dept;
  cout << "\nDepartment (SE / EE / CV): ";
  cin >> dept;

  while (dept != "SE" && dept != "EE" && dept != "CV") {
    cout << "Invalid department. Enter again: ";
    cin >> dept;
  }
  return dept;
}

// ======== Login Functions ========

// Teacher login with 3 attempts
bool teacherLogin() {
  string pin;
  int attempts = 3;

  while (attempts--) {
    pin = getSixDigitPIN();

    if (pin == TEACHER_PIN) {
      cout << "\nTeacher login successful\n\n";
      return true;
    } else {
      cout << "Wrong PIN. Attempts left: " << attempts << "\n\n";
    }
  }

  cout << "Login blocked. Too many failed attempts.\n\n";
  return false;
}

// Student login by ID with 3 attempts
int studentLogin() {
  string id;
  int attempts = 3;

  while (attempts--) {
    id = getValidETSID();

    for (size_t i = 0; i < students.size(); i++) {
      if (students[i].id == id) {
        cout << "\nStudent login successful\n\n";
        return i;
      }
    }

    cout << "Student not found. Attempts left: " << attempts << "\n\n";
  }
  return -1;
}

// ======== Teacher Functionalities ========

// Register a new student
void registerStudent() {
  Student s;
  s.id = getValidETSID();

  for (auto &stu : students) {
    if (stu.id == s.id) {
      cout << "\nError: Student with this ID already exists!\n\n";
      return;
    }
  }

  s.name = getFullName();
  s.department = getDepartment();
  s.hall = -1;
  s.seat = -1;

  students.push_back(s);
  cout << "\nStudent registered successfully\n\n";
}

// Update existing student details
void updateStudent() {
  string id;
  cout << "\nEnter ID of student to update: ";
  id = getValidETSID();

  for (auto &s : students) {
    if (s.id == id) {
      cout << "Updating student: " << s.name << "\n";
      s.name = getFullName();
      s.department = getDepartment();
      cout << "\nStudent updated successfully\n\n";
      return;
    }
  }
  cout << "\nStudent not found\n\n";
}

// Delete a student
void deleteStudent() {
  string id;
  cout << "\nEnter ID of student to delete: ";
  id = getValidETSID();

  for (size_t i = 0; i < students.size(); i++) {
    if (students[i].id == id) {
      students.erase(students.begin() + i);
      cout << "\nStudent deleted successfully\n\n";
      return;
    }
  }
  cout << "\nStudent not found\n\n";
}

// Search for a student by ID
void searchStudent() {
  string id;
  cout << "\nEnter ID of student to search: ";
  id = getValidETSID();

  for (auto &s : students) {
    if (s.id == id) {
      cout << "Name: " << s.name << ", Department: " << s.department;
      if (s.hall != -1)
        cout << ", Hall: " << s.hall << ", Seat: " << s.seat;
      cout << "\n\n";
      return;
    }
  }
  cout << "\nStudent not found\n\n";
}

// Configure number of halls and seats
void configureHalls() {
  cout << "\nEnter number of halls: ";
  cin >> numHalls;
  cout << "Enter number of seats per hall: ";
  cin >> seatsPerHall;
  cout << "\nConfiguration updated\n\n";
}

// Allocate seats sequentially by department
void allocateDepartmentWise() {
  int hall = 1, seat = 1;

  for (auto &s : students) {
    s.hall = hall;
    s.seat = seat++;
    if (seat > seatsPerHall) {
      seat = 1;
      hall++;
    }
    if (hall > numHalls) {
      cout << "\nWarning: Not enough halls for all students!\n\n";
      break;
    }
  }
  cout << "\nDepartment-wise allocation done\n\n";
}

// Allocate seats randomly without conflicts
void allocateRandom() {
  if (students.empty())
    return;

  srand(time(0));

  vector<vector<bool>> taken(numHalls + 1,
                             vector<bool>(seatsPerHall + 1, false));

  for (auto &s : students) {
    int h, se;
    int attempts = 0;
    do {
      h = rand() % numHalls + 1;
      se = rand() % seatsPerHall + 1;
      attempts++;
      if (attempts > 1000) {
        cout << "\nNot enough seats to allocate randomly.\n\n";
        s.hall = -1;
        s.seat = -1;
        break;
      }
    } while (taken[h][se]);

    if (attempts <= 1000) {
      s.hall = h;
      s.seat = se;
      taken[h][se] = true;
    }
  }
  cout << "\nRandom allocation done with no conflicts!\n\n";
}

// Reset system (delete all students)
void resetSystem() {
  students.clear();
  cout << "\nSystem reset complete. All students removed.\n\n";
}

// View all registered students
void viewAllStudents() {
  if (students.empty()) {
    cout << "\nNo students registered yet\n\n";
    return;
  }

  for (auto &s : students) {
    cout << "ID: " << s.id << ", Name: " << s.name
         << ", Dept: " << s.department;
    if (s.hall != -1)
      cout << ", Hall: " << s.hall << ", Seat: " << s.seat;
    cout << "\n";
  }
  cout << "\n";
}

// Teacher menu interface
void teacherMenu() {
  int ch;
  while (true) {
    cout << "Teacher Menu:\n";
    cout << "1. Register Student\n";
    cout << "2. Update Student\n";
    cout << "3. Delete Student\n";
    cout << "4. Search Student\n";
    cout << "5. Configure Halls\n";
    cout << "6. Department-wise Allocation\n";
    cout << "7. Random Allocation\n";
    cout << "8. View All Students\n";
    cout << "9. Reset System\n";
    cout << "10. Logout\n";
    cout << "Choice: ";
    cin >> ch;

    switch (ch) {
    case 1:
      registerStudent();
      break;
    case 2:
      updateStudent();
      break;
    case 3:
      deleteStudent();
      break;
    case 4:
      searchStudent();
      break;
    case 5:
      configureHalls();
      break;
    case 6:
      allocateDepartmentWise();
      break;
    case 7:
      allocateRandom();
      break;
    case 8:
      viewAllStudents();
      break;
    case 9:
      resetSystem();
      break;
    case 10:
      return;
    default:
      cout << "\nInvalid choice\n\n";
    }
  }
}

// Student dashboard interface
void studentMenu(int index) {
  auto &s = students[index];
  cout << "Student Dashboard:\n";
  cout << "Name: " << s.name << "\n";
  cout << "ID: " << s.id << "\n";
  cout << "Department: " << s.department << "\n";

  if (s.hall == -1)
    cout << "Seat not allocated yet\n\n";
  else
    cout << "Hall: " << s.hall << ", Seat: " << s.seat << "\n\n";
}

// Main program
int main() {
  int choice;

  while (true) {
    cout << "Exam Hall Seat Allocation System\n";
    cout << "1. Teacher Login (PIN only)\n";
    cout << "2. Student Login (ID etsXXXXXX)\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "\nEnter numbers only\n\n";
      continue;
    }

    switch (choice) {
    case 1:
      if (teacherLogin())
        teacherMenu();
      break;
    case 2: {
      int idx = studentLogin();
      if (idx != -1)
        studentMenu(idx);
      break;
    }
    case 3:
      cout << "\nGoodbye!\n";
      return 0;
    default:
      cout << "\nInvalid choice\n\n";
    }
  }
}
