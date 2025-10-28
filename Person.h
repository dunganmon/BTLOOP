#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Lớp cơ sở trừu tượng cho tất cả các loại người dùng trong hệ thống.
// Sử dụng pure virtual function để đảm bảo các lớp con phải định nghĩa lại hàm displayInfo.
class Person {
protected:
    int id;
    string name;
    string email;
    string password;
    int role; // 1: Admin, 2: Librarian, 3: Student

public:
    // Constructor
    Person(int id = 0, string name = "", string email = "", string password = "", int role = 0)
        : id(id), name(name), email(email), password(password), role(role) {}

    // Destructor ảo để đảm bảo việc giải phóng bộ nhớ đúng cách cho các đối tượng lớp con
    virtual ~Person() {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    int getRole() const { return role; }

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const string& name) { this->name = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPassword(const string& password) { this->password = password; }
    void setRole(int role) { this->role = role; }

    // Hàm ảo thuần túy (pure virtual function), buộc các lớp con phải cài đặt
    virtual void displayInfo() const = 0;

    // Hàm ảo để phục vụ cho việc lưu file, trả về chuỗi CSV
    virtual string toCSV() const {
        return to_string(role) + "," + to_string(id) + "," + name + "," + email + "," + password;
    }
};

#endif 