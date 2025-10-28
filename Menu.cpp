#include "Menu.h"
#include <limits> // for numeric_limits

void Menu::clearScreen() {
    // Lệnh "cls" hoạt động trên Windows.
    // Trên Linux/macOS, sử dụng "clear".
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Menu::pause() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Nếu dòng trên không hoạt động ổn định, dùng 2 dòng dưới
    // cin.sync();
    // cin.get();
}

int Menu::getIntegerInput() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Vui long nhap mot so nguyen hop le: ";
        cin.clear(); // Xóa trạng thái lỗi của cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Loại bỏ input không hợp lệ
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Loại bỏ các ký tự thừa như Enter
    return choice;
}

void Menu::displayMainMenu() {
    clearScreen();
    cout << "========================================" << endl;
    cout << "|| CHAO MUNG DEN VOI HE THONG THU VIEN ||" << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Dang Nhap ||" << endl;
    cout << "|| 2. Dang Ky (danh cho Sinh vien) ||" << endl;
    cout << "|| 3. Thoat ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayAdminMenu(const string& adminName) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "|| MENU ADMIN - " << adminName << " ||" << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Them Sach Moi ||" << endl;
    cout << "|| 2. Chinh Sua Thong Tin Sach ||" << endl;
    cout << "|| 3. Xoa Sach ||" << endl;
    cout << "|| 4. Them Tai Khoan Thu Thu ||" << endl;
    cout << "|| 5. Xem Tat Ca Tai Khoan ||" << endl;
    cout << "|| 6. Xoa Tai Khoan Nguoi Dung ||" << endl;
    cout << "|| 7. Xem Toan Bo Sach ||" << endl;
    cout << "|| 8. Dang Xuat ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayLibrarianMenu(const string& librarianName) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "|| MENU THU THU - " << librarianName << " ||" << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Them Sach Moi ||" << endl;
    cout << "|| 2. Chinh Sua Thong Tin Sach ||" << endl;
    cout << "|| 3. Xem Toan Bo Sach ||" << endl;
    cout << "|| 4. Tim Kiem Sach ||" << endl;
    cout << "|| 5. Dang Xuat ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::displayStudentMenu(const string& studentName) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "|| MENU SINH VIEN - " << studentName << " ||" << endl;
    cout << "========================================" << endl;
    cout << "|| 1. Tim Kiem Sach ||" << endl;
    cout << "|| 2. Xem Toan Bo Sach ||" << endl;
    cout << "|| 3. Muon Sach ||" << endl;
    cout << "|| 4. Tra Sach ||" << endl;
    cout << "|| 5. Bao Mat Sach (co phat) ||" << endl;
    cout << "|| 6. Xem Thong Tin Ca Nhan & Sach Muon ||" << endl;
    cout << "|| 7. Bao Tim Duoc Sach Da Mat ||" << endl;
    cout << "|| 8. Dang Xuat ||" << endl;
    cout << "========================================" << endl;
    cout << "Vui long chon: ";
}

void Menu::viewAllBooks(const vector<Book>& books) {
    clearScreen();
    cout << "--- DANH SACH TOAN BO SACH TRONG THU VIEN ---" << endl;
    if (books.empty()) {
        cout << "Thu vien hien chua co sach nao." << endl;
    } else {
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "| " << left << setw(8) << "ID Sach"
             << "| " << left << setw(35) << "Tieu de"
             << "| " << left << setw(25) << "Tac gia"
             << "| " << left << setw(10) << "Con lai"
             << "| " << left << setw(14) << "Tong so luong" << " |" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        
        for (const auto& book : books) {
            book.display(); 
        }

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    }
}


void Menu::searchAndDisplayBooks(const vector<Book>& books) {
    cout << "Nhap tu khoa tim kiem (tieu de hoac tac gia): ";
    string keyword;
    getline(cin >> ws, keyword);
    
    for(char &c : keyword) { c = tolower(c); }

    clearScreen();
    cout << "--- KET QUA TIM KIEM CHO '" << keyword << "' ---" << endl;
    bool found = false;
    
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(8) << "ID Sach"
         << "| " << left << setw(34) << "Tieu de"
         << "| " << left << setw(24) << "Tac gia"
         << "| " << left << setw(10) << "Con lai"
         << "| " << left << setw(15) << "Tong so luong" << " |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;

    for (const auto& book : books) {
        string title = book.getTitle();
        string author = book.getAuthor();
        for(char &c : title) { c = tolower(c); }
        for(char &c : author) { c = tolower(c); }

        if (title.find(keyword)!= string::npos || author.find(keyword)!= string::npos) {
            book.display();
            found = true;
        }
    }
    
    if (!found) {
        cout << "| " << left << setw(102) << "Khong tim thay sach nao phu hop." << " |" << endl;
    }
    
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
}