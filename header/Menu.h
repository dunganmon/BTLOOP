#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include "Book.h"

using namespace std;

class Menu {
public:
    static void clearScreen();
    static void pause();
    static int getIntegerInput();

    static string getStringNoSpaces();
    static bool isValidEmailFormat(const string& email);

    static string getStringPassword();
    static int getPublicationYearInput();
    static int getBookTypeInput();

    static void displayMainMenu();
    static void displayAdminMenu(const string& adminName);
    static void displayLibrarianMenu(const string& librarianName);
    static void displayStudentMenu(const string& studentName);
    static void viewAllBooks(const vector<Book*>& books);
    static void searchAndDisplayBooks(const vector<Book*>& books);
    static void displayBooksByType(const vector<Book*>& books);
    static void displaySortedBooks(const vector<Book*>& books);
private:
    static void printBookTableHeader();
};

#endif 