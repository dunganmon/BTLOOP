#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "Person.h"
#include "Book.h"
#include "BorrowingRecord.h"

class DataManager {
private:
    string usersFile;
    string booksFile;
    string recordsFile;

    int nextUserId;
    int nextBookId;
    int nextRecordId;
   
    void findNextIds(const vector<Person*>& users, const vector<Book*>& books, const vector<BorrowingRecord>& records);

public:
    DataManager(string uFile, string bFile, string rFile);

    void loadAllData(vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records);

    void saveAllData(const vector<Person*>& users, const vector<Book*>& books, const vector<BorrowingRecord>& records);

    int getNextUserId();
    int getNextBookId();
    int getNextRecordId();
     bool importBooksFromFile(const string& filename, vector<Book*>& books);
    bool importUsersFromFile(const string& filename, vector<Person*>& users);
};

#endif 