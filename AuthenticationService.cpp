// AuthenticationService.cpp
#include "AuthenticationService.h"
#include <iostream>

Person* AuthenticationService::login(const string& email, const string& password, const vector<Person*>& users) {
    for (Person* user : users) {
        if (user->getEmail() == email && user->getPassword() == password) {
            return user; // Trả về con trỏ tới người dùng được tìm thấy
        }
    }
    return nullptr; // Không tìm thấy người dùng
}

void AuthenticationService::registerStudent(vector<Person*>& users, DataManager& dataManager) {
    string name, email, password, studentId, studentClass;
    int courseYear;

    cout << "--- Dang Ky Tai Khoan Sinh Vien ---" << endl;
    cout << "Nhap ho ten: ";
    getline(cin >> ws, name);
    cout << "Nhap email: ";
    cin >> email;

    // Kiểm tra email đã tồn tại chưa
    for (const auto& user : users) {
        if (user->getEmail() == email) {
            cout << "Loi: Email nay da duoc su dung. Vui long chon email khac." << endl;
            return;
        }
    }

    cout << "Nhap mat khau: ";
    cin >> password;
    cout << "Nhap Ma so sinh vien: ";
    cin >> studentId;
    cout << "Nhap Lop: ";
    cin >> studentClass;
    cout << "Nhap Khoa hoc (VD: 2022): ";
    cin >> courseYear;

    int newId = dataManager.getNextUserId();
    Student* newStudent = new Student(newId, name, email, password, studentId, studentClass, courseYear, 0.0);
    users.push_back(newStudent);

    cout << "Dang ky thanh cong! Vui long dang nhap de su dung he thong." << endl;
}