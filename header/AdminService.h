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
    void updateUserInfo(vector<Person*>& users);
    bool isStudentIdTaken(const vector<Person*>& users, const string& studentId, int currentUserId);
    bool isEmployeeIdTaken(const vector<Person*>& users, const string& employeeId, int currentUserId);
    void importBooks(vector<Book*>& books, DataManager& dataManager);
    void importUsers(vector<Person*>& users, DataManager& dataManager);
    void changePassword(Admin* admin);

};

#endif 