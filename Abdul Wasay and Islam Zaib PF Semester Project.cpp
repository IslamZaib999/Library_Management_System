#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// Structure to hold book data
struct Book {
    int id;
    char title[50];
    char author[50];
    int quantity;
};

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();

int main() {
    int choice;

    do {
        cout << "\n\n=== LIBRARY MENU ===\n";
        cout << "1. Add New Book (Old data deleted)\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book by ID\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addBook(); break;
        case 2: displayBooks(); break;
        case 3: searchBook(); break;
        case 4: issueBook(); break;
        case 5: returnBook(); break;
        case 6: cout << "\nThank you!\n"; break;
        default: cout << "\nInvalid choice!\n"; break;
        }
    } while (choice != 6);

    return 0;
}

void addBook() {
    Book b;

    // ios::trunc deletes old data and saves only new book
    ofstream file("library.dat", ios::binary | ios::trunc);

    cout << "\nEnter Book ID: ";
    cin >> b.id;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Title: ";
    cin.getline(b.title, 50);

    cout << "Enter Author: ";
    cin.getline(b.author, 50);

    cout << "Enter Quantity: ";
    cin >> b.quantity;

    file.write(reinterpret_cast<char*>(&b), sizeof(b));
    file.close();

    cout << "\nNew book saved successfully (old data removed)!\n";
}

void displayBooks() {
    Book b;
    ifstream file("library.dat", ios::binary);

    if (!file) {
        cout << "\nNo books found.\n";
        return;
    }

    cout << "\nLibrary Books:\n";
    cout << "ID | Title | Author | Qty\n";
    cout << "-------------------------\n";

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        cout << b.id << " | " << b.title << " | "
             << b.author << " | " << b.quantity << endl;
    }

    file.close();
}

void searchBook() {
    Book b;
    int id;
    bool found = false;

    ifstream file("library.dat", ios::binary);
    if (!file) {
        cout << "\nLibrary is empty.\n";
        return;
    }

    cout << "\nEnter Book ID to search: ";
    cin >> id;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.id == id) {
            cout << "\nBook Found!\n";
            cout << "ID: " << b.id
                 << "\nTitle: " << b.title
                 << "\nAuthor: " << b.author
                 << "\nQuantity: " << b.quantity << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nBook not found!\n";

    file.close();
}

void issueBook() {
    Book b;
    int id;
    bool found = false;

    fstream file("library.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nLibrary is empty.\n";
        return;
    }

    cout << "\nEnter Book ID to issue: ";
    cin >> id;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.id == id) {
            if (b.quantity > 0) {
                b.quantity--;
                file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
                file.write(reinterpret_cast<char*>(&b), sizeof(b));
                cout << "\nBook issued successfully!\n";
            } else {
                cout << "\nBook is out of stock.\n";
            }
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nBook not found.\n";

    file.close();
}

void returnBook() {
    Book b;
    int id, days;
    bool found = false;

    fstream file("library.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nLibrary is empty.\n";
        return;
    }

    cout << "\nEnter Book ID to return: ";
    cin >> id;
    cout << "Enter late days (0 if none): ";
    cin >> days;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.id == id) {
            b.quantity++;
            file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            file.write(reinterpret_cast<char*>(&b), sizeof(b));

            if (days > 0)
                cout << "\nLate fine: Rs " << days * 10 << endl;

            cout << "\nBook returned successfully!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nInvalid Book ID!\n";

    file.close();
}
