# Library Management System (C++)

A console-based library management system written in C++ using object-oriented programming.

## Features
- Add a new book (title and author, ID is generated automatically)
- Display all books with their status (Available or Issued)
- Search books by title or author (not case-sensitive)
- Issue a book to a member
- Return a book
- Delete a book (only if it is not issued)
- Data is saved in `books.txt`, so nothing is lost when the program closes

## Concepts used
- Classes and objects (`Book`, `Library`)
- Encapsulation (private data with getters)
- Constructors
- Vectors (STL)
- File handling (`ifstream`, `ofstream`)
- Input validation

## How to run
```
g++ main.cpp -o library
./library
```
On Windows, run `library.exe` after compiling.
