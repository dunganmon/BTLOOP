// StudentService.cpp
#include "StudentService.h"
#include "Menu.h"
#include <ctime> // Để lấy ngày giờ hiện tại
#include <iomanip>

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
}

void StudentService::run(Student* student, vector<Book>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 8) {
        Menu::displayStudentMenu(student->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: Menu::searchAndDisplayBooks(books); break;
            case 2: Menu::viewAllBooks(books); break;
            case 3: borrowBook(student, books, records, dataManager); break;
            case 4: returnBook(student, books, records); break;
            case 5: reportLostBook(student, books, records); break;
            case 6: viewMyInfo(student, records, books); break;
            case 7: cout << "Chuc nang 'Bao tim duoc sach' dang duoc phat trien." << endl; break;
            case 8: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 8) Menu::pause();
    }
}

void StudentService::viewMyInfo(Student* student, const vector<BorrowingRecord>& allRecords, const vector<Book>& allBooks) {
    student->displayDetailedInfo(); 

    cout << "\n--- Sach Dang Muon ---" << endl;
    bool hasBorrowedBooks = false;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(34) << "Tieu de"
         << "| " << left << setw(15) << "Ngay muon" << " |" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) { // Status 0: Dang muon
            for (const auto& book : allBooks) {
                if (book.getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << book.getId()
                         << "| " << left << setw(34) << book.getTitle()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasBorrowedBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasBorrowedBooks) {
        cout << "| " << left << setw(66) << "Ban chua muon cuon sach nao." << " |" << endl;
    }
    cout << "-------------------------------------------------------------------" << endl;

    cout << "\n--- Sach Da Lam Mat ---" << endl;
    bool hasLostBooks = false;
    cout << "-------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(34) << "Tieu de" << " |" << endl;
    cout << "-------------------------------------------------" << endl;

    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 2) { // Status 2: Lam mat
            for (const auto& book : allBooks) {
                if (book.getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << book.getId()
                         << "| " << left << setw(34) << book.getTitle() << " |" << endl;
                    hasLostBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasLostBooks) {
        cout << "| " << left << setw(48) << "Ban khong lam mat cuon sach nao." << " |" << endl;
    }
    cout << "-------------------------------------------------" << endl;
}

void StudentService::borrowBook(Student* student, vector<Book>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    cout << "--- Muon Sach ---" << endl;
    Menu::viewAllBooks(books);
    cout << "Nhap ID sach ban muon muon: ";
    int bookId = Menu::getIntegerInput();

    Book* targetBook = nullptr;
    for (auto& book : books) {
        if (book.getId() == bookId) {
            targetBook = &book;
            break;
        }
    }

    if (targetBook == nullptr) {
        cout << "Khong tim thay sach voi ID nay." << endl;
        return;
    }

    if (targetBook->borrowBook()) { // borrowBook() trả về true nếu còn sách
        int newRecordId = dataManager.getNextRecordId();
        records.push_back(BorrowingRecord(newRecordId, bookId, student->getId(), getCurrentDate(), "N/A", 0));
        cout << "Muon sach '" << targetBook->getTitle() << "' thanh cong!" << endl;
    } else {
        cout << "Xin loi, sach '" << targetBook->getTitle() << "' da het." << endl;
    }
}

void StudentService::returnBook(Student* student, vector<Book>& books, vector<BorrowingRecord>& records) {
    cout << "--- Tra Sach ---" << endl;
    cout << "Danh sach sach ban dang muon:" << endl;
    vector<int> borrowedRecordIds;
    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) {
            for (const auto& book : books) {
                if (book.getId() == record.getBookId()) {
                    cout << "ID Phieu muon: " << record.getRecordId() << " | ID Sach: " << book.getId() << " | Tieu de: " << book.getTitle() << endl;
                    borrowedRecordIds.push_back(record.getRecordId());
                    break;
                }
            }
        }
    }

    if (borrowedRecordIds.empty()) {
        cout << "Ban khong co sach nao de tra." << endl;
        return;
    }

    cout << "Nhap ID Phieu muon cua sach ban muon tra: ";
    int recordId = Menu::getIntegerInput();

    for (auto& record : records) {
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 0) {
            record.setStatus(1); // 1: Da tra
            record.setReturnDate(getCurrentDate());
            for (auto& book : books) {
                if (book.getId() == record.getBookId()) {
                    book.returnBook();
                    cout << "Tra sach '" << book.getTitle() << "' thanh cong!" << endl;
                    return;
                }
            }
        }
    }
    cout << "ID phieu muon khong hop le hoac ban khong muon sach nay." << endl;
}

void StudentService::reportLostBook(Student* student, vector<Book>& books, vector<BorrowingRecord>& records) {
    cout << "--- Bao Mat Sach ---" << endl;
    cout << "Danh sach sach ban dang muon:" << endl;
    vector<int> borrowedRecordIds;
    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) {
             for (const auto& book : books) {
                if (book.getId() == record.getBookId()) {
                    cout << "ID Phieu muon: " << record.getRecordId() << " | ID Sach: " << book.getId() << " | Tieu de: " << book.getTitle() << endl;
                    borrowedRecordIds.push_back(record.getRecordId());
                    break;
                }
            }
        }
    }

    if (borrowedRecordIds.empty()) {
        cout << "Ban khong co sach nao de bao mat." << endl;
        return;
    }

    cout << "Nhap ID Phieu muon cua sach ban lam mat: ";
    int recordId = Menu::getIntegerInput();

    for (auto& record : records) {
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 0) {
            record.setStatus(2); // 2: Lam mat
            record.setReturnDate(getCurrentDate());
            
            // Giảm tổng số lượng sách và số lượng có sẵn
            for (auto& book : books) {
                if (book.getId() == record.getBookId()) {
                    double fine = 100000.0; // Giả sử tiền phạt là 100,000 VND
                    student->addFine(fine);
                    book.setTotalQuantity(book.getTotalQuantity() - 1);
                    // Số lượng có sẵn đã giảm khi mượn, nên không cần giảm nữa
                    cout << "Da ghi nhan ban lam mat sach '" << book.getTitle() << "'." << endl;
                    cout << "Ban bi phat " << fine << " VND. Tong no hien tai: " << student->getFineAmount() << " VND." << endl;
                    return;
                }
            }
        }
    }
    cout << "ID phieu muon khong hop le hoac ban khong muon sach nay." << endl;
}