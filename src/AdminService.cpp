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
    while (choice!= 9) { 
        Menu::displayAdminMenu(admin->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: Menu::viewAllBooks(books); break;
            case 2: Menu::displayBooksByType(books); break;  
            case 3: Menu::displaySortedBooks(books); break; 
            case 4: addLibrarian(users, dataManager); break;
            case 5: viewAllUsers(users); break;
            case 6: deleteUser(users, books, records); break;
            case 7: importBooks(books, dataManager); break; 
            case 8: importUsers(users, dataManager); break;
            case 9: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 9) Menu::pause();
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

    bool isPending = false;
    for (auto it = records.begin(); it != records.end();) {
        if (it->getStudentId() == idToDelete && it->getStatus() == 3) {
            it = records.erase(it); 
            isPending = true;
        } else {
            ++it; 
        }
    }
    if (isPending) {
        cout << "Luu y: Tai khoan nay co yeu cau dang cho. He thong da tu dong huy cac yeu cau." << endl;
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