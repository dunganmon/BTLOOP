#include "AdminService.h"
#include "Menu.h" 
#include "Newspaper.h"
#include "Magazine.h"
#include "Textbook.h"
#include "Novel.h"
#include <limits>
#include <iomanip>
#include <ctime> 
#include <sstream>
string getAdminCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-" 
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-" 
       << setfill('0') << setw(2) << ltm->tm_mday;
    return ss.str();
}
void AdminService::run(Admin* admin, vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 12) { 
        Menu::displayAdminMenu(admin->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: addBook(books, dataManager); break;
            case 2: editBook(books); break;
            case 3: deleteBook(books, records); break;
            case 4: Menu::viewAllBooks(books); break;
            case 5: Menu::displayBooksByType(books); break;  
            case 6: Menu::displaySortedBooks(books); break; 
            case 7: addLibrarian(users, dataManager); break;
            case 8: viewAllUsers(users); break;
            case 9: deleteUser(users, books, records); break;
            case 10: importBooks(books, dataManager); break; 
            case 11: importUsers(users, dataManager); break;
            case 12: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 12) Menu::pause();
    }
}
void AdminService::addLibrarian(vector<Person*>& users, DataManager& dataManager) {
    string name, email, password, employeeId;
    cout << "--- Them Thu Thu Moi ---" << endl;
    cout << "Nhap ho ten: ";
    getline(cin >> ws, name);
    bool emailIsValid;
    do {
        emailIsValid = true; 
        cout << "Nhap email: ";
        email = Menu::getStringNoSpaces();

        if (!Menu::isValidEmailFormat(email)) {
            cout << "Loi: Dinh dang email khong hop le (vd: user@domain.com)." << endl;
            emailIsValid = false;
            continue;
        }

        for (const auto& user : users) {
            if (user->getEmail() == email) {
                cout << "Loi: Email nay da duoc su dung. Vui long chon email khac." << endl;
                emailIsValid = false;
                break; 
            }
        }

    } while (!emailIsValid); 
    
    password = Menu::getStringPassword();
    
    bool employeeIdIsValid;
    do {
        employeeIdIsValid = true;
        cout << "Nhap ma nhan vien: ";
        employeeId = Menu::getStringNoSpaces();
        for (const auto& user : users) {
            if (Librarian* lib = dynamic_cast<Librarian*>(user)) {
                if (lib->getEmployeeId() == employeeId) {
                    cout << "Loi: Ma nhan vien nay da duoc su dung. Vui long nhap lai." << endl;
                    employeeIdIsValid = false;
                    break; 
                }
            }
        }
    } while (!employeeIdIsValid); 

    int newId = dataManager.getNextUserId();
    users.push_back(new Librarian(newId, name, email, password, employeeId));
    cout << "Them thu thu thanh cong!" << endl;
}

void AdminService::viewAllUsers(const vector<Person*>& users) {
    cout << "--- Danh Sach Tat Ca Nguoi Dung ---" << endl;
    
 
    cout << "-------------------------------------------------------------------------------------------------------------" << endl;
   
    cout << "| " << left << setw(8) << "ID"
         << "| " << left << setw(20) << "Ho Ten"
         << "| " << left << setw(25) << "Email"
         << "| " << left << setw(15) << "Vai tro"
         << "| " << left << setw(25) << "Ma NV/MSSV" << " |" << endl;
    
    cout << "-------------------------------------------------------------------------------------------------------------" << endl;
    
    for (const auto& user : users) {
        user->displayInfo();
    }
    

    cout << "-------------------------------------------------------------------------------------------------------------" << endl;
}

void AdminService::deleteUser(vector<Person*>& users, vector<Book*>& books, vector<BorrowingRecord>& records) {
    viewAllUsers(users);
    cout << "--- Xoa Nguoi Dung ---" << endl;
    cout << "Nhap ID nguoi dung can xoa: ";
    int idToDelete = Menu::getIntegerInput();

    if (idToDelete == 101) { 
        cout << "Khong the xoa tai khoan Admin mac dinh." << endl;
        return;
    }

    bool isBorrowing = false;
    for (auto& record : records) {
        if (record.getStudentId() == idToDelete && record.getStatus() == 0) {
            isBorrowing = true;
            record.setStatus(1); 
            record.setReturnDate(getAdminCurrentDate()); 
            
            for (auto* book : books) {
                if (book->getId() == record.getBookId()) {
                    book->returnBook();
                    break; 
                }
            }
        }
    }

    if (isBorrowing) {
        cout << "Luu y: Tai khoan nay dang muon sach. He thong da tu dong tra lai sach." << endl;
    }

    for (auto it = users.begin(); it!= users.end(); ++it) {
        if ((*it)->getId() == idToDelete) {
            delete *it; 
            users.erase(it);
            cout << "Da xoa nguoi dung co ID " << idToDelete << "." << endl;
            return;
        }
    }
    cout << "Khong tim thay nguoi dung voi ID " << idToDelete << "." << endl;
}

void AdminService::addBook(vector<Book*>& books, DataManager& dataManager) {
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
void AdminService::editBook(vector<Book*>& books) {
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

void AdminService::deleteBook(vector<Book*>& books, const vector<BorrowingRecord>& records) {
    Menu::viewAllBooks(books);
    cout << "--- Xoa Sach ---" << endl;
    cout << "Nhap ID sach can xoa: ";
    int idToDelete = Menu::getIntegerInput();

    bool isBorrowed = false;
    for (const auto& record : records) {
        if (record.getBookId() == idToDelete && record.getStatus() == 0) { 
            isBorrowed = true;
            break;
        }
    }

    if (isBorrowed) {
        cout << "Loi: Khong the xoa sach nay. Hien dang co sinh vien muon." << endl;
        return;
    }

    for (auto it = books.begin(); it!= books.end(); ++it) {
        if ((*it)->getId() == idToDelete) {
            delete *it; 
            books.erase(it);
            cout << "Da xoa sach co ID " << idToDelete << "." << endl;
            return;
        }
    }
    cout << "Khong tim thay sach voi ID " << idToDelete << "." << endl;
}

void AdminService::importBooks(vector<Book*>& books, DataManager& dataManager) {
    cout << "--- Nhap Sach Hang Loat (CSV) ---" << endl;
    cout << "Luu y: File CSV phai co 5 cot theo dung thu tu:" << endl;
    cout << "LoaiSach(1-4),TieuDe,TacGia,NamXuatBan,TongSoLuong" << endl;
    cout << "\nNhap ten file (vd: data/import_books.csv): ";
    
    string filename;
    getline(cin >> ws, filename); 

    dataManager.importBooksFromFile(filename, books);
}

void AdminService::importUsers(vector<Person*>& users, DataManager& dataManager) {
    cout << "--- Nhap User Hang Loat (CSV) ---" << endl;
    cout << "Luu y: File CSV phai co dinh dang:" << endl;
    cout << "Thu Thu:     Role(2),Ten,Email,MatKhau,MaNV" << endl;
    cout << "Sinh Vien:   Role(3),Ten,Email,MatKhau,MSSV,Lop,Khoa" << endl;
    cout << "\nNhap ten file (vd: data/import_users.csv): ";
    
    string filename;
    getline(cin >> ws, filename);

    dataManager.importUsersFromFile(filename, users);
}