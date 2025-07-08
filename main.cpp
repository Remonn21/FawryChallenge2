#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

class Book
{
protected:
    string isbn, title, author;
    int year;
    double price;

public:
    Book(string isbn, string title, string author, int year, double price)
        : isbn(isbn), title(title), author(author), year(year), price(price) {}

    virtual ~Book() {}
    virtual bool available(int qty) const = 0;
    virtual void deliver(const string &email, const string &addr, int qty) = 0;

    string getISBN() { return isbn; }
    int getYear() { return year; }
    double getPrice() { return price; }
};

class PaperBook : public Book
{
    int stock;

public:
    PaperBook(string isbn, string title, string author, int year, double price, int stock)
        : Book(isbn, title, author, year, price), stock(stock) {}

    bool available(int qty) const
    {
        return stock >= qty;
    }

    void deliver(const string &email, const string &addr, int qty)
    {
        if (stock < qty)
            throw runtime_error("Out of stock: " + title);
        stock = stock - qty;
        cout << "Quantum book store: Shipped '" << title << "' to " << addr << endl;
    }
};

class EBook : public Book
{
    string format;

public:
    EBook(string isbn, string title, string author, int year, double price, string format)
        : Book(isbn, title, author, year, price), format(format) {}

    bool available(int qty) const { return true; }

    void deliver(const string &email, const string &addr, int qty)
    {
        cout << "Quantum book store: Emailed '" << title << "' (" << format << ") to " << email << endl;
    }
};

class DemoBook : public Book
{
public:
    DemoBook(string isbn, string title, string author, int year)
        : Book(isbn, title, author, year, 0.0) {}

    bool available(int qty) const { return false; }

    void deliver(const string &email, const string &addr, int qty)
    {
        throw runtime_error("Demo book '" + title + "' not for sale");
    }
};

class Store
{
    vector<Book *> books;

public:
    ~Store()
    {
        for (int i = 0; i < books.size(); i++)
        {
            delete books[i];
        }
    }

    void add(Book *book)
    {
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i]->getISBN() == book->getISBN())
            {
                delete book;
                throw runtime_error("Book already exists");
            }
        }
        books.push_back(book);
    }

    vector<string> purgeOld(int currentYear, int ageLimit)
    {
        vector<string> removed;
        vector<Book *> newBooks;

        for (int i = 0; i < books.size(); i++)
        {
            if ((currentYear - books[i]->getYear()) > ageLimit)
            {
                removed.push_back(books[i]->getISBN());
                delete books[i];
            }
            else
            {
                newBooks.push_back(books[i]);
            }
        }

        books = newBooks;
        return removed;
    }

    double buy(const string &isbn, int qty, const string &email, const string &addr)
    {
        Book *found = NULL;
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i]->getISBN() == isbn)
            {
                found = books[i];
                break;
            }
        }

        if (found == NULL || !found->available(qty))
            throw runtime_error("Book unavailable for purchase");

        found->deliver(email, addr, qty);
        return found->getPrice() * qty;
    }
};

class QuantumBookstoreFullTest
{
public:
    static void runTests()
    {
        Store store;

        store.add(new PaperBook("111", "Effective C++", "Scott Meyers", 2018, 45.0, 3));
        store.add(new EBook("222", "Deep Learning", "Ian Goodfellow", 2016, 35.0, "epub"));
        store.add(new DemoBook("333", "Ancient Manuscript", "Unknown", 1800));

        // Test 1: buying a PaperBook
        try
        {
            cout << "Quantum book store: Buying PaperBook..." << endl;
            double total = store.buy("111", 2, "customer@gmail.com", "221B Baker Street");
            cout << "Quantum book store: Paid $" << total << endl;
        }
        catch (exception &e)
        {
            cout << "Quantum book store error: " << e.what() << endl;
        }

        // Test 2: buying an EBook
        try
        {
            cout << "Quantum book store: Buying EBook..." << endl;
            double total = store.buy("222", 1, "reader@mail.com", "");
            cout << "Quantum book store: Paid $" << total << endl;
        }
        catch (exception &e)
        {
            cout << "Quantum book store error: " << e.what() << endl;
        }

        // Test 3: trying to buy DemoBook (should fail)
        try
        {
            cout << "Quantum book store: Buying DemoBook..." << endl;
            store.buy("333", 1, "test@test.com", "Some Address");
        }
        catch (exception &e)
        {
            cout << "Quantum book store error: " << e.what() << endl;
        }

        // Test 4: Remove old books
        vector<string> removedBooks = store.purgeOld(2024, 100);
        if (removedBooks.size() > 0)
        {
            cout << "Quantum book store: Removed outdated books:\n";
            for (int i = 1; i <= removedBooks.size(); i++)
                cout << i << "-ISBN: " << removedBooks[i - 1] << endl;
        }
        else
        {
            cout << "Quantum book store: No outdated books found." << endl;
        }
    }
};

int main()
{
    QuantumBookstoreFullTest::runTests();
    return 0;
}
