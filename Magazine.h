#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "Book.h"

class Magazine : public Book {
public:
    Magazine(int id = 0, string title = "", string author = "", int total = 0, int available = 0, int year = 0)
        : Book(id, title, author, total, available, year, 2) {} 

    virtual string getBookTypeString() const override {
        return "Tap chi";
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