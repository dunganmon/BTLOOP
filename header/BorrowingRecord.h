#ifndef BORROWINGRECORD_H
#define BORROWINGRECORD_H

#include <string>

using namespace std;

class BorrowingRecord {
private:
    int recordId;
    int bookId;
    int studentId;
    string borrowDate;
    string dueDate;
    string returnDate;
    int status; // 0: Cho duyet, 1: Dang muon, 2: Da tra, 3: Lam mat

public:
    BorrowingRecord(int recId = 0, int bId = 0, int sId = 0, string bDate = "", string dDate = "", string rDate = "N/A", int st = 0)
        : recordId(recId), bookId(bId), studentId(sId), 
        borrowDate(bDate), dueDate(dDate), returnDate(rDate), status(st) {}

    int getRecordId() const { return recordId; }
    int getBookId() const { return bookId; }
    int getStudentId() const { return studentId; }
    string getBorrowDate() const { return borrowDate; }
    string getDueDate() const { return dueDate; }
    string getReturnDate() const { return returnDate; }
    int getStatus() const { return status; }
    void setDueDate(const string& date) { this->dueDate = date; }
    void setReturnDate(const string& date) { this->returnDate = date; }
    void setStatus(int newStatus) { this->status = newStatus; }
    void setBorrowDate(const string& date) { this->borrowDate = date; }

    // Chuyển đổi thành chuỗi CSV
    string toCSV() const {
        return to_string(recordId) + "," + to_string(bookId) + "," + to_string(studentId) + "," +
               borrowDate + "," + dueDate + "," + returnDate + "," + to_string(status);
    }
};

#endif 