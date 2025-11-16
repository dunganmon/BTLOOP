#include "LibrarianService.h"
#include "Menu.h"
#include "Student.h"
#include "Newspaper.h"
#include "Magazine.h"
#include "Textbook.h"
#include "Novel.h"
#include <ctime>     
#include <sstream>  
#include <iomanip>
string getLibrarianCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-" 
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" 
       << setfill('0') << setw(2) << ltm->tm_mday;
    return ss.str();
}
string calculateDueDate(const string& dateStr, int daysToAdd) {
    tm tm_in = {};
    stringstream ss(dateStr);
    ss >> get_time(&tm_in, "%Y-%m-%d");

    tm_in.tm_mday += daysToAdd; // Cộng thêm số ngày
    mktime(&tm_in); // Hàm này sẽ tự động sửa ngày (vd: 35/11 -> 05/12)

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm_in);
    return string(buffer);
}
void LibrarianService::run(Librarian* librarian, vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 11) { 
        Menu::displayLibrarianMenu(librarian->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: addBook(books, dataManager); break;
            case 2: editBook(books); break;
            case 3: deleteBook(books, records); break;
            case 4: Menu::viewAllBooks(books); break;
            case 5: searchBook(books); break;
            case 6: Menu::displayBooksByType(books); break;  
            case 7: Menu::displaySortedBooks(books); break; 
            case 8: reviewPendingRequests(records, books); break;
            case 9: collectFines(users); break;
            case 10: changePassword(librarian); break;
            case 11: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 11) Menu::pause();
    }
}
void LibrarianService::addBook(vector<Book*>& books, DataManager& dataManager) {
    cout << "--- Them Sach Moi ---" << endl;
    int bookType = Menu::getBookTypeInput();

    cout << "Nhap tieu de sach: ";
    string title;
    getline(cin >> ws, title);
    
    cout << "Nhap tac gia: ";
    string author;
    getline(cin >> ws, author);
    
    int quantity;
    do {
        cout << "Nhap so luong: ";
        quantity = Menu::getIntegerInput();
        if (quantity <= 0) {
            cout << "Loi: So luong phai la so duong. Vui long nhap lai." << endl;
        }
    } while (quantity <= 0);

    int pubYear = Menu::getPublicationYearInput();
    int newId = dataManager.getNextBookId();

    switch (bookType) {
        case 1: 
            books.push_back(new Newspaper(newId, title, author, quantity, quantity, pubYear));
            break;
        case 2: 
            books.push_back(new Magazine(newId, title, author, quantity, quantity, pubYear));
            break;
        case 3: 
            books.push_back(new Textbook(newId, title, author, quantity, quantity, pubYear));
            break;
        case 4: 
            books.push_back(new Novel(newId, title, author, quantity, quantity, pubYear));
            break;
    }
    
    cout << "Them sach moi thanh cong!" << endl;
}

void LibrarianService::editBook(vector<Book*>& books) {
    Menu::viewAllBooks(books);
    cout << "--- Chinh Sua Thong Tin Sach ---" << endl;
    cout << "Nhap ID sach can chinh sua: ";
    int idToEdit = Menu::getIntegerInput();

    Book* targetBook = nullptr;
    int targetIndex = -1; 
    for (int i = 0; i < books.size(); ++i) { 
        if (books[i]->getId() == idToEdit) {
            targetBook = books[i];
            targetIndex = i;
            break;
        }
    }

    if (targetBook == nullptr) {
        cout << "Khong tim thay sach voi ID " << idToEdit << "." << endl;
        return;
    }

    string newTitle, newAuthor;
    int newTotal, newYear, newType;

    cout << "Nhap tieu de moi (de trong de giu nguyen [" << targetBook->getTitle() << "]): ";
    getline(cin, newTitle); 
    cout << "Nhap tac gia moi (de trong de giu nguyen [" << targetBook->getAuthor() << "]): ";
    getline(cin, newAuthor); 
    cout << "Nhap nam xuat ban moi (-1 de giu nguyen [" << targetBook->getPublicationYear() << "]): ";
    newYear = Menu::getIntegerInput();
    cout << "Nhap tong so luong moi (-1 de giu nguyen [" << targetBook->getTotalQuantity() << "]): ";
    newTotal = Menu::getIntegerInput();

    cout << "Chon loai sach moi (-1 de giu nguyen [" << targetBook->getBookTypeString() << "]):" << endl;
    cout << "1. Bao" << endl;
    cout << "2. Tap chi" << endl;
    cout << "3. Giao trinh" << endl;
    cout << "4. Tieu thuyet" << endl;
    cout << "Lua chon cua ban: ";
    newType = Menu::getIntegerInput();

    if (!newTitle.empty()) targetBook->setTitle(newTitle);
    if (!newAuthor.empty()) targetBook->setAuthor(newAuthor);

    if (newYear != -1) {
        if (newYear > 2025 || newYear <= 0) {
            cout << "Loi: Nam xuat ban khong hop le. Cap nhat nam THAT BAI." << endl;
        } else {
            targetBook->setPublicationYear(newYear);
        }
    }

    if (newTotal != -1) {
        int borrowedCount = targetBook->getTotalQuantity() - targetBook->getAvailableQuantity();
        if (newTotal <= 0) {
            cout << "Loi: So luong moi phai la so duong." << endl;
            cout << "Cap nhat so luong THAT BAI." << endl;
        }
        else if (newTotal < borrowedCount) {
            cout << "Loi: So luong moi (" << newTotal 
                 << ") nho hon so sach dang duoc muon (" << borrowedCount << ")." << endl;
            cout << "Cap nhat so luong THAT BAI." << endl;
        } else {
            targetBook->setTotalQuantity(newTotal);
            targetBook->setAvailableQuantity(newTotal - borrowedCount);
            cout << "Cap nhat so luong thanh cong! (Tong: " << newTotal 
                 << ", Con lai: " << (newTotal - borrowedCount) << ")" << endl;
        }
    }
    if (newType == -1) {} 
    else if (newType == targetBook->getBookType()) {
        cout << "Ban da chon lai loai sach hien tai, khong co gi thay doi." << endl;
    } 
    else if (newType >= 1 && newType <= 4) {
        int id = targetBook->getId();
        string title = targetBook->getTitle();
        string author = targetBook->getAuthor();
        int year = targetBook->getPublicationYear();
        int total = targetBook->getTotalQuantity();
        int available = targetBook->getAvailableQuantity();

        delete books[targetIndex]; 
        
        Book* newBook = nullptr;
        if (newType == 1) newBook = new Newspaper(id, title, author, total, available, year);
        else if (newType == 2) newBook = new Magazine(id, title, author, total, available, year);
        else if (newType == 3) newBook = new Textbook(id, title, author, total, available, year);
        else if (newType == 4) newBook = new Novel(id, title, author, total, available, year);
        
        books[targetIndex] = newBook; 
        
        cout << "Cap nhat LOAI SACH thanh cong!" << endl;
    } 
    else {
        cout << "Loi: Lua chon loai sach khong hop le (" << newType << "). Cap nhat loai sach THAT BAI." << endl;
    }
    cout << "Cap nhat thong tin sach hoan tat!" << endl;
}

