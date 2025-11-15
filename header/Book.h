#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Book {
protected:
    int id;
    string title;
    string author;
    int totalQuantity;
    int availableQuantity;
    int publicationYear; 
    int bookType; //(1: Báo, 2: Tạp chí, 3: Giáo trình, 4: Tiểu thuyết)

public:
    Book(int id = 0, string title = "", string author = "", int total = 0, int available = 0, int year = 0, int type = 0)
        : id(id), title(title), author(author), totalQuantity(total), availableQuantity(available), publicationYear(year), bookType(type) {}

    virtual ~Book() {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getTotalQuantity() const { return totalQuantity; }
    int getAvailableQuantity() const { return availableQuantity; }
    int getPublicationYear() const { return publicationYear; }
    int getBookType() const { return bookType; }

    void setId(int id) { this->id = id; }
    void setTitle(const string& title) { this->title = title; }
    void setAuthor(const string& author) { this->author = author; }
    void setTotalQuantity(int total) { this->totalQuantity = total; }
    void setAvailableQuantity(int available) { this->availableQuantity = available; }
    void setPublicationYear(int year) { this->publicationYear = year; }

    bool borrowBook() {
        if (availableQuantity > 0) {
            availableQuantity--;
            return true;
        }
        return false;
    }

    void returnBook() {
        if (availableQuantity < totalQuantity) {
            availableQuantity++;
        }
    }

    virtual string getBookTypeString() const = 0;

    virtual void display() const = 0;

    virtual string toCSV() const {
        return to_string(id) + "," + title + "," + author + "," + 
               to_string(publicationYear) + "," + 
               to_string(availableQuantity) + "," + 
               to_string(totalQuantity); 
    }
};

#endif