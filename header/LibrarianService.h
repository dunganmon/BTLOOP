#ifndef LIBRARIANSERVICE_H
#define LIBRARIANSERVICE_H

#include "Librarian.h"
#include "Book.h"
#include "DataManager.h"
#include <vector>

class LibrarianService {
public:
    void run(Librarian* librarian, vector<Person*>& users,vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager);

private:
    void addBook(vector<Book*>& books, DataManager& dataManager);
    void editBook(vector<Book*>& books);
    void searchBook(const vector<Book*>& books);
    void deleteBook(vector<Book*>& books, const vector<BorrowingRecord>& records);
    void reviewPendingRequests(vector<BorrowingRecord>& records, vector<Book*>& books);
    void collectFines(vector<Person*>& users);
    void changePassword(Librarian* librarian);
};

#endif