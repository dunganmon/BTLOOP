#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include "Student.h"
#include "Book.h"
#include "BorrowingRecord.h"
#include "DataManager.h"
#include <vector>

class StudentService {
public:
    void run(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager);

private:
    void viewMyInfo(Student* student, const vector<BorrowingRecord>& allRecords, const vector<Book*>& allBooks);
    void borrowBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager);
    void returnBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records);
    void reportLostBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records);
    int countMyBorrowedBooks(Student* student, const vector<BorrowingRecord>& allRecords);
    void cancelBorrowRequest(Student* student, vector<BorrowingRecord>& records);
    void changePassword(Student* student);
};

#endif 