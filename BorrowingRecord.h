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
    string returnDate;
    int status; // 0: Dang muon, 1: Da tra, 2: Lam mat

public:
    BorrowingRecord(int recId = 0, int bId = 0, int sId = 0, string bDate = "", string rDate = "N/A", int st = 0)
        : recordId(recId), bookId(bId), studentId(sId), borrowDate(bDate), returnDate(rDate), status(st) {}

    int getRecordId() const { return recordId; }
    int getBookId() const { return bookId; }
    int getStudentId() const { return studentId; }
    string getBorrowDate() const { return borrowDate; }
    string getReturnDate() const { return returnDate; }
    int getStatus() const { return status; }

    void setReturnDate(const string& date) { this->returnDate = date; }
    void setStatus(int newStatus) { this->status = newStatus; }

    // Chuyển đổi thành chuỗi CSV
    string toCSV() const {
        return to_string(recordId) + "," + to_string(bookId) + "," + to_string(studentId) + "," +
               borrowDate + "," + returnDate + "," + to_string(status);
    }
};

#endif 