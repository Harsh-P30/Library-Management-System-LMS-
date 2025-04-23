#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include<algorithm>
using namespace std;

const int MIN_DEPOSIT = 50;
const int MAX_BOOKS = 15;

struct Book {
    string title;
    string author;
    int isbn;
    bool isAvailable = true;
};

struct Student {
    int roll;
    string name;
    double balance = 0.0;
};

vector<Book> books;
vector<Student> students;

int findStudent(int roll) {
    for (int i = 0; i < students.size(); i++) {
        if (students[i].roll == roll) return i;
    }
    return -1;
}

int findBook(int isbn) {
    for (int i = 0; i < books.size(); i++) {
        if (books[i].isbn == isbn) return i;
    }
    return -1;
}

void createStudent() {
    Student s;
    cout << "Enter roll number: ";
    cin >> s.roll;

    if (findStudent(s.roll) != -1) {
        cout << "Student already exists!\n";
        return;
    }

    cout << "Enter name: ";
    cin.ignore();
    getline(cin, s.name);

    cout << "Enter initial deposit (min $50): ";
    cin >> s.balance;

    if (s.balance < MIN_DEPOSIT) {
        cout << "Insufficient initial deposit.\n";
        return;
    }

    s.balance -= 20 + 30; // Security + account fees
    students.push_back(s);
    cout << "Account created successfully.\n";
}

void showStudent(int roll) {
    int idx = findStudent(roll);
    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }
    Student& s = students[idx];
    cout << "Roll: " << s.roll << "\nName: " << s.name << "\nBalance: $" << fixed << setprecision(2) << s.balance << "\n";
}

void deposit(int roll) {
    int idx = findStudent(roll);
    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }
    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;
    students[idx].balance += amount;
    cout << "New balance: $" << students[idx].balance << "\n";
}

void issueBook(int roll) {
    int sIdx = findStudent(roll);
    if (sIdx == -1) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Available books:\n";
    for (int i = 0; i < books.size(); i++) {
        if (books[i].isAvailable)
            cout << i + 1 << ". " << books[i].title << " by " << books[i].author << " (ISBN: " << books[i].isbn << ")\n";
    }

    int choice;
    cout << "Select book number (0 to cancel): ";
    cin >> choice;

    if (choice <= 0 || choice > books.size() || !books[choice - 1].isAvailable) {
        cout << "Invalid choice or book unavailable.\n";
        return;
    }

    if (students[sIdx].balance < 2) {
        cout << "Insufficient balance to issue book.\n";
        return;
    }

    books[choice - 1].isAvailable = false;
    students[sIdx].balance -= 2;
    cout << "Book issued successfully.\n";
}

void addBook() {
    if (books.size() >= MAX_BOOKS) {
        cout << "Book limit reached.\n";
        return;
    }

    Book b;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, b.title);
    cout << "Enter book author: ";
    getline(cin, b.author);
    cout << "Enter ISBN: ";
    cin >> b.isbn;

    if (findBook(b.isbn) != -1) {
        cout << "Book already exists.\n";
        return;
    }

    books.push_back(b);
    cout << "Book added successfully.\n";
}

void editBook() {
    int isbn;
    cout << "Enter ISBN to edit: ";
    cin >> isbn;
    int idx = findBook(isbn);
    if (idx == -1) {
        cout << "Book not found.\n";
        return;
    }

    cout << "Enter new title: ";
    cin.ignore();
    getline(cin, books[idx].title);
    cout << "Enter new author: ";
    getline(cin, books[idx].author);
    cout << "Book updated successfully.\n";
}

void viewBooks() {
    for (const auto& book : books) {
        cout << "Title: " << book.title << "\nAuthor: " << book.author << "\nISBN: " << book.isbn
             << "\nAvailable: " << (book.isAvailable ? "Yes" : "No") << "\n\n";
    }
}

void showAllStudentsSorted() {
    vector<Student> sorted = students;
    sort(sorted.begin(), sorted.end(), [](Student& a, Student& b) {
        return a.roll < b.roll;
    });

    for (const auto& s : sorted) {
        cout << s.roll << " - " << s.name << " - Balance: $" << fixed << setprecision(2) << s.balance << "\n";
    }
}

int main() {
    int choice;
    string password;

    // Sample books
    for (int i = 0; i < MAX_BOOKS; i++) {
        books.push_back({"Book" + to_string(i + 1), "Author" + to_string(i + 1), 1000 + i, true});
    }

    while (true) {
        cout << "\n1. Admin\n2. Student\n0. Exit\nEnter your role: ";
        cin >> choice;
        if (choice == 0) break;

        cout << "Enter password: ";
        cin >> password;
        if (password != "password") {
            cout << "Access denied.\n";
            continue;
        }

        if (choice == 1) {
            int op;
            cout << "\n1. Add Book\n2. Edit Book\n3. View Books\n4. Show Sorted Students\n5. Show Student by Roll\nChoice: ";
            cin >> op;
            switch (op) {
                case 1: addBook(); break;
                case 2: editBook(); break;
                case 3: viewBooks(); break;
                case 4: showAllStudentsSorted(); break;
                case 5: {
                    int r; cout << "Enter roll: "; cin >> r;
                    showStudent(r);
                    break;
                }
                default: cout << "Invalid option.\n";
            }
        } else {
            int roll;
            cout << "Enter roll number: ";
            cin >> roll;

            int idx = findStudent(roll);
            if (idx == -1) {
                cout << "1. Create Account\n2. Cancel\nChoice: ";
                int op; cin >> op;
                if (op == 1) createStudent();
            } else {
                int op;
                cout << "1. View Details\n2. Deposit\n3. Issue Book\nChoice: ";
                cin >> op;
                switch (op) {
                    case 1: showStudent(roll); break;
                    case 2: deposit(roll); break;
                    case 3: issueBook(roll); break;
                    default: cout << "Invalid option.\n";
                }
            }
        }
    }

    return 0;
}
