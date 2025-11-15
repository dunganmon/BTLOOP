#ifndef NEWSPAPER_H
#define NEWSPAPER_H

#include "Book.h"

class Newspaper : public Book {
public:
    Newspaper(int id = 0, string title = "", string author = "", int total = 0, int available = 0, int year = 0)
        : Book(id, title, author, total, available, year, 1) {} 

    virtual string getBookTypeString() const override {
        return "Bao";
    }

    void display() const override {
        cout << "| " << left << setw(12) << getBookTypeString()
             << "| " << left << setw(8) << id
             << "| " << left << setw(35) << title
             << "| " << left << setw(25) << author
             << "| " << left << setw(5) << publicationYear
             << "| " << left << setw(10) << availableQuantity
             << "| " << left << setw(14) << totalQuantity << " |" << endl;
    }

    string toCSV() const override {
        return to_string(bookType) + "," + Book::toCSV();
    }
};

#endif