#ifndef AUTHENTICATIONSERVICE_H
#define AUTHENTICATIONSERVICE_H

#include "Person.h"
#include "Student.h"
#include "DataManager.h"
#include <vector>
#include <string>

class AuthenticationService {
public:
    // Trả về con trỏ đến người dùng nếu đăng nhập thành công, ngược lại trả về nullptr
    Person* login(const string& email, const string& password, const vector<Person*>& users);

    // Đăng ký tài khoản sinh viên mới
    void registerStudent(vector<Person*>& users, DataManager& dataManager);
};

#endif 