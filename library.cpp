#include <bits/stdc++.h>
using namespace std;

class Person {
protected:
    int id;
    string name;
    string password;

public:
    Person() = default;
    Person(int id, string name, string password)
        : id(id), name(name), password(password) {}

    string get_name() const { return name; }
    int get_id() const { return id; }
    bool check_password(const string& pw) const { return password == pw; }
};


class User; 

class Book {
private:
    int id;
    string name;
    int quantity;
    vector<User*> borrowers;

public:
    Book() = default;
    Book(int id, string name, int quantity)
        : id(id), name(name), quantity(quantity) {}

    int get_id() const { return id; }
    string get_name() const { return name; }
    int get_quantity() const { return quantity; }

    void decrease_quantity() { if (quantity > 0) quantity--; }
    void increase_quantity() { quantity++; }

    void add_borrower(User* user) { borrowers.push_back(user); }
    void remove_borrower(int user_id);

    const vector<User*>& get_borrowers() const { return borrowers; }

    static vector<Book> books;
};

vector<Book> Book::books;

class User : public Person {
private:
    vector<Book*> borrowed_books;

public:
    User() = default;
    User(int id, string name, string password)
        : Person(id, name, password) {}

    void borrow_book(Book* book) {
        borrowed_books.push_back(book);
    }

    void return_book(const string& book_name);

    const vector<Book*>& get_borrowed_books() const {
        return borrowed_books;
    }

    static vector<User> users;
};

vector<User> User::users;

void Book::remove_borrower(int user_id) {
    borrowers.erase(
        remove_if(borrowers.begin(), borrowers.end(),
                    [user_id](User* u) { return u->get_id() == user_id; }),
        borrowers.end());
}

void User::return_book(const string& book_name) {
    auto it = find_if(borrowed_books.begin(), borrowed_books.end(),
        [&book_name](Book* book) {
            return book->get_name() == book_name;
        });

    if (it != borrowed_books.end()) {
        (*it)->increase_quantity();
        (*it)->remove_borrower(this->id);
        borrowed_books.erase(it);
        cout << "Book returned successfully.\n";
    }
    else {
        cout << "Book not found in user's borrowed list.\n";
    }
}


class Admin : public Person {
public:
    Admin() : Person(0, "ManarElhabbal", "manarmanora") {}

    void add_book(const string& name, int id, int quantity) {
        for (auto& book : Book::books) {
            if (book.get_id() == id) {
                cout << "Book ID already exists.\n";
                return;
            }
        }
        Book::books.emplace_back(id, name, quantity);
        cout << "Book added successfully.\n";
    }

    void add_user(const string& name, int id) {
        for (auto& u : User::users) {
            if (u.get_name() == name) {
                cout << "User already exists.\n";
                return;
            }
        }
        User::users.emplace_back(id, name, "password");
        cout << "User added successfully.\n";
    }

    void who_borrowed(const string& book_name) {
        for (const auto& book : Book::books) {
            if (book.get_name() == book_name) {
                if (book.get_borrowers().empty()) {
                    cout << "No one borrowed this book.\n";
                }
                else {
                    cout << "Borrowers:\n";
                    for (const auto& user : book.get_borrowers()) {
                        cout << "- " << user->get_name() << "\n";
                    }
                }
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void print_users() {
        for (auto& user : User::users) {
            cout << "User: " << user.get_name() << " - ID: " << user.get_id() << "\n";
            auto books = user.get_borrowed_books();
            if (!books.empty()) {
                cout << "Borrowed Books:\n";
                for (auto book : books) {
                    cout << "  - " << book->get_name() << "\n";
                }
            }
        }
    }

    void print_books_by_id() {
        sort(Book::books.begin(), Book::books.end(),
            [](const Book& a, const Book& b) {
                return a.get_id() < b.get_id();
            });
        for (const auto& book : Book::books) {
            cout << "ID: " << book.get_id() << " - " << book.get_name()
                << " | Quantity: " << book.get_quantity() << "\n";
        }
    }

    void search_book_by_prefix(const string& prefix) {
        bool found = false;
        for (const auto& book : Book::books) {
            if (book.get_name().substr(0, prefix.size()) == prefix) {
                cout << "- " << book.get_name() << "\n";
                found = true;
            }
        }
        if (!found) cout << "No books found with this prefix.\n";
    }
};


class Loan_Service {
public:
    void borrow_book(const string& book_name, int user_id) {
        Book* book = nullptr;
        for (auto& b : Book::books) {
            if (b.get_name() == book_name && b.get_quantity() > 0) {
                book = &b;
                break;
            }
        }

        if (!book) {
            cout << "Book not found or unavailable.\n";
            return;
        }

        for (auto& user : User::users) {
            if (user.get_id() == user_id) {
                user.borrow_book(book);
                book->add_borrower(&user);
                book->decrease_quantity();
                cout << "Book borrowed successfully.\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void return_book(const string& book_name, int user_id) {
        for (auto& user : User::users) {
            if (user.get_id() == user_id) {
                user.return_book(book_name);
                return;
            }
        }
        cout << "User not found.\n";
    }
};


void show_menu() {
    cout << "\n Library Menu:\n";
    cout << "[1]  Add Book\n";
    cout << "[2]  Search Book by Prefix\n";
    cout << "[3]  Who Borrowed Book\n";
    cout << "[4]  Print All Books\n";
    cout << "[5]  Add User\n";
    cout << "[6]  Borrow Book\n";
    cout << "[7]  Return Book\n";
    cout << "[8]  Print Users\n";
    cout << "[9]  Exit\n";
    cout << "Choose: ";
}

void run(Admin& admin, Loan_Service& service) {
    show_menu();
    int choice;
    cin >> choice;
    cin.ignore();

    string name;
    int id, quantity;
    switch (choice) {
        case 1:
            cout << "Book Name: "; getline(cin, name);
            cout << "Book ID: "; cin >> id;
            cout << "Quantity: "; cin >> quantity;
            admin.add_book(name, id, quantity);
            break;
        case 2:
            cout << "Prefix: "; cin >> name;
            admin.search_book_by_prefix(name);
            break;
        case 3:
            cout << "Book Name: "; getline(cin, name);
            admin.who_borrowed(name);
            break;
        case 4:
            admin.print_books_by_id();
            break;
        case 5:
            cout << "User Name: "; cin >> name;
            cout << "User ID: "; cin >> id;
            admin.add_user(name, id);
            break;
        case 6:
            cout << "Book Name: "; getline(cin, name);
            cout << "User ID: "; cin >> id;
            service.borrow_book(name, id);
            break;
        case 7:
            cout << "Book Name: "; getline(cin, name);
            cout << "User ID: "; cin >> id;
            service.return_book(name, id);
            break;
        case 8:
            admin.print_users();
            break;
        case 9:
            exit(0);
        default:
            cout << "Invalid choice!\n";
    }
}


int main() {
    Admin admin;
    string name, pw;
    cout << "Welcome to the Library System!\n";
    cout << "Enter Admin Name: "; cin >> name;
    cout << "Enter Password: "; cin >> pw;

    if (name != admin.get_name() || !admin.check_password(pw)) {
        cout << "Invalid credentials.\n";
        return 0;
    }

    Loan_Service service;
    while (true) {
        run(admin, service);
    }

    return 0;
}
