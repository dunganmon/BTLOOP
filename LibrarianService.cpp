#include "LibrarianService.h"
#include "Menu.h"
#include "Newspaper.h"
#include "Magazine.h"
#include "Textbook.h"
#include "Novel.h"

void LibrarianService::run(Librarian* librarian, vector<Book*>& books, DataManager& dataManager) {
    int choice = 0;
    while (choice!= 7) { 
        Menu::displayLibrarianMenu(librarian->getName());
        choice = Menu::getIntegerInput();

        switch (choice) {
            case 1: addBook(books, dataManager); break;
            case 2: editBook(books); break;
            case 3: Menu::viewAllBooks(books); break;
            case 4: searchBook(books); break;
            case 5: Menu::displayBooksByType(books); break;  
            case 6: Menu::displaySortedBooks(books); break; 
            case 7: cout << "Dang xuat..." << endl; break;
            default: cout << "Lua chon khong hop le. Vui long chon lai." << endl; break;
        }
        if (choice!= 7) Menu::pause();
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