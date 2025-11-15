#include "AuthenticationService.h"
#include "Menu.h"
#include <iostream>

Person* AuthenticationService::login(const string& email, const string& password, const vector<Person*>& users) {
    for (Person* user : users) {
        if (user->getEmail() == email && user->getPassword() == password) {
            return user; 
        }
    }
    return nullptr; 
}

void AuthenticationService::registerStudent(vector<Person*>& users, DataManager& dataManager) {
    string name, email, password, studentId, studentClass;
    int courseYear;

    cout << "--- Dang Ky Tai Khoan Sinh Vien ---" << endl;
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

    bool studentIdIsValid;
    do {
        studentIdIsValid = true;
        cout << "Nhap Ma so sinh vien: ";
        studentId = Menu::getStringNoSpaces();

        for (const auto& user : users) {
            if (Student* s = dynamic_cast<Student*>(user)) {
                if (s->getStudentId() == studentId) {
                    cout << "Loi: Ma so sinh vien nay da duoc su dung. Vui long nhap lai." << endl;
                    studentIdIsValid = false;
                    break; 
                }
            }
        }
    } while (!studentIdIsValid); 
    
    cout << "Nhap Lop: ";
    studentClass = Menu::getStringNoSpaces();

    cout << "Nhap Khoa hoc (VD: 65): ";
    courseYear = Menu::getIntegerInput(); 

    int newId = dataManager.getNextUserId();
    Student* newStudent = new Student(newId, name, email, password, studentId, studentClass, courseYear, 0.0);
    users.push_back(newStudent);

    cout << "Dang ky thanh cong! Vui long dang nhap de su dung he thong." << endl;
}