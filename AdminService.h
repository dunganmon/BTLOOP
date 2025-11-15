#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "Admin.h"
#include "Librarian.h"
#include "Student.h"
#include "Book.h"
#include "DataManager.h"
#include <vector>

class AdminService {
public:
    void run(Admin* admin, vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager);

private:
    void addLibrarian(vector<Person*>& users, DataManager& dataManager);
    void viewAllUsers(const vector<Person*>& users);
    void deleteUser(vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records);
    void addBook(vector<Book*>& books, DataManager& dataManager);
    void editBook(vector<Book*>& books);
    void deleteBook(vector<Book*>& books, const vector<BorrowingRecord>& records);
    void importBooks(vector<Book*>& books, DataManager& dataManager);
    void importUsers(vector<Person*>& users, DataManager& dataManager);
};

#endif 