#include "Menu.h"
#include <limits> 
#include <algorithm> 
#include <iomanip>   
#include <cctype>
void Menu::printBookTableHeader() {
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(12) << "Loai Sach"
         << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(35) << "Tieu de"
         << "| " << left << setw(25) << "Tac gia"
         << "| " << left << setw(5) << "Nam"
         << "| " << left << setw(10) << "Con lai"
         << "| " << left << setw(14) << "Tong so luong" << " |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Menu::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #endif
}

void Menu::pause() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int Menu::getIntegerInput() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Vui long nhap mot so nguyen hop le: ";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    return choice;
}

string Menu::getStringNoSpaces() {
    string line;
    while (true) {
        getline(cin >> ws, line); 
        if (line.empty()) {
            cout << "Loi: Khong duoc de trong. Vui long nhap lai: ";
            continue; 
        }
        if (line.find(' ') != string::npos) {
            cout << "Loi: Khong duoc chua dau cach. Vui long nhap lai: ";
            continue; 
        }
        return line;
    }
}

bool Menu::isValidEmailFormat(const string& email) {
    size_t atPos = email.find('@');
    size_t lastAtPos = email.rfind('@');  
    size_t dotPos = email.rfind('.');

    if (atPos == string::npos || atPos == 0 || 
        dotPos == string::npos || dotPos <= atPos + 1 || 
        dotPos == email.length() - 1 ||
        atPos != lastAtPos) 
    {
        return false;
    }
    return true;
}

string Menu::getStringPassword() {
    string pass;
    while (true) {
        cout << "Nhap mat khau: ";
        getline(cin >> ws, pass);

        if (pass.length() < 6) {
            cout << "Loi: Mat khau phai co it nhat 6 ky tu." << endl;
            continue;
        }

        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;

        for (char c : pass) {
            if (isupper(c)) hasUpper = true;
            else if (islower(c)) hasLower = true;
            else if (isdigit(c)) hasDigit = true;
        }

        if (pass.find(' ') != string::npos) {
             cout << "Loi: Mat khau khong duoc chua dau cach." << endl;
             continue;
        }

        if (hasUpper && hasLower && hasDigit) {
            return pass; 
        } else {
            cout << "Loi: Mat khau phai chua it nhat 1 chu hoa, 1 chu thuong, va 1 chu so." << endl;
        }
    }
}

int Menu::getPublicationYearInput() {
    int year;
    while (true) {
        cout << "Nhap nam xuat ban (<= 2025): ";
        while (!(cin >> year)) {
            cout << "Vui long nhap mot so nguyen hop le: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (year > 2025 || year <= 0) {
            cout << "Loi: Nam xuat ban phai la so duong va khong lon hon 2025." << endl;
        } else {
            return year;
        }
    }
}

int Menu::getBookTypeInput() {
    int choice;
    while (true) {
        cout << "Chon loai sach:" << endl;
        cout << "1. Bao" << endl;
        cout << "2. Tap chi" << endl;
        cout << "3. Giao trinh" << endl;
        cout << "4. Tieu thuyet" << endl;
        cout << "Lua chon cua ban: ";
        
        while (!(cin >> choice)) {
            cout << "Vui long nhap mot so nguyen hop le: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice >= 1 && choice <= 4) {
            return choice;
        } else {
            cout << "Lua chon khong hop le. Vui long chon tu 1 den 4." << endl;
        }
    }
}



void Menu::displayMainMenu() {
    clearScreen();
    cout << "========================================" << endl;
    cout << "  CHAO MUNG DEN VOI HE THONG THU VIEN   " << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Dang Nhap                       ||" << endl;
    cout << "|| 2. Dang Ky (danh cho Sinh vien)    ||" << endl;
    cout << "|| 3. Thoat                           ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayAdminMenu(const string& adminName) {
    clearScreen();
    cout << "==========================================" << endl;
    cout << "   MENU ADMIN - " << adminName            << endl;
    cout << "==========================================" << endl;
    cout << "|| 1. Them Sach Moi                     ||" << endl;
    cout << "|| 2. Chinh Sua Thong Tin Sach          ||" << endl;
    cout << "|| 3. Xoa Sach                          ||" << endl;
    cout << "|| 4. Xem Toan Bo Sach                  ||" << endl;
    cout << "|| 5. Xem Sach Theo Loai                ||" << endl; 
    cout << "|| 6. Xem Sach Sap Xep                  ||" << endl; 
    cout << "|| 7. Them Tai Khoan Thu Thu            ||" << endl;
    cout << "|| 8. Xem Tat Ca Tai Khoan              ||" << endl;
    cout << "|| 9. Xoa Tai Khoan Nguoi Dung          ||" << endl;
    cout << "|| 10. Nhap Sach Hang Loat (CSV)        ||" << endl; 
    cout << "|| 11. Nhap User Hang Loat (CSV)        ||" << endl; 
    cout << "|| 12. Dang Xuat                        ||" << endl; 
    cout << "==========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayLibrarianMenu(const string& librarianName) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "   MENU THU THU - " << librarianName      << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Them Sach Moi                   ||" << endl;
    cout << "|| 2. Chinh Sua Thong Tin Sach        ||" << endl;
    cout << "|| 3. Xem Toan Bo Sach                ||" << endl;
    cout << "|| 4. Tim Kiem Sach                   ||" << endl;
    cout << "|| 5. Xem Sach Theo Loai              ||" << endl; 
    cout << "|| 6. Xem Sach Sap Xep                ||" << endl; 
    cout << "|| 7. Dang Xuat                       ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayStudentMenu(const string& studentName) {
    clearScreen();
    cout << "==========================================" << endl;
    cout << "   MENU SINH VIEN - " << studentName        << endl;
    cout << "==========================================" << endl;
    cout << "|| 1. Tim Kiem Sach                     ||" << endl;
    cout << "|| 2. Xem Toan Bo Sach                  ||" << endl;
    cout << "|| 3. Xem Sach Theo Loai                ||" << endl; 
    cout << "|| 4. Xem Sach Sap Xep                  ||" << endl; 
    cout << "|| 5. Muon Sach                         ||" << endl;
    cout << "|| 6. Tra Sach                          ||" << endl;
    cout << "|| 7. Bao Mat Sach (co phat)            ||" << endl;
    cout << "|| 8. Bao Tim Duoc Sach Da Mat          ||" << endl;
    cout << "|| 9. Xem Thong Tin Ca Nhan & Sach Muon ||" << endl;
    cout << "|| 10. Dang Xuat                        ||" << endl;
    cout << "==========================================" << endl;
    cout << "Vui long chon: ";
}