void LibrarianService::searchBook(const vector<Book*>& books) {
    Menu::searchAndDisplayBooks(books);
}

void LibrarianService::deleteBook(vector<Book*>& books, const vector<BorrowingRecord>& records) {
    Menu::viewAllBooks(books);
    cout << "--- Xoa Sach ---" << endl;
    cout << "Nhap ID sach can xoa: ";
    int idToDelete = Menu::getIntegerInput();

    bool isBorrowed = false;
    for (const auto& record : records) {
        if (record.getBookId() == idToDelete && (record.getStatus() == 1 || record.getStatus() == 0)) { 
            isBorrowed = true;
            break;
        }
    }

    if (isBorrowed) {
        cout << "Loi: Khong the xoa sach nay. Dang co sinh vien muon hoac cho duyet." << endl;
        return;
    }

    for (auto it = books.begin(); it != books.end(); ++it) {
        if ((*it)->getId() == idToDelete) {
            delete *it; 
            books.erase(it);
            cout << "Da xoa sach co ID " << idToDelete << "." << endl;
            return;
        }
    }
    cout << "Khong tim thay sach voi ID " << idToDelete << "." << endl;
}

void LibrarianService::reviewPendingRequests(vector<BorrowingRecord>& records, vector<Book*>& books) {
    Menu::clearScreen();
    cout << "--- Duyet Yeu Cau Muon Sach ---" << endl;
    
    bool hasRequests = false;
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Phieu"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(35) << "Tieu de"
         << "| " << left << setw(10) << "ID SV"
         << "| " << left << setw(15) << "Ngay yeu cau" << " |" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

    for (const auto& record : records) {
        if (record.getStatus() == 0) {
            for (const auto* book : books) {
                if (book->getId() == record.getBookId()) {
                    cout << "| " << left << setw(8) << record.getRecordId()
                         << "| " << left << setw(8) << book->getId()
                         << "| " << left << setw(35) << book->getTitle()
                         << "| " << left << setw(10) << record.getStudentId()
                         << "| " << left << setw(15) << record.getBorrowDate() << " |" << endl;
                    hasRequests = true;
                    break;
                }
            }
        }
    }

    if (!hasRequests) {
        cout << "| " << left << setw(84) << "Khong co yeu cau muon sach nao dang cho." << " |" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        return;
    }
    cout << "----------------------------------------------------------------------------------------" << endl;

    cout << "\nNhap ID Phieu muon ban muon xu ly (nhap 0 de thoat): ";
    int recordIdToProcess = Menu::getIntegerInput();

    if (recordIdToProcess == 0) return;

    for (auto it = records.begin(); it != records.end(); ++it) {
        
        if (it->getRecordId() == recordIdToProcess && it->getStatus() == 0) {
            
            cout << "Ban muon lam gi voi phieu muon " << recordIdToProcess << "?" << endl;
            cout << "1. Duyet" << endl;
            cout << "2. Huy" << endl;
            cout << "0. Quay lai" << endl;
            cout << "Lua chon cua ban: ";
            int actionChoice = Menu::getIntegerInput();
            
            if (actionChoice == 1) { 
                
                Book* targetBook = nullptr;
                for (auto* book : books) {
                    if (book->getId() == it->getBookId()) {
                        targetBook = book;
                        break;
                    }
                }

                if (targetBook == nullptr) {
                    cout << "LOI: Khong tim thay sach. Phieu bi loi." << endl;
                    records.erase(it); 
                    cout << "Phieu loi da duoc tu dong huy." << endl;
                    return;
                }

                if (targetBook->getAvailableQuantity() > 0) {
                    targetBook->borrowBook();
                    const int borrowDaysLimit = 15; 
                    string currentDate = getLibrarianCurrentDate();
                    string dueDate = calculateDueDate(currentDate, borrowDaysLimit); 
                    it->setStatus(1);
                    it->setBorrowDate(currentDate);
                    it->setDueDate(dueDate); 

                    cout << "Da duyet thanh cong phieu muon " << recordIdToProcess << "." << endl;
                    cout << "Ngay muon: " << currentDate << " | Han tra: " << dueDate << endl;
                } else {
                    cout << "LOI: Sach '" << targetBook->getTitle() << "' da het." << endl;
                    cout << "He thong se tu dong huy yeu cau nay." << endl;
                    records.erase(it); 
                }

            } else if (actionChoice == 2) { 
                
                records.erase(it);
                cout << "Da huy phieu muon " << recordIdToProcess << " theo quyet dinh cua Thu thu." << endl;

            } else if (actionChoice == 0) { 
                return; 
            
            } else {
                cout << "Lua chon khong hop le." << endl;
            }
            
            return; 
        }
    }

    cout << "Khong tim thay ID Phieu muon: " << recordIdToProcess << " (hoac da duoc xu ly)." << endl;
}
void LibrarianService::collectFines(vector<Person*>& users) {
    Menu::clearScreen();
    cout << "--- Thu Tien Phat Cua Sinh Vien ---" << endl;
    cout << "--- Danh Sach Sinh Vien Dang No ---" << endl;
    
    bool hasDebt = false;
    cout << "----------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(15) << "MSSV"
         << "| " << left << setw(25) << "Ten Sinh Vien"
         << "| " << left << setw(22) << "So Tien No (VND)" << " |" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    for (auto* user : users) {
        if (Student* s = dynamic_cast<Student*>(user)) {
            if (s->getFineAmount() > 0) {
                cout << "| " << left << setw(15) << s->getStudentId()
                     << "| " << left << setw(25) << s->getName()
                     << "| " << left << setw(22) << fixed << setprecision(0) << s->getFineAmount() << " |" << endl;
                hasDebt = true;
            }
        }
    }

    if (!hasDebt) {
        cout << "| " << left << setw(66) << "Khong co sinh vien nao no tien phat." << " |" << endl;
        cout << "----------------------------------------------------------------------" << endl;
        return;
    }
    cout << "----------------------------------------------------------------------" << endl;

    cout << "\nNhap Ma So Sinh Vien (MSSV) ban muon xoa no: ";
    string mssv = Menu::getStringNoSpaces();
    
    Student* targetStudent = nullptr;
    for (auto* user : users) {
        if (Student* s = dynamic_cast<Student*>(user)) {
            if (s->getStudentId() == mssv) {
                targetStudent = s;
                break;
            }
        }
    }

    if (targetStudent == nullptr || targetStudent->getFineAmount() == 0) {
        cout << "Khong tim thay sinh vien co no voi MSSV: " << mssv << endl;
        return;
    }
    
    double currentDebt = targetStudent->getFineAmount();
    cout << "Tim thay sinh vien: " << targetStudent->getName() << " | No: " << currentDebt << " VND" << endl;
    
    cout << "\nBan co chac chan muon xac nhan sinh vien da thanh toan TOAN BO so no nay khong?" << endl;
    cout << "1. Co (Xoa no)" << endl;
    cout << "0. Khong (Huy)" << endl;
    cout << "Lua chon: ";
    int choice = Menu::getIntegerInput();
    
    if (choice == 1) {
        targetStudent->payFine(currentDebt); 
        cout << "Xoa no thanh cong! So no con lai: 0 VND." << endl;
    } else {
        cout << "Da huy thao tac xoa no." << endl;
    }
}


void LibrarianService::changePassword(Librarian* librarian) {
    cout << "--- Doi Mat Khau ---" << endl;
    cout << "Nhap mat khau cu: ";
    string oldPass = Menu::getStringNoSpaces();

    if (oldPass != librarian->getPassword()) {
        cout << "Mat khau cu không dung. Thao tac bi huy." << endl;
        return;
    }

    string newPass = Menu::getStringPassword(); // Hàm này đã có validation
    
    cout << "Xac nhan mat khau moi: ";
    string confirmPass = Menu::getStringNoSpaces();

    if (newPass != confirmPass) {
        cout << "Mat khau xac nhan không khop. Thao tac bi huy." << endl;
        return;
    }

    librarian->setPassword(newPass);
    cout << "Doi mat khau thanh cong!" << endl;
}