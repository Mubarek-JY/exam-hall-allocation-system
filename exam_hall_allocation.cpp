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

// Fixed array for something (array example)
const int MAX_LOG_ATTEMPTS = 50;
string loginAttempts[MAX_LOG_ATTEMPTS]; // Fixed size array
int attemptCount = 0;

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

// make string lowercase for case insensitive
string makeLower(const string &input) {
  string result = input;
  for (char &c : result) {
    c = tolower(c);
  }
  return result;
}

// Get a valid student ID in format "etsXXXXXX" or "ETSXXXXXX"
string getValidETSID() {
  string id;
  while (true) {
    cout << "\nEnter Student ID (ets/ETS + 6 digits): ";
    cin >> id;

    if (id.length() != 9) {
      cout << "Invalid format! Must be 9 characters\n";
      continue;
    }

    // Check first 3 chars (ets or ETS)
    string firstThree = id.substr(0, 3);
    string lowerFirstThree = makeLower(firstThree);

    if (lowerFirstThree != "ets") {
      cout << "Invalid! Must start with 'ets' or 'ETS'\n";
      continue;
    }

    // Check last 6 are digits
    bool allDigits = true;
    for (int i = 3; i < 9; i++) {
      if (!isdigit(id[i]))
        allDigits = false;
    }

    if (!allDigits) {
      cout << "Last 6 characters must be digits only\n";
      continue;
    }

    // Return in lowercase format for consistency
    return "ets" + id.substr(3);
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

  // Convert to uppercase
  for (char &c : dept) {
    c = toupper(c);
  }

  while (dept != "SE" && dept != "EE" && dept != "CV") {
    cout << "Invalid department. Enter again: ";
    cin >> dept;
    for (char &c : dept) {
      c = toupper(c);
    }
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

    // Store attempt in array (array usage)
    if (attemptCount < MAX_LOG_ATTEMPTS) {
      loginAttempts[attemptCount] = "PIN tried: " + pin;
      attemptCount++;
    }

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

    string searchID = makeLower(id); // case insensitive search

    for (size_t i = 0; i < students.size(); i++) {
      if (makeLower(students[i].id) == searchID) {
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

  // Check if exists (case insensitive)
  string newIDLower = makeLower(s.id);
  for (auto &stu : students) {
    if (makeLower(stu.id) == newIDLower) {
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

  string searchID = makeLower(id);
  for (auto &s : students) {
    if (makeLower(s.id) == searchID) {
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

  string searchID = makeLower(id);
  for (size_t i = 0; i < students.size(); i++) {
    if (makeLower(students[i].id) == searchID) {
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

  if (students.empty()) {
    cout << "\nNo students registered.\n\n";
    return;
  }

  // Using pointer to access vector elements (pointer example)
  Student *studentPtr = students.data(); // pointer to first element

  for (size_t i = 0; i < students.size(); i++) {
    // Pointer arithmetic: studentPtr + i
    if (makeLower((studentPtr + i)->id) == makeLower(id)) {
      cout << "Name: " << (studentPtr + i)->name
           << ", Department: " << (studentPtr + i)->department;
      if ((studentPtr + i)->hall != -1)
        cout << ", Hall: " << (studentPtr + i)->hall
             << ", Seat: " << (studentPtr + i)->seat;
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

  // Dynamic 2D array using pointers (pointer + array example)
  bool **seatTaken = new bool *[numHalls + 1]; // array of pointers

  for (int hallNum = 0; hallNum <= numHalls; hallNum++) {
    seatTaken[hallNum] =
        new bool[seatsPerHall + 1]; // each pointer points to array
    for (int seatNum = 0; seatNum <= seatsPerHall; seatNum++) {
      seatTaken[hallNum][seatNum] = false; // initialize
    }
  }

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
    } while (seatTaken[h][se]);

    if (attempts <= 1000) {
      s.hall = h;
      s.seat = se;
      seatTaken[h][se] = true;
    }
  }

  // Free memory
  for (int i = 0; i <= numHalls; i++) {
    delete[] seatTaken[i];
  }
  delete[] seatTaken;

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
    cout << "2. Student Login (ID etsXXXXXX or ETSXXXXXX)\n";
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