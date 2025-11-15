#include "StudentService.h"
#include "Menu.h"
#include <ctime> 
#include <iomanip>

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-" 
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" 
       << setfill('0') << setw(2) << ltm->tm_mday;
    return ss.str();
}

int StudentService::countMyBorrowedBooks(Student* student, const vector<BorrowingRecord>& allRecords) {
    int count = 0;
    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) {
            count++;
        }
    }
    return count;
}


void StudentService::run(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 10) {
        Menu::displayStudentMenu(student->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: Menu::searchAndDisplayBooks(books); break;
            case 2: Menu::viewAllBooks(books); break;
            case 3: Menu::displayBooksByType(books); break;    
            case 4: Menu::displaySortedBooks(books); break;   
            case 5: borrowBook(student, books, records, dataManager); break;
            case 6: returnBook(student, books, records); break;
            case 7: reportLostBook(student, books, records); break;
            case 8: reportFoundBook(student, books, records); break;
            case 9: viewMyInfo(student, records, books); break;
            case 10: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 10) Menu::pause();
    }
}

void StudentService::viewMyInfo(Student* student, const vector<BorrowingRecord>& allRecords, const vector<Book*>& allBooks) {
    student->displayDetailedInfo(); 

    cout << "\n--- Sach Dang Muon ---" << endl;
    bool hasBorrowedBooks = false;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu" 
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de"
         << "| " << left << setw(15) << "Ngay muon" << " |" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) { 
            for (const auto* book : allBooks) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasBorrowedBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasBorrowedBooks) {
        cout << "| " << left << setw(62) << "Ban chua muon cuon sach nao." << " |" << endl;
    }
    cout << "-------------------------------------------------------------------" << endl;

    cout << "\n--- Sach Da Lam Mat ---" << endl;
    bool hasLostBooks = false;
    cout << "-------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu" 
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de" << " |" << endl;
    cout << "-------------------------------------------------" << endl;

    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 2) { 
            for (const auto* book : allBooks) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle() << " |" << endl;
                    hasLostBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasLostBooks) {
        cout << "| " << left << setw(45) << "Ban khong lam mat cuon sach nao." << " |" << endl;
    }
    cout << "-------------------------------------------------" << endl;
}

void StudentService::borrowBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    cout << "--- Muon Sach ---" << endl;
    Menu::viewAllBooks(books);
    cout << "Nhap ID sach ban muon muon: ";
    int bookId = Menu::getIntegerInput();

    Book* targetBook = nullptr;
    for (auto* book : books) { 
        if (book->getId() == bookId) {
            targetBook = book;
            break;
        }
    }

    if (targetBook == nullptr) {
        cout << "Khong tim thay sach voi ID nay." << endl;
        return;
    }

    cout << "Sach '" << targetBook->getTitle() << "' hien con " << targetBook->getAvailableQuantity() << " quyen." << endl;
    cout << "Nhap so luong ban muon muon: ";
    int quantityToBorrow = Menu::getIntegerInput();

    if (quantityToBorrow <= 0) {
        cout << "So luong muon phai lon hon 0." << endl;
        return;
    }

    if (quantityToBorrow > targetBook->getAvailableQuantity()) {
        cout << "Xin loi, so luong ban muon (" << quantityToBorrow 
             << ") vuot qua so sach con lai (" << targetBook->getAvailableQuantity() << ")." << endl;
        return;
    }

    int currentBorrowedCount = countMyBorrowedBooks(student, records);
    const int MAX_BORROW_LIMIT = 10;

    if (currentBorrowedCount + quantityToBorrow > MAX_BORROW_LIMIT) {
        cout << "Xin loi, ban da muon " << currentBorrowedCount << " quyen." << endl;
        cout << "Ban chi co the muon them toi da " << (MAX_BORROW_LIMIT - currentBorrowedCount) << " quyen nua." << endl;
        return;
    }

    for (int i = 0; i < quantityToBorrow; i++) {
        targetBook->borrowBook(); 
        int newRecordId = dataManager.getNextRecordId();
        records.push_back(BorrowingRecord(newRecordId, bookId, student->getId(), getCurrentDate(), "N/A", 0));
    }

    cout << "Muon thanh cong " << quantityToBorrow << " quyen sach '" << targetBook->getTitle() << "'!" << endl;
}

