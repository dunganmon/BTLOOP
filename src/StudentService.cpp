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
        if (record.getStudentId() == student->getId() && (record.getStatus() == 1 || record.getStatus() == 0)) {
            count++;
        }
    }
    return count;
}


void StudentService::run(Student* student, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 11) {
        Menu::displayStudentMenu(student->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: Menu::searchAndDisplayBooks(books); break;
            case 2: Menu::viewAllBooks(books); break;
            case 3: Menu::displayBooksByType(books); break;    
            case 4: Menu::displaySortedBooks(books); break;   
            case 5: borrowBook(student, books, records, dataManager); break;
            case 6: cancelBorrowRequest(student, records); break;
            case 7: returnBook(student, books, records); break;
            case 8: reportLostBook(student, books, records); break;
            case 9: viewMyInfo(student, records, books); break;
            case 10: changePassword(student); break;
            case 11: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 11) Menu::pause();
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
        if (record.getStudentId() == student->getId() && record.getStatus() == 1) { 
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

    cout << "\n--- Sach Dang Cho Duyet ---" << endl;
    bool hasPendingBooks = false;
    cout << "-------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Tieu de"
         << "| " << left << setw(15) << "Ngay yeu cau" << " |" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (const auto& record : allRecords) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) { // 0 = Chờ duyệt
            for (const auto* book : allBooks) { 
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(25) << book->getTitle()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasPendingBooks = true;
                    break;
                }
            }
        }
    }
    if (!hasPendingBooks) {
        cout << "| " << left << setw(62) << "Ban khong co yeu cau nao cho duyet." << " |" << endl;
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
        if (record.getStudentId() == student->getId() && record.getStatus() == 3) { 
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
    cout << "--- Gui Yeu Cau Muon Sach ---" << endl;
    Menu::viewAllBooks(books);
    cout << "Nhap ID sach ban muon gui yeu cau: ";
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
    cout << "Nhap so luong ban muon gui yeu cau: ";
    int quantityToBorrow = Menu::getIntegerInput();

    if (quantityToBorrow <= 0) {
        cout << "So luong phai lon hon 0." << endl;
        return;
    }

    if (quantityToBorrow > targetBook->getAvailableQuantity()) {
        cout << "Xin loi, so luong ban yeu cau (" << quantityToBorrow 
             << ") vuot qua so sach con lai (" << targetBook->getAvailableQuantity() << ")." << endl;
        return;
    }

    int currentBorrowedCount = countMyBorrowedBooks(student, records);
    const int MAX_BORROW_LIMIT = 10;

    if (currentBorrowedCount + quantityToBorrow > MAX_BORROW_LIMIT) {
        cout << "Xin loi, ban da co " << currentBorrowedCount << " sach (dang muon + cho duyet)." << endl;
        cout << "Ban chi co the yeu cau them toi da " << (MAX_BORROW_LIMIT - currentBorrowedCount) << " quyen nua." << endl;
        return;
    }

    for (int i = 0; i < quantityToBorrow; i++) {
        int newRecordId = dataManager.getNextRecordId();
        records.push_back(BorrowingRecord(newRecordId, bookId, student->getId(), getCurrentDate(), "N/A", "N/A", 0));
    }

    cout << "Da gui thanh cong " << quantityToBorrow << " yeu cau muon sach '" << targetBook->getTitle() << "'!" << endl;
    cout << "Vui long cho Thu thu duyet yeu cau." << endl;
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
        if (record.getStudentId() == student->getId() && record.getStatus() == 1) { 
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
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 1) {
            string returnDate = getCurrentDate();
            string dueDate = record.getDueDate();
            if (returnDate > dueDate) {
                double fine = 10000.0; 
                student->addFine(fine);
                cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                cout << "!! Ban da tra sach tre han. Phat: " << fixed << setprecision(0) << fine << " VND." << endl;
                cout << "!! Han tra: " << dueDate << " | Ngay tra: " << returnDate << endl;
                cout << "!! Tong no hien tai: " << student->getFineAmount() << " VND." << endl;
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << endl;
            }

            record.setStatus(2); 
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
        if (record.getStudentId() == student->getId() && record.getStatus() == 1) { 
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
        if (record.getRecordId() == recordId && record.getStudentId() == student->getId() && record.getStatus() == 1) {
            string lostDate = getCurrentDate();
            string dueDate = record.getDueDate();
            if (lostDate > dueDate) {
                double overdueFine = 10000.0; 
                student->addFine(overdueFine);
                cout << "\n!! Ban bi phat tre han: " << fixed << setprecision(0) << overdueFine << " VND." << endl;
            }
            record.setStatus(3); 
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

void StudentService::cancelBorrowRequest(Student* student, vector<BorrowingRecord>& records) {
    cout << "--- Huy Yeu Cau Muon Sach ---" << endl;
    
    bool hasRequests = false;
    cout << "Danh sach yeu cau dang cho duyet cua ban:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(25) << "Ngay yeu cau" << " |" << endl;
    cout << "-------------------------------------------------" << endl;

    for (const auto& record : records) {
        if (record.getStudentId() == student->getId() && record.getStatus() == 0) {
            cout << "| " << left << setw(8) << record.getRecordId()
                 << "| " << left << setw(8) << record.getBookId()
                 << "| " << left << setw(25) << record.getBorrowDate() << " |" << endl;
            hasRequests = true;
        }
    }

    if (!hasRequests) {
        cout << "| " << left << setw(45) << "Ban khong co yeu cau nao dang cho." << " |" << endl;
        cout << "-------------------------------------------------" << endl;
        return;
    }
    cout << "-------------------------------------------------" << endl;

    cout << "Nhap ID Phieu muon ban muon huy (nhap 0 de thoat): ";
    int recordIdToCancel = Menu::getIntegerInput();

    if (recordIdToCancel == 0) return;
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->getRecordId() == recordIdToCancel && 
            it->getStudentId() == student->getId() && 
            it->getStatus() == 0) 
        {
            records.erase(it);
            cout << "Da huy thanh cong yeu cau co ID: " << recordIdToCancel << "." << endl;
            return;
        }
    }

    cout << "Khong tim thay yeu cau hop le voi ID: " << recordIdToCancel << "." << endl;
}

void StudentService::changePassword(Student* student) {
    cout << "--- Doi Mat Khau ---" << endl;
    cout << "Nhap mat khau cu: ";
    string oldPass = Menu::getStringNoSpaces();

    if (oldPass != student->getPassword()) {
        cout << "Mat khau cu không dung. Thao tac bi huy." << endl;
        return;
    }

    string newPass = Menu::getStringPassword(); 
    
    cout << "Xac nhan mat khau moi: ";
    string confirmPass = Menu::getStringNoSpaces();

    if (newPass != confirmPass) {
        cout << "Mat khau xac nhan không khop. Thao tac bi huy." << endl;
        return;
    }

    student->setPassword(newPass);
    cout << "Doi mat khau thanh cong!" << endl;
}