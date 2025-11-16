#include "DataManager.h"
#include "Admin.h"
#include "Librarian.h"
#include "Student.h"
#include "Newspaper.h"
#include "Magazine.h"
#include "Textbook.h"
#include "Novel.h"
#include "Menu.h"
#include <fstream>
#include <sstream>
#include <algorithm> 

DataManager::DataManager(string uFile, string bFile, string rFile)
    : usersFile(uFile), booksFile(bFile), recordsFile(rFile),
      nextUserId(1), nextBookId(1), nextRecordId(1) {}

void DataManager::findNextIds(const vector<Person*>& users, const vector<Book*>& books, const vector<BorrowingRecord>& records) {
    int maxUserId = 0;
    for (const auto& user : users) {
        if (user->getId() > maxUserId) {
            maxUserId = user->getId();
        }
    }
    nextUserId = maxUserId + 1;

    int maxBookId = 0;
    for (const auto& book : books) {
        if (book->getId() > maxBookId) {
            maxBookId = book->getId();
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

void DataManager::loadAllData(vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records) {
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

        if (tokens.size() < 5) continue; 

        int role = stoi(tokens[0]);
        int id = stoi(tokens[1]);
        string name = tokens[2];
        string email = tokens[3];
        string password = tokens[4];

        if (role == 1) { 
            users.push_back(new Admin(id, name, email, password));
        } 
        else if (role == 2 && tokens.size() >= 6) { 
            string employeeId = tokens[5];
            users.push_back(new Librarian(id, name, email, password, employeeId));
        } 
        else if (role == 3 && tokens.size() >= 9) { 
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

        if (tokens.size() >= 7) {
            int bookType = stoi(tokens[0]);
            int id = stoi(tokens[1]);
            string title = tokens[2];
            string author = tokens[3];
            int pubYear = stoi(tokens[4]);
            int available = stoi(tokens[5]);
            int total = stoi(tokens[6]);
            
            if (bookType == 1) {
                books.push_back(new Newspaper(id, title, author, total, available, pubYear));
            } else if (bookType == 2) {
                books.push_back(new Magazine(id, title, author, total, available, pubYear));
            } else if (bookType == 3) {
                books.push_back(new Textbook(id, title, author, total, available, pubYear));
            } else if (bookType == 4) {
                books.push_back(new Novel(id, title, author, total, available, pubYear));
            }
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
            int bookId = stoi(tokens[1]);
            int userId = stoi(tokens[2]);
            string borrowDate = tokens[3];
            string returnDate = tokens[4];
            int status = stoi(tokens[5]);
            records.push_back(BorrowingRecord(recordId, bookId, userId, borrowDate, returnDate, status));
        }
    }
    recordFileStream.close();

    findNextIds(users, books, records);
}

void DataManager::saveAllData(const vector<Person*>& users, const vector<Book*>& books, const vector<BorrowingRecord>& records) {
    ofstream userFileStream(usersFile);
    for (const auto& user : users) {
        userFileStream << user->toCSV() << endl;
    }
    userFileStream.close();

    ofstream bookFileStream(booksFile);
    for (const auto& book : books) {
        bookFileStream << book->toCSV() << endl; 
    }
    bookFileStream.close();

    ofstream recordFileStream(recordsFile);
    for (const auto& record : records) {
        recordFileStream << record.toCSV() << endl;
    }
    recordFileStream.close();
}

// Nằm trong file: DataManager.cpp

// **** THAY THẾ TOÀN BỘ HÀM NÀY BẰNG CODE BÊN DƯỚI ****
bool DataManager::importBooksFromFile(const string& filename, vector<Book*>& books) {
    ifstream fileStream(filename);
    if (!fileStream.is_open()) {
        cout << "LOI: Khong the mo file '" << filename << "'." << endl;
        return false;
    }

    string line;
    // --- Counters mới để báo cáo chi tiết ---
    int newCount = 0;
    int updateCount = 0;
    int failCount = 0;
    
    // (Tùy chọn) Bỏ qua dòng tiêu đề
    // getline(fileStream, line); 

    while (getline(fileStream, line)) {
        if (line.empty()) continue; // Bỏ qua các dòng trống

        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 5) {
            try {
                int bookType = stoi(tokens[0]);
                string title = tokens[1];
                string author = tokens[2];
                int pubYear = stoi(tokens[3]);
                int quantityToAdd = stoi(tokens[4]); 
                
                if (pubYear > 2025 || pubYear <= 0 || quantityToAdd <= 0 || bookType < 1 || bookType > 4) {
                     cout << "LOI: Du lieu khong hop le (Nam/SoLuong/Loai) -> " << line << endl;
                     failCount++;
                     continue;
                }
                bool bookFound = false;
                for (auto* existingBook : books) {
                    if (existingBook->getBookType() == bookType &&
                        existingBook->getTitle() == title &&
                        existingBook->getAuthor() == author &&
                        existingBook->getPublicationYear() == pubYear) 
                    {
                        existingBook->setTotalQuantity(existingBook->getTotalQuantity() + quantityToAdd);
                        existingBook->setAvailableQuantity(existingBook->getAvailableQuantity() + quantityToAdd);
                        
                        bookFound = true;
                        updateCount++; 
                        break; 
                    }
                }

                if (!bookFound) {
                    int newId = getNextBookId(); 

                    if (bookType == 1) books.push_back(new Newspaper(newId, title, author, quantityToAdd, quantityToAdd, pubYear));
                    else if (bookType == 2) books.push_back(new Magazine(newId, title, author, quantityToAdd, quantityToAdd, pubYear));
                    else if (bookType == 3) books.push_back(new Textbook(newId, title, author, quantityToAdd, quantityToAdd, pubYear));
                    else if (bookType == 4) books.push_back(new Novel(newId, title, author, quantityToAdd, quantityToAdd, pubYear));
                    
                    newCount++; 
                }

            } catch (const std::exception& e) {
                cout << "LOI: Dong bi loi dinh dang (khong phai so) -> " << line << endl;
                failCount++;
            }
        } else {
             cout << "LOI: Dong khong du cot -> " << line << endl;
             failCount++;
        }
    }
    fileStream.close();
    cout << "----------------------------------------" << endl;
    cout << "Nhap Sach Hoan Tat." << endl;
    cout << "Sach moi them: " << newCount 
         << " | Sach cap nhat SL: " << updateCount 
         << " | That bai: " << failCount << endl;
    return true;
}



bool DataManager::importUsersFromFile(const string& filename, vector<Person*>& users) {
    ifstream fileStream(filename);
    if (!fileStream.is_open()) {
        cout << "LOI: Khong the mo file '" << filename << "'." << endl;
        return false;
    }

    string line;
    int successCount = 0;
    int failCount = 0;

    while (getline(fileStream, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        try {
            if (tokens.empty()) continue; 

            int role = stoi(tokens[0]);
            string name, email, password, specificId;

            email = tokens[2];
            if (!Menu::isValidEmailFormat(email)) {
                cout << "LOI: Dinh dang email sai -> " << line << endl;
                failCount++;
                continue;
            }
            bool emailExists = false;
            for (const auto& user : users) {
                if (user->getEmail() == email) {
                    emailExists = true;
                    break;
                }
            }
            if (emailExists) {
                cout << "LOI: Email da ton tai -> " << line << endl;
                failCount++;
                continue;
            }

            if (role == 2) { 
                if (tokens.size() < 5) {
                    cout << "LOI: Thieu cot cho Librarian -> " << line << endl;
                    failCount++;
                    continue;
                }
                name = tokens[1];
                password = tokens[3];
                specificId = tokens[4]; 

                bool idExists = false;
                for (const auto& user : users) {
                    if (Librarian* lib = dynamic_cast<Librarian*>(user)) {
                        if (lib->getEmployeeId() == specificId) {
                            idExists = true;
                            break;
                        }
                    }
                }
                if (idExists) {
                    cout << "LOI: Ma Nhan Vien da ton tai -> " << line << endl;
                    failCount++;
                    continue;
                }
                
                users.push_back(new Librarian(getNextUserId(), name, email, password, specificId));
                successCount++;

            } else if (role == 3) { 
                if (tokens.size() < 7) {
                    cout << "LOI: Thieu cot cho Student -> " << line << endl;
                    failCount++;
                    continue;
                }
                name = tokens[1];
                password = tokens[3];
                specificId = tokens[4]; 
                string studentClass = tokens[5];
                int courseYear = stoi(tokens[6]);

                bool idExists = false;
                for (const auto& user : users) {
                    if (Student* s = dynamic_cast<Student*>(user)) {
                        if (s->getStudentId() == specificId) {
                            idExists = true;
                            break;
                        }
                    }
                }
                if (idExists) {
                    cout << "LOI: Ma Sinh Vien da ton tai -> " << line << endl;
                    failCount++;
                    continue;
                }

                users.push_back(new Student(getNextUserId(), name, email, password, specificId, studentClass, courseYear, 0.0));
                successCount++;

            } else {
                cout << "LOI: Role khong hop le (chi 2 hoac 3) -> " << line << endl;
                failCount++;
            }

        } catch (const std::exception& e) {
            cout << "LOI: Dong bi loi dinh dang (khong phai so) -> " << line << endl;
            failCount++;
        }
    }

    fileStream.close();
    cout << "----------------------------------------" << endl;
    cout << "Nhap User Hoan Tat." << endl;
    cout << "Thanh cong: " << successCount << " | That bai: " << failCount << endl;
    return true;
}