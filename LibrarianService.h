#ifndef LIBRARIANSERVICE_H
#define LIBRARIANSERVICE_H

#include "Librarian.h"
#include "Book.h"
#include "DataManager.h"
#include <vector>

class LibrarianService {
public:
    void run(Librarian* librarian, vector<Book*>& books, DataManager& dataManager);

private:
    void addBook(vector<Book*>& books, DataManager& dataManager);
    void editBook(vector<Book*>& books);
    void searchBook(const vector<Book*>& books);
};

#endif