#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include <iomanip> 

class Student : public Person {
private:
    string studentId;
    string studentClass;
    int courseYear;
    double fineAmount;

public:
    Student(int id = 0, string name = "", string email = "", string password = "",
            string studentId = "", string studentClass = "", int courseYear = 0, double fineAmount = 0.0)
        : Person(id, name, email, password, 3), studentId(studentId), studentClass(studentClass),
          courseYear(courseYear), fineAmount(fineAmount) {}

    string getStudentId() const { return studentId; }
    string getStudentClass() const { return studentClass; }
    int getCourseYear() const { return courseYear; }
    double getFineAmount() const { return fineAmount; }

    void setStudentId(const string& sid) { this->studentId = sid; }
    void setStudentClass(const string& sClass) { this->studentClass = sClass; }
    void setCourseYear(int year) { this->courseYear = year; }
    void setFineAmount(double amount) { this->fineAmount = amount; }

    void addFine(double amount) {
        if (amount > 0) { this->fineAmount += amount; }
    }

    void payFine(double amount) {
        if (amount > 0) {
            this->fineAmount -= amount;
            if (this->fineAmount < 0) { this->fineAmount = 0; }
        }
    }

    void displayInfo() const override {
        cout << "| " << left << setw(8) << id
             << "| " << left << setw(20) << name
             << "| " << left << setw(25) << email
             << "| " << left << setw(15) << "Sinh vien"
             << "| " << left << setw(25) << studentId << " |" << endl;
    }
    
    void displayDetailedInfo() const {
        const int labelWidth = 12; 
        cout << "\n.---------------------------------------|" << endl;
        cout << "|       THONG TIN CHI TIET SINH VIEN    |" << endl;
        cout << ".---------------------------------------|" << endl;
        cout << "| " << left << setw(labelWidth) << "ID" << ": " << id << endl;
        cout << "| " << left << setw(labelWidth) << "Ho ten" << ": " << name << endl;
        cout << "| " << left << setw(labelWidth) << "Email" << ": " << email << endl;
        cout << "|---------------------------------------|" << endl;     
        cout << "| " << left << setw(labelWidth) << "MSSV" << ": " << studentId << endl;
        cout << "| " << left << setw(labelWidth) << "Lop" << ": " << studentClass << endl;
        cout << "| " << left << setw(labelWidth) << "Khoa hoc" << ": " << courseYear << endl;        
        cout << "|---------------------------------------|" << endl; 
        cout << "| " << left << setw(labelWidth) << "Tien phat" << ": " 
             << fixed << setprecision(0) << fineAmount << " VND" << endl;     
        cout << "'---------------------------------------|" << endl;
    }


    string toCSV() const override {
        return Person::toCSV() + "," + studentId + "," + studentClass + "," + to_string(courseYear) + "," + to_string(fineAmount);
    }
};

#endif 