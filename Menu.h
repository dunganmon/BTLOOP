#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include "Book.h"

using namespace std;

// Lớp này chứa các hàm tiện ích tĩnh để hiển thị menu và xử lý input.
// Việc này giúp tập trung code liên quan đến UI vào một nơi.
class Menu {
public:
    static void clearScreen();
    static void pause();
    static int getIntegerInput();

    static void displayMainMenu();
    static void displayAdminMenu(const string& adminName);
    static void displayLibrarianMenu(const string& librarianName);
    static void displayStudentMenu(const string& studentName);

    static void viewAllBooks(const vector<Book>& books);
    static void searchAndDisplayBooks(const vector<Book>& books);
};

#endif 