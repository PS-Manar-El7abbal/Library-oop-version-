#include <bits/stdc++.h>
using namespace std;

void run();
void userOradmin();

class Book;
class User;
class Book {
private:
    int id;
    string name;
    int quantity;

public:
    friend class LoanBook;
    friend class Admin;
    Book() = default;
    Book(int id, string name, int quantity) : id(id), name(name), quantity(quantity) {}
    static vector<Book> books;
    vector<User> borrowers;
};
class User {
private:
    int id;
    string name;
    string password;

public:
    friend class LoanBook;
    friend class Admin;
    User() = default;
    User(int id, string name, string password) : id(id), name(name), password(password) {}
    static vector<User> users;
    vector<Book> borrowed_books;
};

class Admin {

public:
    friend class LoanBook;  
    static string name ;
    static string password ;
    Admin() = default;
    void add_book() {
        cout << "Enter the name: ";
        string n;
        cin.ignore();
        getline(cin, n);

        cout << "Enter the id: ";
        int id_book;
        cin >> id_book;

        for (auto &book : Book::books) {
            if (book.id == id_book) {
                cout << "Change this id. This ID already exists.\n";
                return;
            }
        }

        cout << "Enter the quantity: ";
        int q;
        cin >> q;
        
        Book::books.push_back(Book(id_book, n, q));
        cout << "Book added successfully.\n\n";
    }

    void who_borrow() {
        cout << "Enter the book Name: ";
        string book_name;
        cin.ignore();
        getline(cin, book_name);

        for (const auto &book : Book::books) {
            if (book.name == book_name) {
                cout << "Borrowers for " << book_name << ":\n";
                if (book.borrowers.empty()) {
                    cout << "No One borrow this book.\n\n";
                    return;
                }
                for (const auto &user : book.borrowers) {
                    cout << "- " << user.name << '\n';
                }
                return;
            }
        }
        cout << "No such book found.\n\n";
    }

    void print_users() {
        for (const auto &user : User::users) {
            cout << "Name : " << user.name << " - ID : " << user.id << '\n';
            if (user.borrowed_books.empty()) continue;
            cout << "Books which this user borrowed:\n";
            for (const auto &book : user.borrowed_books) {
                cout << "Book name : " << book.name << " Book ID : " << book.id << '\n';
            }
        }
    }

    void add_user() {
        cout << "Enter user name and ID: ";
        string name;
        int id;
        cin >> name >> id;
        for (const auto &u : User::users) {
            if (u.name == name) {
                cout << "User already exists.\n\n";
                return;
            }
        }
        User::users.push_back(User(id, name, "password"));
        cout << "User added successfully.\n\n";
    }

    void print_by_id() {
        sort(Book::books.begin(), Book::books.end(), [](const Book &a, const Book &b) {
            return a.id < b.id;
        });
        for (const auto &book : Book::books) {
            cout << "ID : " << book.id << " - Name : " << book.name << " Quantity :" << book.quantity << endl;
        }
    }

    void search_book_by_prefix() {
        cout << "Enter the prefix to search: ";
        string prefix;
        cin >> prefix;

        bool found = false;
        for (const auto &book : Book::books) {
            if (book.name.substr(0, prefix.size()) == prefix) {
                cout << book.name << endl;
                found = true;
            }
        }
        if (!found) cout << "No books found with the given prefix.\n";
    }
};


vector<User> User::users;
vector<Book> Book::books;
string Admin::name = "ManarElhabbal" ;
string Admin::password = "manarmanora" ;

class LoanBook {
public:
    void borrow_book() {
        cout << "Enter book name and user id: ";
        string book_name;
        int user_id;
        cin.ignore();
        getline(cin, book_name);
        cin >> user_id;

        Book *book = nullptr;
        for (auto &b : Book::books) {
            if (b.name == book_name && b.quantity > 0) {
                book = &b;
                break;
            }
        }
        if (!book) {
            cout << "Book not found or unavailable.\n";
            return;
        }

        for (auto &user : User::users) {
            if (user.id == user_id) {
                for (const auto &b : user.borrowed_books) {
                    if (b.id == book->id) {
                        cout << "User already borrowed this book.\n";
                        return;
                    }
                }
                user.borrowed_books.push_back(*book);
                book->borrowers.push_back(user);
                book->quantity--;
                cout << "Successfully borrowed " << book->name << ".\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void return_book() {
        cout << "Enter book name and user id: ";
        string book_name;
        int user_id;
        cin.ignore();
        getline(cin, book_name);
        cin >> user_id;

        for (auto &user : User::users) {
            if (user.id == user_id) {
                auto it = find_if(user.borrowed_books.begin(), user.borrowed_books.end(),
                                [&book_name](const Book &book) { return book.name == book_name; });

                if (it != user.borrowed_books.end()) {
                    user.borrowed_books.erase(it);
                    for (auto &book : Book::books) {
                        if (book.name == book_name) {
                            book.quantity++;
                            book.borrowers.erase(remove_if(book.borrowers.begin(), book.borrowers.end(),
                                                        [user_id](const User &u) { return u.id == user_id; }),
                                                book.borrowers.end());
                            cout << "Successfully returned " << book.name << ".\n";
                            return;
                        }
                    }
                }
            }
        }
        cout << "No such borrowed book found.\n";
    }
};

void show_Menu() {
    cout << "Library Menu:\n";
    cout << "[1] add_book.\n";
    cout << "[2] search book by prefix.\n";
    cout << "[3] who borrowed the book.\n";
    cout << "[4] print library by id and name.\n";
    cout << "[5] Add user.\n";
    cout << "[6] user borrow book.\n";
    cout << "[7] user return book.\n";
    cout << "[8] print users.\n";
    cout << "[9] exit.\n";
}

void run() {
    show_Menu();
    int choose = 0;
    cin >> choose;
    LoanBook L;
    Admin ad1;
    switch (choose) {
        case 1: ad1.add_book(); break;
        case 2: ad1.search_book_by_prefix(); break;
        case 3: ad1.who_borrow(); break;
        case 4: ad1.print_by_id(); break;
        case 5: ad1.add_user(); break;
        case 6: L.borrow_book(); break;
        case 7: L.return_book(); break;
        case 8: ad1.print_users(); break;
        case 9: exit(0);
    }
}

int main() {
    cout << "Welcome to our library.\n";
    string name,password;
    cout << "Enter your name: ";cin >> name;
    cout << "Enter your password: ";cin >> password;
    if (name != Admin::name || password != Admin::password) {
        cout << "Wrong username or password.\n";
        return 0;
    }
    
    while (true) 
        run();
    return 0;
}
