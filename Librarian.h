#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "Person.h"
#include <iomanip> 

class Librarian : public Person {
private:
    string employeeId;

public:
    Librarian(int id = 0, string name = "", string email = "", string password = "", string employeeId = "")
        : Person(id, name, email, password, 2), employeeId(employeeId) {}

    string getEmployeeId() const { return employeeId; }
    void setEmployeeId(const string& empId) { this->employeeId = empId; }

    void displayInfo() const override {
        cout << "| " << left << setw(8) << id
             << "| " << left << setw(20) << name
             << "| " << left << setw(25) << email
             << "| " << left << setw(15) << "Thu thu"
             << "| " << left << setw(25) << employeeId << " |" << endl;
    }

    string toCSV() const override {
        return Person::toCSV() + "," + employeeId;
    }
};

#endif 