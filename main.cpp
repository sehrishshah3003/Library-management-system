// Library Management System
// A console-based program in C++ that uses Object-Oriented Programming.
// Features: add, display, search, issue, return and delete books.
// Data is saved in "books.txt", so it is not lost when the program closes.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

// ---------- Helper functions ----------

// Convert text to lowercase (used for case-insensitive search)
string toLower(string text) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}

// Show a message and read a full line of text
string readLine(const string& message) {
    string line;
    cout << message;
    getline(cin, line);
    return line;
}

// Show a message and read a valid whole number
int readInt(const string& message) {
    while (true) {
        string line = readLine(message);
        try {
            size_t used;
            int value = stoi(line, &used);
            if (used == line.size()) return value;
        } catch (...) {}
        cout << "Please enter a valid number.\n";
    }
}

// ---------- Book class ----------
// Stores the information of one book.
class Book {
private:
    int id;
    string title;
    string author;
    bool issued;
    string issuedTo;   // name of the member who has the book

public:
    Book(int id, const string& title, const string& author,
         bool issued = false, const string& issuedTo = "")
        : id(id), title(title), author(author), issued(issued), issuedTo(issuedTo) {}

    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }
    string getIssuedTo() const { return issuedTo; }

    // Actions
    void issueTo(const string& memberName) { issued = true; issuedTo = memberName; }
    void giveBack() { issued = false; issuedTo = ""; }

    void display() const {
        cout << "ID: " << id
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (issued ? "Issued to " + issuedTo : "Available")
             << "\n";
    }

    // Convert the book to one line of text for saving in the file
    string toFileLine() const {
        return to_string(id) + "|" + title + "|" + author + "|" +
               (issued ? "1" : "0") + "|" + issuedTo;
    }
};

// ---------- Library class ----------
// Manages the whole collection of books.
class Library {
private:
    vector<Book> books;
    string fileName;
    int nextId;

    // Find a book by its ID. Returns -1 if not found.
    int findIndex(int id) const {
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].getId() == id) return (int)i;
        return -1;
    }

    void loadFromFile() {
        ifstream file(fileName);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idText, title, author, issuedText, issuedTo;
            getline(ss, idText, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, issuedText, '|');
            getline(ss, issuedTo, '|');
            if (idText.empty()) continue;
            int id = stoi(idText);
            books.push_back(Book(id, title, author, issuedText == "1", issuedTo));
            if (id >= nextId) nextId = id + 1;
        }
    }

    void saveToFile() const {
        ofstream file(fileName);
        for (const Book& b : books) file << b.toFileLine() << "\n";
    }

public:
    Library(const string& fileName) : fileName(fileName), nextId(1) {
        loadFromFile();
    }

    void addBook() {
        string title = readLine("Enter book title: ");
        string author = readLine("Enter author name: ");
        if (title.empty() || author.empty()) {
            cout << "Title and author cannot be empty.\n";
            return;
        }
        books.push_back(Book(nextId, title, author));
        cout << "Book added successfully with ID " << nextId << ".\n";
        nextId++;
        saveToFile();
    }

    void displayAll() const {
        if (books.empty()) {
            cout << "The library has no books yet.\n";
            return;
        }
        cout << "\n--- All Books (" << books.size() << ") ---\n";
        for (const Book& b : books) b.display();
    }

    void searchBook() const {
        string keyword = toLower(readLine("Enter title or author to search: "));
        bool found = false;
        for (const Book& b : books) {
            if (toLower(b.getTitle()).find(keyword) != string::npos ||
                toLower(b.getAuthor()).find(keyword) != string::npos) {
                b.display();
                found = true;
            }
        }
        if (!found) cout << "No book found.\n";
    }

    void issueBook() {
        int id = readInt("Enter ID of the book to issue: ");
        int index = findIndex(id);
        if (index == -1) { cout << "Book not found.\n"; return; }
        if (books[index].isIssued()) {
            cout << "Sorry, this book is already issued to "
                 << books[index].getIssuedTo() << ".\n";
            return;
        }
        string member = readLine("Enter member name: ");
        if (member.empty()) { cout << "Member name cannot be empty.\n"; return; }
        books[index].issueTo(member);
        saveToFile();
        cout << "Book \"" << books[index].getTitle() << "\" issued to " << member << ".\n";
    }

    void returnBook() {
        int id = readInt("Enter ID of the book to return: ");
        int index = findIndex(id);
        if (index == -1) { cout << "Book not found.\n"; return; }
        if (!books[index].isIssued()) {
            cout << "This book was not issued.\n";
            return;
        }
        books[index].giveBack();
        saveToFile();
        cout << "Book \"" << books[index].getTitle() << "\" returned successfully.\n";
    }

    void deleteBook() {
        int id = readInt("Enter ID of the book to delete: ");
        int index = findIndex(id);
        if (index == -1) { cout << "Book not found.\n"; return; }
        if (books[index].isIssued()) {
            cout << "This book is currently issued. Return it first.\n";
            return;
        }
        books.erase(books.begin() + index);
        saveToFile();
        cout << "Book deleted successfully.\n";
    }
};

// ---------- Main program ----------
void showMenu() {
    cout << "\n===== Library Management System =====\n"
         << "1. Add a book\n"
         << "2. Display all books\n"
         << "3. Search a book\n"
         << "4. Issue a book\n"
         << "5. Return a book\n"
         << "6. Delete a book\n"
         << "0. Exit\n";
}

int main() {
    Library library("books.txt");
    int choice;

    do {
        showMenu();
        choice = readInt("Enter your choice: ");
        switch (choice) {
            case 1: library.addBook(); break;
            case 2: library.displayAll(); break;
            case 3: library.searchBook(); break;
            case 4: library.issueBook(); break;
            case 5: library.returnBook(); break;
            case 6: library.deleteBook(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
