#include "AdminService.h"
#include "Menu.h" // Sẽ được tạo ở phần sau
#include <limits>
#include <iomanip>
void AdminService::run(Admin* admin, vector<Person*>& users, vector<Book>& books, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 8) {
        Menu::displayAdminMenu(admin->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: addBook(books, dataManager); break;
            case 2: editBook(books); break;
            case 3: deleteBook(books); break;
            case 4: addLibrarian(users, dataManager); break;
            case 5: viewAllUsers(users); break;
            case 6: deleteUser(users); break;
            case 7: Menu::viewAllBooks(books); break;
            case 8: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 8) Menu::pause();
    }
}

void AdminService::addLibrarian(vector<Person*>& users, DataManager& dataManager) {
    string name, email, password, employeeId;
    cout << "--- Them Thu Thu Moi ---" << endl;
    cout << "Nhap ho ten: ";
    getline(cin >> ws, name);
    cout << "Nhap email: ";
    cin >> email;
    cout << "Nhap mat khau: ";
    cin >> password;
    cout << "Nhap ma nhan vien: ";
    cin >> employeeId;

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

void AdminService::deleteUser(vector<Person*>& users) {
    cout << "--- Xoa Nguoi Dung ---" << endl;
    cout << "Nhap ID nguoi dung can xoa: ";
    int idToDelete = Menu::getIntegerInput();

    if (idToDelete == 101) { // Giả sử ID của admin mặc định là 101
        cout << "Khong the xoa tai khoan Admin mac dinh." << endl;
        return;
    }

    for (auto it = users.begin(); it!= users.end(); ++it) {
        if ((*it)->getId() == idToDelete) {
            delete *it; // Giải phóng bộ nhớ
            users.erase(it);
            cout << "Da xoa nguoi dung co ID " << idToDelete << "." << endl;
            return;
        }
    }
    cout << "Khong tim thay nguoi dung voi ID " << idToDelete << "." << endl;
}

void AdminService::addBook(vector<Book>& books, DataManager& dataManager) {
    string title, author;
    int quantity;
    cout << "--- Them Sach Moi ---" << endl;
    cout << "Nhap tieu de sach: ";
    getline(cin >> ws, title);
    cout << "Nhap tac gia: ";
    getline(cin >> ws, author);
    cout << "Nhap so luong: ";
    quantity = Menu::getIntegerInput();

    int newId = dataManager.getNextBookId();
    books.push_back(Book(newId, title, author, quantity, quantity));
    cout << "Them sach moi thanh cong!" << endl;
}

void AdminService::editBook(vector<Book>& books) {
    cout << "--- Chinh Sua Thong Tin Sach ---" << endl;
    cout << "Nhap ID sach can chinh sua: ";
    int idToEdit = Menu::getIntegerInput();

    for (auto& book : books) {
        if (book.getId() == idToEdit) {
            string newTitle, newAuthor;
            int newTotal;
            cout << "Nhap tieu de moi (de trong de giu nguyen): ";
            getline(cin >> ws, newTitle);
            cout << "Nhap tac gia moi (de trong de giu nguyen): ";
            getline(cin >> ws, newAuthor);
            cout << "Nhap tong so luong moi (-1 de giu nguyen): ";
            cin >> newTotal;

            if (!newTitle.empty()) book.setTitle(newTitle);
            if (!newAuthor.empty()) book.setAuthor(newAuthor);
            if (newTotal!= -1) {
                // Cập nhật số lượng còn lại một cách hợp lý
                int diff = newTotal - book.getTotalQuantity();
                book.setTotalQuantity(newTotal);
                book.setAvailableQuantity(book.getAvailableQuantity() + diff);
                if (book.getAvailableQuantity() < 0) book.setAvailableQuantity(0);
                if (book.getAvailableQuantity() > newTotal) book.setAvailableQuantity(newTotal);
            }
            cout << "Cap nhat thong tin sach thanh cong!" << endl;
            return;
        }
    }
    cout << "Khong tim thay sach voi ID " << idToEdit << "." << endl;
}

void AdminService::deleteBook(vector<Book>& books) {
    cout << "--- Xoa Sach ---" << endl;
    cout << "Nhap ID sach can xoa: ";
    int idToDelete = Menu::getIntegerInput();

    for (auto it = books.begin(); it!= books.end(); ++it) {
        if (it->getId() == idToDelete) {
            books.erase(it);
            cout << "Da xoa sach co ID " << idToDelete << "." << endl;
            return;
        }
    }
    cout << "Khong tim thay sach voi ID " << idToDelete << "." << endl;
}