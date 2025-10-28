#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int totalQuantity;
    int availableQuantity;

public:
    // Constructor
    Book(int id = 0, string title = "", string author = "", int total = 0, int available = 0)
        : id(id), title(title), author(author), totalQuantity(total), availableQuantity(available) {}

    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getTotalQuantity() const { return totalQuantity; }
    int getAvailableQuantity() const { return availableQuantity; }

    // Setters
    void setId(int id) { this->id = id; }
    void setTitle(const string& title) { this->title = title; }
    void setAuthor(const string& author) { this->author = author; }
    void setTotalQuantity(int total) { this->totalQuantity = total; }
    void setAvailableQuantity(int available) { this->availableQuantity = available; }

    // Các hàm nghiệp vụ
    // Trả về true nếu mượn thành công, false nếu hết sách
    bool borrowBook() {
        if (availableQuantity > 0) {
            availableQuantity--;
            return true;
        }
        return false;
    }

    // Trả sách
    void returnBook() {
        if (availableQuantity < totalQuantity) {
            availableQuantity++;
        }
    }

    // Hiển thị thông tin sách
    void display() const {
        cout << "| " << left << setw(8) << id
             << "| " << left << setw(35) << title
             << "| " << left << setw(25) << author
             << "| " << left << setw(10) << availableQuantity
             << "| " << left << setw(14) << totalQuantity << " |" << endl;
    }

    // Chuyển đổi thông tin sách thành chuỗi CSV để lưu file
    string toCSV() const {
        return to_string(id) + "," + title + "," + author + "," + to_string(totalQuantity) + "," + to_string(availableQuantity);
    }
};

#endif 