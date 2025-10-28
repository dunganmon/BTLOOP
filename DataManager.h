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

    // Các biến này quản lý ID tiếp theo sẽ được gán cho đối tượng mới.
    // Đây là giải pháp thay thế cho việc dùng 'static' bị cấm trong yêu cầu.
    int nextUserId;
    int nextBookId;
    int nextRecordId;

    // Hàm private để tính toán ID tiếp theo sau khi tải dữ liệu
    void findNextIds(const vector<Person*>& users, const vector<Book>& books, const vector<BorrowingRecord>& records);

public:
    DataManager(string uFile, string bFile, string rFile);

    // Tải tất cả dữ liệu từ file vào các vector
    void loadAllData(vector<Person*>& users, vector<Book>& books, vector<BorrowingRecord>& records);

    // Lưu tất cả dữ liệu từ các vector vào file
    void saveAllData(const vector<Person*>& users, const vector<Book>& books, const vector<BorrowingRecord>& records);

    // Cung cấp ID duy nhất cho đối tượng mới
    int getNextUserId();
    int getNextBookId();
    int getNextRecordId();
};

#endif 