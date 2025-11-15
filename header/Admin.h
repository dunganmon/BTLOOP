#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <iomanip> 

class Admin : public Person {
public:
    Admin(int id = 0, string name = "", string email = "", string password = "")
        : Person(id, name, email, password, 1) {}

    void displayInfo() const override {
        cout << "| " << left << setw(8) << id
             << "| " << left << setw(20) << name
             << "| " << left << setw(25) << email
             << "| " << left << setw(15) << "Admin"
             << "| " << left << setw(25) << "N/A" << " |" << endl;
    }

    string toCSV() const override {
        return Person::toCSV();
    }
};

#endif 