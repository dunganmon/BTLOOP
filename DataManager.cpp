#include "DataManager.h"
#include "Admin.h"
#include "Librarian.h"
#include "Student.h"
#include <fstream>
#include <sstream>
#include <algorithm> // for max

DataManager::DataManager(string uFile, string bFile, string rFile)
    : usersFile(uFile), booksFile(bFile), recordsFile(rFile),
      nextUserId(1), nextBookId(1), nextRecordId(1) {}

void DataManager::findNextIds(const vector<Person*>& users, const vector<Book>& books, const vector<BorrowingRecord>& records) {
    int maxUserId = 0;
    for (const auto& user : users) {
        if (user->getId() > maxUserId) {
            maxUserId = user->getId();
        }
    }
    nextUserId = maxUserId + 1;

    int maxBookId = 0;
    for (const auto& book : books) {
        if (book.getId() > maxBookId) {
            maxBookId = book.getId();
        }
    }
    nextBookId = maxBookId + 1;

    int maxRecordId = 0;
    for (const auto& record : records) {
        if (record.getRecordId() > maxRecordId) {
            maxRecordId = record.getRecordId();
        }
    }
    nextRecordId = maxRecordId + 1;
}

int DataManager::getNextUserId() {
    return nextUserId++;
}

int DataManager::getNextBookId() {
    return nextBookId++;
}

int DataManager::getNextRecordId() {
    return nextRecordId++;
}

void DataManager::loadAllData(vector<Person*>& users, vector<Book>& books, vector<BorrowingRecord>& records) {
    ifstream userFileStream(usersFile);
    string line;

    // ===== Load Users =====
    while (getline(userFileStream, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() < 5) continue; // ít nhất phải có 5 trường

        int role = stoi(tokens[0]);
        int id = stoi(tokens[1]);
        string name = tokens[2];
        string email = tokens[3];
        string password = tokens[4];

        if (role == 1) { // Admin
            users.push_back(new Admin(id, name, email, password));
        } 
        else if (role == 2 && tokens.size() >= 6) { // Librarian
            string employeeId = tokens[5];
            users.push_back(new Librarian(id, name, email, password, employeeId));
        } 
        else if (role == 3 && tokens.size() >= 9) { // Student
            string studentId = tokens[5];
            string studentClass = tokens[6];
            int courseYear = stoi(tokens[7]);
            double fineAmount = stod(tokens[8]);
            users.push_back(new Student(id, name, email, password, studentId, studentClass, courseYear, fineAmount));
        }
    }
    userFileStream.close();

    // ===== Load Books =====
    ifstream bookFileStream(booksFile);
    while (getline(bookFileStream, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() == 5) {
            int id = stoi(tokens[0]);
            string title = tokens[1];
            string author = tokens[2];
            int year = stoi(tokens[3]);
            int quantity = stoi(tokens[4]);
            books.push_back(Book(id, title, author, year, quantity));
        }
    }
    bookFileStream.close();

    // ===== Load Records =====
    ifstream recordFileStream(recordsFile);
    while (getline(recordFileStream, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() == 6) {
            int recordId = stoi(tokens[0]);
            int userId = stoi(tokens[1]);
            int bookId = stoi(tokens[2]);
            string borrowDate = tokens[3];
            string returnDate = tokens[4];
            int status = stoi(tokens[5]);
            records.push_back(BorrowingRecord(recordId, userId, bookId, borrowDate, returnDate, status));
        }
    }
    recordFileStream.close();

    // Cập nhật ID tiếp theo
    findNextIds(users, books, records);
}

void DataManager::saveAllData(const vector<Person*>& users, const vector<Book>& books, const vector<BorrowingRecord>& records) {
    // Save Users
    ofstream userFileStream(usersFile);
    for (const auto& user : users) {
        userFileStream << user->toCSV() << endl;
    }
    userFileStream.close();

    // Save Books
    ofstream bookFileStream(booksFile);
    for (const auto& book : books) {
        bookFileStream << book.toCSV() << endl;
    }
    bookFileStream.close();

    // Save Records
    ofstream recordFileStream(recordsFile);
    for (const auto& record : records) {
        recordFileStream << record.toCSV() << endl;
    }
    recordFileStream.close();
}