void StudentService::returnBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records) {
    cout << "--- Tra Sach ---" << endl;
    cout << "Danh sach sach ban dang muon:" << endl;
    
    bool hasBorrowedBooks = false;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de"
         << "| " << left << setw(15) << "Ngay muon" << " |" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) { 
            for (const auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasBorrowedBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasBorrowedBooks) {
        cout << "| " << left << setw(62) << "Ban chua muon cuon sach nao." << " |" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        return;
    }
    cout << "-------------------------------------------------------------------" << endl;


    cout << "Nhap ID Phieu muon cua sach ban muon tra: ";
    int recordId = Menu::getIntegerInput();

    for (auto& record : records) {
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 0) {
            record.setStatus(1); 
            record.setReturnDate(getCurrentDate());
            for (auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    book->returnBook(); 
                    cout << "Tra sach '" << book->getTitle() << "' thanh cong!" << endl;
                    return;
                }
            }
        }
    }
    cout << "ID phieu muon khong hop le hoac ban khong muon sach nay." << endl;
}

void StudentService::reportLostBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records) {
    cout << "--- Bao Mat Sach ---" << endl;
    cout << "Danh sach sach ban dang muon:" << endl;
    
    bool hasBorrowedBooks = false;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de"
         << "| " << left << setw(15) << "Ngay muon" << " |" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) { 
            for (const auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasBorrowedBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasBorrowedBooks) {
        cout << "| " << left << setw(62) << "Ban chua muon muon sach nao." << " |" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        return;
    }
    cout << "-------------------------------------------------------------------" << endl;


    cout << "Nhap ID Phieu muon cua sach ban lam mat: ";
    int recordId = Menu::getIntegerInput();

    for (auto& record : records) {
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 0) {
            record.setStatus(2); 
            record.setReturnDate(getCurrentDate());
            
            for (auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    double fine = 100000.0; 
                    student->addFine(fine);
                    book->setTotalQuantity(book->getTotalQuantity() - 1); 
                    
                    cout << "Da ghi nhan ban lam mat sach '" << book->getTitle() << "'." << endl;
                    cout << "Ban bi phat " << fixed << setprecision(0) << fine << " VND. Tong no hien tai: " << student->getFineAmount() << " VND." << endl;
                    return;
                }
            }
        }
    }
    cout << "ID phieu muon khong hop le hoac ban khong muon sach nay." << endl;
}

void StudentService::reportFoundBook(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records) {
    cout << "--- Bao Tim Duoc Sach Da Mat ---" << endl;
    cout << "Danh sach sach ban da bao mat:" << endl;
    
    bool hasLostBooks = false;
    cout << "-------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de" << " |" << endl;
    cout << "-------------------------------------------------" << endl;

    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 2) { 
            for (const auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle() << " |" << endl;
                    hasLostBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasLostBooks) {
        cout << "| " << left << setw(45) << "Ban khong lam mat cuon sach nao." << " |" << endl;
        cout << "-------------------------------------------------" << endl;
        return;
    }
    cout << "-------------------------------------------------" << endl;

    cout << "Nhap ID Phieu cua sach ban moi tim thay: ";
    int recordId = Menu::getIntegerInput();

    for (auto& record : records) {
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 2) {
            
            record.setStatus(1); 
            
            double fine = 100000.0; 
            student->payFine(fine); 

            for (auto* book : books) { 
                if (book->getId() == record.getBookId()) {
                    book->setTotalQuantity(book->getTotalQuantity() + 1); 
                    book->returnBook(); 
                    
                    cout << "Da ghi nhan tim thay sach '" << book->getTitle() << "'." << endl;
                    cout << "Da hoan lai " << fixed << setprecision(0) << fine << " VND. Tong no hien tai: " << student->getFineAmount() << " VND." << endl;
                    return;
                }
            }
        }
    }

    cout << "ID phieu muon khong hop le hoac do khong phai sach ban bao mat." << endl;
}