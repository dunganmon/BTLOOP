// LibrarianService.cpp
#include "LibrarianService.h"
#include "Menu.h"

void LibrarianService::run(Librarian* librarian, vector<Book>& books, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 5) {
        Menu::displayLibrarianMenu(librarian->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: addBook(books, dataManager); break;
            case 2: editBookInfo(books); break;
            case 3: Menu::viewAllBooks(books); break;
            case 4: searchBook(books); break;
            case 5: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 5) Menu::pause();
    }
}

void LibrarianService::addBook(vector<Book>& books, DataManager& dataManager) {
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

void LibrarianService::editBookInfo(vector<Book>& books) {
    cout << "--- Chinh Sua Thong Tin Sach ---" << endl;
    cout << "Nhap ID sach can chinh sua: ";
    int idToEdit = Menu::getIntegerInput();

    for (auto& book : books) {
        if (book.getId() == idToEdit) {
            string newTitle, newAuthor;
            cout << "Nhap tieu de moi (de trong de giu nguyen): ";
            getline(cin >> ws, newTitle);
            cout << "Nhap tac gia moi (de trong de giu nguyen): ";
            getline(cin >> ws, newAuthor);

            if (!newTitle.empty()) book.setTitle(newTitle);
            if (!newAuthor.empty()) book.setAuthor(newAuthor);
            
            cout << "Cap nhat thong tin sach thanh cong!" << endl;
            return;
        }
    }
    cout << "Khong tim thay sach voi ID " << idToEdit << "." << endl;
}

void LibrarianService::searchBook(const vector<Book>& books) {
    Menu::searchAndDisplayBooks(books);
}