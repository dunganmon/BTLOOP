#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
protected:
    int id;
    string name;
    string email;
    string password;
    int role; // 1: Admin, 2: Librarian, 3: Student

public:
    Person(int id = 0, string name = "", string email = "", string password = "", int role = 0)
        : id(id), name(name), email(email), password(password), role(role) {}

    virtual ~Person() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    int getRole() const { return role; }

    void setId(int id) { this->id = id; }
    void setName(const string& name) { this->name = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPassword(const string& password) { this->password = password; }
    void setRole(int role) { this->role = role; }

    virtual void displayInfo() const = 0;

    virtual string toCSV() const {
        return to_string(role) + "," + to_string(id) + "," + name + "," + email + "," + password;
    }
};

#endif 