void Menu::viewAllBooks(const vector<Book*>& books) {
    clearScreen();
    cout << "--- DANH SACH TOAN BO SACH TRONG THU VIEN ---" << endl;
    if (books.empty()) {
        cout << "Thu vien hien chua co sach nao." << endl;
    } else {
        printBookTableHeader(); 
        
        for (const auto& book : books) {
            book->display(); 
        }

        cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
    }
}

void Menu::searchAndDisplayBooks(const vector<Book*>& books) {
    cout << "Nhap tu khoa tim kiem (tieu de hoac tac gia): ";
    string keyword;
    getline(cin >> ws, keyword);
    
    for(char &c : keyword) { c = tolower(c); }

    clearScreen();
    cout << "--- KET QUA TIM KIEM CHO '" << keyword << "' ---" << endl;
    bool found = false;
    
    printBookTableHeader(); 

    for (const auto& book : books) {
        string title = book->getTitle();
        string author = book->getAuthor();
        for(char &c : title) { c = tolower(c); }
        for(char &c : author) { c = tolower(c); }

        if (title.find(keyword)!= string::npos || author.find(keyword)!= string::npos) {
            book->display();
            found = true;
        }
    }
    
    if (!found) {
        cout << "| " << left << setw(118) << "Khong tim thay sach nao phu hop." << " |" << endl;
    }
    
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Menu::displayBooksByType(const vector<Book*>& books) {
    int type = getBookTypeInput();
    string typeName;
    if (type == 1) typeName = "Bao";
    else if (type == 2) typeName = "Tap chi";
    else if (type == 3) typeName = "Giao trinh";
    else if (type == 4) typeName = "Tieu thuyet";

    clearScreen();
    cout << "--- DANH SACH SACH LOAI: " << typeName << " ---" << endl;
    bool found = false;
    
    printBookTableHeader();

    for (const auto& book : books) {
        if (book->getBookType() == type) {
            book->display();
            found = true;
        }
    }

    if (!found) {
        cout << "| " << left << setw(118) << "Khong tim thay sach nao thuoc loai nay." << " |" << endl;
    }
    
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Menu::displaySortedBooks(const vector<Book*>& books) {
    if (books.empty()) {
        cout << "Thu vien hien chua co sach nao de sap xep." << endl;
        return;
    }

    int sortChoice;
    while (true) {
        cout << "Ban muon sap xep theo:" << endl;
        cout << "1. Nam xuat ban" << endl;
        cout << "2. So luong con lai" << endl;
        cout << "Lua chon: ";
        sortChoice = getIntegerInput();
        
        if (sortChoice == 1 || sortChoice == 2) {
            break; 
        } else {
            cout << "Lua chon khong hop le. Vui long chi chon 1 hoac 2." << endl;
        }
    }

    int orderChoice;
    while (true) {
        cout << "Chon kieu sap xep:" << endl;
        cout << "1. Tang dan (ASC)" << endl;
        cout << "2. Giam dan (DESC)" << endl;
        cout << "Lua chon: ";
        orderChoice = getIntegerInput();
        
        if (orderChoice == 1 || orderChoice == 2) {
            break; 
        } else {
            cout << "Lua chon khong hop le. Vui long chi chon 1 hoac 2." << endl;
        }
    }

    vector<Book*> sortedBooks = books;

    if (sortChoice == 1) { 
        sort(sortedBooks.begin(), sortedBooks.end(), [orderChoice](Book* a, Book* b) {
            if (orderChoice == 1) { 
                return a->getPublicationYear() < b->getPublicationYear();
            } else { 
                return a->getPublicationYear() > b->getPublicationYear();
            }
        });
    } else if (sortChoice == 2) { 
        sort(sortedBooks.begin(), sortedBooks.end(), [orderChoice](Book* a, Book* b) {
            if (orderChoice == 1) { 
                return a->getAvailableQuantity() < b->getAvailableQuantity();
            } else { 
                return a->getAvailableQuantity() > b->getAvailableQuantity();
            }
        });
    } else {
        cout << "Lua chon sap xep khong hop le." << endl;
        return;
    }

    viewAllBooks(sortedBooks);
}