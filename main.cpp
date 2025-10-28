#include <iostream>
#include <vector>
#include <string>
#include <limits> // Cần thư viện này cho cin.ignore

#include "Person.h"
#include "Admin.h"
#include "Librarian.h"
#include "Student.h"
#include "Book.h"
#include "BorrowingRecord.h"
#include "DataManager.h"
#include "AuthenticationService.h"
#include "AdminService.h"
#include "LibrarianService.h"
#include "StudentService.h"
#include "Menu.h"

using namespace std;

void initializeDefaultAdmin(vector<Person*>& users) {
    bool adminExists = false;
    for (const auto& user : users) {
        if (user->getEmail() == "admin@lib.com") {
            adminExists = true;
            break;
        }
    }
    if (!adminExists) {
        users.push_back(new Admin(101, "Admin", "admin@lib.com", "admin123"));
    }
}

int main() {
    vector<Person*> allUsers;
    vector<Book> allBooks;
    vector<BorrowingRecord> allRecords;

    DataManager dataManager("users.csv", "books.csv", "records.csv");
    AuthenticationService authService;
    AdminService adminService;
    LibrarianService librarianService;
    StudentService studentService;

    dataManager.loadAllData(allUsers, allBooks, allRecords);
    initializeDefaultAdmin(allUsers);

    int mainChoice = 0;
    while (mainChoice!= 3) {
        Menu::displayMainMenu();
        mainChoice = Menu::getIntegerInput();

        // FIX: Thêm dòng này để dọn dẹp ký tự Enter còn sót lại trong buffer
        // sau khi nhập số ở menu, trước khi chuyển sang nhập chuỗi (email/password).
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (mainChoice) {
            case 1: {
                Menu::clearScreen();
                cout << "--- DANG NHAP ---" << endl;
                cout << "Email: ";
                string email;
                getline(cin, email); // Bây giờ hàm này sẽ chờ bạn nhập liệu
                cout << "Mat khau: ";
                string password;
                getline(cin, password);

                Person* loggedInUser = authService.login(email, password, allUsers);

                if (loggedInUser!= nullptr) {
                    cout << "Dang nhap thanh cong!" << endl;
                    Menu::pause();
                    
                    if (Admin* admin = dynamic_cast<Admin*>(loggedInUser)) {
                        adminService.run(admin, allUsers, allBooks, dataManager);
                    } else if (Librarian* librarian = dynamic_cast<Librarian*>(loggedInUser)) {
                        librarianService.run(librarian, allBooks, dataManager);
                    } else if (Student* student = dynamic_cast<Student*>(loggedInUser)) {
                        studentService.run(student, allBooks, allRecords, dataManager);
                    }
                } else {
                    // Bây giờ thông báo này sẽ hiện ra một cách chính xác
                    cout << "Email hoac mat khau khong dung." << endl;
                    Menu::pause();
                }
                break;
            }
            case 2: {
                Menu::clearScreen();
                authService.registerStudent(allUsers, dataManager);
                Menu::pause();
                break;
            }
            case 3: {
                cout << "Dang luu du lieu..." << endl;
                dataManager.saveAllData(allUsers, allBooks, allRecords);
                cout << "Cam on ban da su dung he thong. Hen gap lai!" << endl;
                break;
            }
            default: {
                cout << "Lua chon khong hop le. Vui long chon lai." << endl;
                Menu::pause();
                break;
            }
        }
    }

    for (Person* user : allUsers) {
        delete user;
    }
    allUsers.clear();

    return 0;
}