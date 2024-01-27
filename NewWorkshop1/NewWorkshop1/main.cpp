#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <mysql.h> //libmysql

#pragma warning(disable : 4996)
using namespace std;

void Staff();
void Login();
void Register();
void ManageBook();
void AddBook();
void UpdateBook();
void SearchBook();
void DeleteBook();
void ViewBook();
void ViewReport();
void OverallBookSales();
void EachBookSales();
void ViewFeedback();
void Customer();
void CustLogin();
void CustRegister();
void Bookstore();
void CustViewBook();
void CustSearchBook();
void PurchaseBook();
void ViewPurchase();  
void Feedback();

int qstate, previousQuantity = 0;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
string UserID;

class db_response
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        if (conn)
            cout << "Database Connected" << endl;
        else
            cout << "Failed To Connect!" << endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "bookstore", 3306, NULL, 0);
        if (conn)
            cout << "Database Connected To MySql" << endl;
        else
            cout << "Failed To Connect!" << endl;

        cout << endl;
    }
};

bool containsOnlyLetters(std::string const& str) 
{
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ -") ==
        std::string::npos;
    // contains a-z and space
}

int main() {
    system("cls");
    system("title Bookstore");
    db_response::ConnectionFunction();

    int mm; //main menu

    cout << setw(50) << "BOOKSTORE INVENTORY SYSTEM" << endl << endl;

    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. Staff \n"
        << "2. Customer \n"
        << "3. Feedback \n"
        << "4. Exit \n" << endl
        << "Please enter the number: ";
    cin >> mm;

    switch (mm)
    {
    case 1:
        system("cls");
        Staff();
        break;
    case 2:
        system("cls");
        Customer();
        break;
    case 3:
        system("cls");
        Feedback();
        break;
    case 4:
        exit(1);
        break;
    default:
        //system("cls");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        exit(1);

    }

    return 0;
}

void Staff()
{
    int sm; //staff menu

    cout << setw(50) << "Welcome to Staff Menu !\n" << endl;
    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. Login \n" 
        << "2. Register \n" 
        << "3. Back to Main Menu \n"
        << endl << "Please enter the number: ";
    cin >> sm;
    cout << endl;

    switch (sm)
    {
    case 1:
        system("cls");
        Login();
        break;
    case 2:
        system("cls");
        Register();
        break;
    case 3:
        system("cls");
        main();
        break;
    default:
        //system("cls");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        exit(1);

    }
}

void Login()
{
    system("cls");
    string username;
    string password;

    cout << setw(50) << "Login to Your Staff Account\n" << endl;
    cout << "Please enter your login details:" << "\n--------------------------------\n" << endl;

    cout << "Username : ";
    cin >> username;
    cout << "Password : ";
    char ps;
    while ((ps = _getch()) != 13)
    {
        if (ps != 13 && ps != 8) //13 is the ASCII number for ENTER key and 8 is for BACKSPACE key
        {
            password += ps;
            cout << "*";
        }
 
    }
    cout << endl;

    string checkUser_query = "SELECT * FROM staff where Username = '" + username + "' and Password = '" + password + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                username = row[0];
            cout << "Login Successful.\n" << endl;
            system("pause");
            ManageBook();
        }
        else
        {
            char c;
            cout << "\nInvalid account ID or password. Want to try again? (y/n): ";
            cin >> c;
            if (c == 'y' || c == 'Y')
                Login();
            else
                main();
        }
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void Register()
{
    system("cls");
    string name, username, email, password;
    char choose;
    bool checkname;

    cout << setw(50) << "Register Your Staff Account\n" << endl;
    cout << "Please fill in your details:" << "\n--------------------------------\n" << endl;

    cout << "Full Name: ";
    getline(cin.ignore(), name);

    if (containsOnlyLetters(name))
    {
        checkname = true;
    }
    else
    {
        checkname = false;
    }
    if (checkname == false)
    {
        cout << "\n[Wrong input, your name contains numbers or special characters. Insert again!]\n" << endl;
        system("pause");
        Register();
    }

    cout << "Username: ";
    getline(cin, username);

    cout << "Email: ";
    getline(cin, email);

    cout << "Password: ";
    cin >> password;

    cout << endl;

    string checkUser_query = "SELECT * FROM staff WHERE username = '" + username + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {

            cout << "Username already exist. Do you want to try again?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                Register();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Staff();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            string insertCustomer_query = "INSERT INTO staff (Name, Username, Email, Password) VALUES ('" + name + "', '" + username + "', '" + email + "', '" + password + "')";
            const char* q = insertCustomer_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate)
            {
                cout << "You have been successfully registered. Do you want to create another account?(y/n): ";
                cin >> choose;
                if (choose == 'Y' || choose == 'y')
                {
                    system("cls");
                    Register();
                }
                else if (choose == 'N' || choose == 'n')
                {
                    system("cls");
                    Staff();
                }
                else
                {
                    exit(1);
                }
            }
            else
            {
                cout << "Unable to register new account. Query execution problem!" << mysql_errno(conn) << endl;
            }
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

void ManageBook()
{
    system("cls");
    int mb; // managebook menu

    cout << setw(50) << "Welcome to Manage Book Menu !\n" << endl;
    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. Add Book \n"
        << "2. Update Book \n"
        << "3. Search Book \n" 
        << "4. Delete Book \n" 
        << "5. View Book \n"
        << "6. View Report \n"
        << "7. View Feedback \n"
        << "8. Back to Main Menu \n" 
        << endl << "Please enter the number: ";
    cin >> mb;
    cout << endl;

    switch (mb)
    {
    case 1:
        AddBook();
        break;
    case 2:
        UpdateBook();
        break;
    case 3:
        SearchBook();
        break;
    case 4:
        DeleteBook();
        break;
    case 5:
        ViewBook();
        break;
    case 6:
        ViewReport();
        break;
    case 7:
        ViewFeedback();
        break;
    case 8:
        main();
        break;
    default:
        system("cls");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        Staff();
    }
}

void AddBook()
{
    system("cls");
    string name, author, genre, isbn, price, quantity;
    char choose;

    cout << setw(50) << "Welcome to Add Book Menu !\n" << endl;
    cout << "Please fill in the information below:" << "\n--------------------------------------\n" << endl;

    cout << "Book Name: ";
    getline(cin.ignore(), name);

    cout << "Book Author: ";
    getline(cin, author);

    cout << "Book Genre: ";
    getline(cin, genre);

    cout << "Book ISBN: ";
    getline(cin, isbn);

    cout << "Book Price: RM ";
    cin >> price;

    cout << "Book Quantity: ";
    cin >> quantity;

    string insert_query = "INSERT INTO book (Name, Author, Genre, Isbn, Price, Quantity) VALUES ('" + name + "', '" + author + "', '" + genre + "', '" + isbn + "', '" + price + "', '" + quantity + "')";
    const char* q = insert_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        cout << "\nSuccessfully added a new book, the details are as follows:" << "\n-----------------------------------------------------\n" << endl;

        cout << "Book Name: " << name << endl
            << "Book Author: " << author << endl
            << "Book Genre: " << genre << endl
            << "Book ISBN: " << isbn << endl
            << "Book Price: RM " << price << endl
            << "Book Quantity: " << quantity << endl;

        cout << "\nDo you want to add another book?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            AddBook();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            ManageBook();
        }
        else
        {
            exit(1);
        }

    }
    else
    {
        cout << "Insert Failed. Query execution problem!" << mysql_errno(conn) << endl;
    }

}

void UpdateBook()
{
    system("cls");
    int ub; //updatebook menu
    string BookID, name, author, genre, isbn, price, quantity;
    char choose;

    cout << setw(50) << "Welcome to Update Book Menu !" << endl;

    qstate = mysql_query(conn, "SELECT * FROM book");

    if (!qstate)
    {
        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        cout << endl << "Please enter the book ID to update:" << "\n-------------------------------------\n" << endl;

        cout << "Choose ID: ";
        cin >> BookID;

        cout << "\nWhich category do you want to update? Select your choice:" << endl;
        cout << "1. Book Name \n" 
            << "2. Book Author \n"
            << "3. Book Genre \n"
            << "4. Book ISBN \n"
            << "5. Book Price \n" 
            << "6. Book Quantity \n" << endl 
            << "Please enter the number: ";
        cin >> ub;

        if (ub == 1)
        {
            cout << "Book Name: ";
            getline(cin.ignore(), name);
            string update_query = "UPDATE book SET Name = '" + name + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (ub == 2)
        {
            cout << "Book Author: ";
            getline(cin.ignore(), author);
            string update_query = "UPDATE book SET Author = '" + author + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (ub == 3)
        {
            cout << "Book Genre: ";
            getline(cin.ignore(), genre);
            string update_query = "UPDATE book SET Genre = '" + genre + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (ub == 4)
        {
            cout << "Book ISBN: ";
            getline(cin.ignore(), isbn);
            string update_query = "UPDATE book SET Isbn = '" + isbn + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (ub == 5)
        {
            cout << "Book Price: RM ";
            getline(cin.ignore(), price);
            string update_query = "UPDATE book SET Price = '" + price + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (ub == 6)
        {
            cout << "Book Quantity: ";
            getline(cin.ignore(), quantity);
            string update_query = "UPDATE book SET Quantity = '" + quantity + "' WHERE BookID = '" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else
        {
            cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
            UpdateBook();
        }

        cout << "\nBook successfully updated. Do you want to update another book?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            UpdateBook();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            ManageBook();
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        cout << "Update failed. Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

void SearchBook()
{
    system("cls");
    char choose;
    string BookID;

    cout << setw(50) << "Welcome to Search Book Menu !\n" << endl;
    cout << "Please fill in the information below:" << "\n--------------------------------------\n" << endl;

    cout << "Search book by ID: ";
    getline(cin.ignore(), BookID);

    cout << endl;

    string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE BookID like '%" + BookID + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        cout << "--- Book founded! ---" << endl;

        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4] , row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        cout << "\nDo you want to search another book?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            SearchBook();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            ManageBook();
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
    }

}

void DeleteBook()
{
    system("cls");
    string BookID;
    char choose;
    bool found = false;

    cout << setw(50) << "Welcome to Delete Book Menu !\n" << endl;

    qstate = mysql_query(conn, "SELECT * FROM book");

    if (!qstate)
    {
        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

    cout << endl << "Please enter the book ID to delete:" << "\n-------------------------------------\n" << endl;

    cout << "Choose ID: ";
    cin >> BookID;

    string delete_query = "DELETE FROM purchase WHERE BookID = '" + BookID + "'";
    const char* q = delete_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        string delete_query = "DELETE FROM book WHERE BookID = '" + BookID + "'";
        const char* q = delete_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\nBook successfully deleted. Do you want to delete another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                DeleteBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                ManageBook();
            }
            else
            {
                exit(1);
            }
        }
    }
}

void ViewBook()
{
    system("cls");
    char choose;

    cout << setw(50) << "Welcome to View Book Menu !\n" << endl;

    qstate = mysql_query(conn, "SELECT * FROM book");

    if (!qstate)
    {
        cout << "-----List of Books Available-----" << endl;

        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        cout << "\nDo you want to return to manage book menu?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            ManageBook();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            main();
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        cout << "View book failed. Query execution problem!" << mysql_errno(conn) << endl;
    }


}

void ViewReport()
{
    system("cls");
    int rm; // report menu

    cout << setw(50) << "Welcome to Report Menu !\n" << endl;
    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. Overall Book Sales \n"
        << "2. Each Book Sales \n"
        << "3. Back to Manage Book Menu \n"
        << endl << "Please enter the number: ";
    cin >> rm;
    cout << endl;

    switch (rm)
    {
    case 1:
        OverallBookSales();
        break;
    case 2:
        EachBookSales();
        break;
    case 3:
        ManageBook();
        break;
    default:
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        exit(1);
    }
}

void OverallBookSales()
{
    system("cls");
    double totalOverallPrice = 0.0, totalOverallQuantity = 0.0;
    char choose;

    cout << setw(50) << "Bookstore Overall Sales Report !\n" << endl;
    cout << "The information is as below:" << "\n--------------------------------------\n" << endl;

    string totalOverallPrice_query = "SELECT TotalPrice FROM purchase";
    qstate = mysql_query(conn, totalOverallPrice_query.c_str());
    if (!qstate)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            totalOverallPrice += atof(row[0]);
        }
        cout << "Total Overall Sales : RM " << totalOverallPrice << endl;
    }
    else
        cout << "Total Sales Failed to Calculate" << mysql_errno(conn) << endl;

    string totalOverallQuantity_query = "SELECT Quantity FROM purchase";
    qstate = mysql_query(conn, totalOverallQuantity_query.c_str());
    if (!qstate)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            totalOverallQuantity += atof(row[0]);
        }
        cout << "Total Book Sold     : " << totalOverallQuantity << " pcs" << endl;
    }
    else
        cout << "Total Book Sold Failed to Calculate" << mysql_errno(conn) << endl;

    cout << "\nDo you want to view each book sales report?(y/n): ";
    cin >> choose;
    if (choose == 'Y' || choose == 'y')
    {
        system("cls");
        EachBookSales();
    }
    else if (choose == 'N' || choose == 'n')
    {
        system("cls");
        ViewReport();
    }
    else
    {
        exit(1);
    }

}

void EachBookSales()
{
    system("cls");
    string BookID;
    char choose;
    double totPrice = 0.0, totQuantity = 0.0;

    cout << setw(50) << "Welcome to Each Book Sales Report Menu !\n" << endl;
    cout << "Please fill in the information below:" << "\n--------------------------------------\n" << endl;

    cout << "Search by Book ID: ";
    cin >> BookID;
    cout << endl;

    cout << "=====================================================" << endl;
    cout << setw(30) << "Book Sales" << endl;
    cout << "=====================================================\n" << endl;

    cout << "Book Name  : ";
    string bookname_query = "SELECT Name FROM book WHERE BookID = '" + BookID + "'";
    const char* c = bookname_query.c_str();
    qstate = mysql_query(conn, c);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        while ((row = mysql_fetch_row(res)))
        {

            printf(row[0]);
        }
    }
    else
        cout << "Book Name Failed to Find" << mysql_errno(conn) << endl;

    cout << "\nBook Price : RM ";
    string bookprice_query = "SELECT Price FROM book WHERE BookID = '" + BookID + "'";
    const char* cc = bookprice_query.c_str();
    qstate = mysql_query(conn, cc);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        while ((row = mysql_fetch_row(res)))
        {

            printf(row[0]);
        }
    }
    else
        cout << "Book Price Failed to Find" << mysql_errno(conn) << endl;

    string totQuantity_query = "SELECT Quantity FROM purchase WHERE BookID = '" + BookID + "'";
    qstate = mysql_query(conn, totQuantity_query.c_str());
    if (!qstate)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            totQuantity += atof(row[0]);
        }
        cout << "\nBook Sold  : " << totQuantity << " pcs" << endl;
    }
    else
        cout << "Total Book Sold Failed to Calculate" << mysql_errno(conn) << endl;


    string totPrice_query = "SELECT TotalPrice FROM purchase WHERE BookID = '" + BookID + "'";
    qstate = mysql_query(conn, totPrice_query.c_str());
    if (!qstate)
    {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            totPrice += atof(row[0]);
        }
        cout << "\n=====================================================" << endl;
        cout << setw(30) << "Total Sales  : RM " << totPrice << endl;
        cout << "=====================================================" << endl;
    }
    else
        cout << "Total Sales Failed to Calculate" << mysql_errno(conn) << endl;

    cout << "\nDo you want to view another book sales report?(y/n): ";
    cin >> choose;
    if (choose == 'Y' || choose == 'y')
    {
        system("cls");
        EachBookSales();
    }
    else if (choose == 'N' || choose == 'n')
    {
        system("cls");
        ViewReport();
    }
    else
    {
        exit(1);
    }
}

void ViewFeedback()
{
    system("cls");
    char choose;

    cout << setw(50) << "Welcome to View Feedback Menu !\n" << endl;

    qstate = mysql_query(conn, "SELECT * FROM feedback");

    if (!qstate)
    {
        cout << "-----List of Feedback-----" << endl;

        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-15s | %-40s | %-25s | %-20s | %-60s |\n", "Feedback ID", "Full Name", "Email", "Subject", "Message");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-15s | %-40s | %-25s | %-20s | %-60s |\n", row[0], row[1], row[2], row[3], row[4]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        cout << "\nDo you want to return to manage book menu?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            ManageBook();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            main();
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        cout << "View feedback failed. Query execution problem!" << mysql_errno(conn) << endl;
    }
}

void Customer()
{
    int cm; //customer menu

    cout << setw(50) << "Welcome to Customer Menu !\n" << endl;
    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. Login \n" 
        << "2. Register \n" 
        << "3. Back to Main Menu \n" 
        << endl << "Please enter the number: ";
    cin >> cm;
    cout << endl;

    switch (cm)
    {
    case 1:
        system("cls");
        CustLogin();
        break;
    case 2:
        system("cls");
        CustRegister();
        break;
    case 3:
        system("cls");
        main();
        break;
    default:
        //system("cls");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        exit(1);
    }
}

void CustLogin()
{
    system("cls");
    string username;
    string password;

    cout << setw(50) << "Login to Your Customer Account\n" << endl;
    cout << "Please enter your login details:" << "\n--------------------------------\n" << endl;

    cout << "Username : ";
    cin >> username;
    cout << "Password : ";
    char ps;
    while ((ps = _getch()) != 13)
    {
        if (ps != 13 && ps != 8) //13 is the ASCII number for ENTER key and 8 is for BACKSPACE key
        {
            password += ps;
            cout << "*";
        }

    }
    cout << endl;

    string checkUser_query = "SELECT UserID FROM user WHERE Username = '" + username + "' and Password = '" + password + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                UserID = row[0];
            cout << "Login Successful.\n" << endl;
            system("pause");
            Bookstore();
        }
        else
        {
            char c;
            cout << "\nInvalid account ID or password. Want to try again? (y/n): ";
            cin >> c;
            if (c == 'y' || c == 'Y')
                CustLogin();
            else
                main();
        }
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void CustRegister()
{
    system("cls");
    string name, username, email, password;
    char choose;
    bool checkname;

    cout << setw(50) << "Register Your Member Account\n" << endl;
    cout << "Please fill in your details:" << "\n--------------------------------\n" << endl;

    cout << "Full Name : ";
    getline(cin.ignore(), name);

    if (containsOnlyLetters(name))
    {
        checkname = true;
    }
    else
    {
        checkname = false;
    }
    if (checkname == false)
    {
        cout << "\n[Wrong input, your name contains numbers or special characters. Insert again!]\n" << endl;
        system("pause");
        CustRegister();
    }

    cout << "Username: ";
    getline(cin, username);

    cout << "Email: ";
    getline(cin, email);

    cout << "Password: ";
    cin >> password;

    cout << endl;

    string checkUser_query = "SELECT * FROM user WHERE Username = '" + username + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {

            cout << "Username already exist. Do you want to try again?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustRegister();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
               Customer();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            string insertCustomer_query = "INSERT INTO user (Name, Username, Email, Password) VALUES ('" + name + "', '" + username + "', '" + email + "', '" + password + "')";
            const char* q = insertCustomer_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate)
            {
                cout << "You have been successfully registered. Do you want to create another account?(y/n): ";
                cin >> choose;
                if (choose == 'Y' || choose == 'y')
                {
                    system("cls");
                    CustRegister();
                }
                else if (choose == 'N' || choose == 'n')
                {
                    system("cls");
                    Customer();
                }
                else
                {
                    exit(1);
                }
            }
            else
            {
                cout << "Unable to register new account. Query execution problem!" << mysql_errno(conn) << endl;
            }
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

void Bookstore()
{
    system("cls");
    int bm; // bookstore menu

    cout << setw(50) << "Welcome to Bookstore Menu !\n" << endl;
    cout << "Please choose the option below:" << "\n--------------------------------\n" << endl;

    cout << "1. View Book \n"
        << "2. Search Book \n"
        << "3. Purchase Book \n"
        << "4. View Purchase \n"
        << "5. Back to Main Menu \n"
        << endl << "Please enter the number: ";
    cin >> bm;
    cout << endl;

    switch (bm)
    {
    case 1:
        CustViewBook();
        break;
    case 2:
        CustSearchBook();
        break;
    case 3:
        PurchaseBook();
        break;
    case 4:
        ViewPurchase();
        break;
    case 5:
        main();
        break;
    default:
        system("cls");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        Staff();
    }
}

void CustViewBook()
{
    system("cls");
    char choose;

    cout << setw(50) << "Welcome to Display Book Menu !\n" << endl;

    qstate = mysql_query(conn, "SELECT * FROM book");

    if (!qstate)
    {
        cout << "-----List of Books Available-----" << endl;

        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        cout << "\nDo you want to return to bookstore menu?(y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            Bookstore();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            main();
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        cout << "Display book failed. Query execution problem!" << mysql_errno(conn) << endl;
    }
}

void CustSearchBook()
{
    int sb; //search book
    string BookID, name, author, genre, isbn;
    char choose;

    system("cls");

    cout << setw(50) << "Welcome to Search Book Menu !\n" << endl;

    cout << "Please Select the options below" << endl;
    cout << "------------------------------\n" << endl;
    cout << "1. Search by Book ID \n"
        << "2. Search by Book Name\n"
        << "3. Search by Book Author\n"
        << "4. Search by Book Genre\n"
        << "5. Search by Book ISBN\n"
        << "6. Exit \n"
        << "Please enter the number: ";
    cin >> sb;
    cout << endl;

    if (sb == 1) // SEARCH BY ID
    {
        cout << "Please enter the book ID: ";
        getline(cin.ignore(), BookID);

        cout << endl;

        string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE BookID like '%" + BookID + "%'";
        const char* q = search_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "--- List of Books ---" << endl;

            res = mysql_store_result(conn);
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
            while ((row = mysql_fetch_row(res)))
            {
                printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            }
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            cout << "\nDo you want to search another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustSearchBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Bookstore();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
        }
    }
    else if(sb == 2) // SEARCH BY NAME
    {
        cout << "Please enter the book name: ";
        getline(cin.ignore(), name);

        cout << endl;

        string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE Name like '%" + name + "%'";
        const char* q = search_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "--- List of Books ---" << endl;

            res = mysql_store_result(conn);
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
            while ((row = mysql_fetch_row(res)))
            {
                printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            }
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            cout << "\nDo you want to search another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustSearchBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Bookstore();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
        }
    }
    else if (sb == 3) //SEARCH BY AUTHOR
    {
        cout << "Please enter the author: ";
        getline(cin.ignore(), author);

        cout << endl;

        string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE Author like '%" + author + "%'";
        const char* q = search_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "--- List of Books ---" << endl;

            res = mysql_store_result(conn);
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
            while ((row = mysql_fetch_row(res)))
            {
                printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            }
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            cout << "\nDo you want to search another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustSearchBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Bookstore();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
        }
    }
    else if(sb == 4) // SEARCH BY GENRE
    {
        cout << "Please enter the book genre: ";
        getline(cin.ignore(), genre);

        cout << endl;

        string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE Genre like '%" + genre + "%'";
        const char* q = search_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "--- List of Books ---" << endl;

            res = mysql_store_result(conn);
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
            while ((row = mysql_fetch_row(res)))
            {
                printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            }
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            cout << "\nDo you want to search another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustSearchBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Bookstore();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
        }
    }
    else if (sb == 5) // SEARCH BY ISBN
    {
        cout << "Please enter the book ISBN: ";
        getline(cin.ignore(), isbn);

        cout << endl;

        string search_query = "SELECT BookID, Name, Author, Genre, Isbn, Price, Quantity FROM book WHERE Isbn like '%" + isbn + "%'";
        const char* q = search_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "--- List of Books ---" << endl;

            res = mysql_store_result(conn);
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
            while ((row = mysql_fetch_row(res)))
            {
                printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            }
            printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            cout << "\nDo you want to search another book?(y/n): ";
            cin >> choose;
            if (choose == 'Y' || choose == 'y')
            {
                system("cls");
                CustSearchBook();
            }
            else if (choose == 'N' || choose == 'n')
            {
                system("cls");
                Bookstore();
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            cout << "Search Failed. Query execution problem!" << mysql_errno(conn) << endl;
        }
    }

    else
    { 
        //system("cls");
        //system("pause");
        cout << "[ Wrong input, Please re-enter your choice! ]\n" << endl;
        exit(1);
    }
}

void PurchaseBook()
{
    system("cls");
    string BookID, quantity, name, bquantity, condition;
    double totalprice, totalquantity;
    char choose;

    cout << setw(50) << "Welcome to Purchase Book Menu !\n" << endl;
  
    qstate = mysql_query(conn, "SELECT * FROM book");

    if (!qstate)
    {
        cout << "-----List of Books Available-----" << endl;

        res = mysql_store_result(conn);
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", "Book ID", "Name", "Author", "Genre", "ISBN", "Price (RM)", "Quantity");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-10s | %-40s | %-20s | %-20s | %-20s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    else
    {
        cout << "View book failed. Query execution problem!" << mysql_errno(conn) << endl;
    }

    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    cout << "\nToday's Date: "; 
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
    string datenow(buffer);
    cout << datenow << endl;

    cout << "Choose the book you want to buy!" << endl;
    cout << "Enter Book ID : ";
    cin >> BookID;
    
    cout << "Enter quantity of book : ";
    cin >> quantity;

    string searchQuantity = "SELECT Quantity from book WHERE BookID = '" + BookID + "'";
    const char* sq = searchQuantity.c_str();
    qstate = mysql_query(conn, sq);
    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
            {
                bquantity = row[0]; 
            }
        }
        if (quantity > bquantity)
        {
            condition = "more";
            cout << "\n[There are only " << bquantity << " books available]\n\n";
            system("pause");
            PurchaseBook();

        }
        else if (quantity <= bquantity)
        {
            condition = "less";

            string query_price = "SELECT Price FROM book WHERE BookID = '" + BookID + "'";
            qstate = mysql_query(conn, query_price.c_str());
            if (!qstate)
            {
                res = mysql_store_result(conn);
                while (row = mysql_fetch_row(res))
                {

                    totalprice = atof(row[0]) * stoi(quantity);

                    cout << "\n=====================================================" << endl;
                    cout << "---------------------- INVOICE ----------------------" << endl;
                    cout << "=====================================================\n" << endl;
                    cout << "Invoice Date : " << datenow << endl;

                    cout << "Full Name    : ";
                    string search_query = "SELECT * FROM user WHERE UserID like '%" + UserID + "%'";
                    const char* c = search_query.c_str();
                    qstate = mysql_query(conn, c);

                    if (!qstate)
                    {
                        res = mysql_store_result(conn);

                        while ((row = mysql_fetch_row(res)))
                        {

                            printf(row[1]);
                        }
                    }

                    cout << "\nBook Name    : ";
                    string searchbook_query = "SELECT * FROM book WHERE BookID = '" + BookID + "'";
                    const char* cc = searchbook_query.c_str();
                    qstate = mysql_query(conn, cc);

                    if (!qstate)
                    {
                        res = mysql_store_result(conn);

                        while ((row = mysql_fetch_row(res)))
                        {

                            printf(row[1]);
                        }
                    }
                    cout << "\nQuantity     : " << quantity << endl;
                    cout << "\n=====================================================" << endl;
                    cout << "Total Price  : RM " << totalprice << endl;
                    cout << "=====================================================" << endl;


                    string insert_query = "INSERT INTO `purchase`(`BookID`, `UserID`, `Quantity`, `TotalPrice`, `Time`) VALUES ('" + BookID + "','" + UserID + "','" + quantity + "', '" + to_string(totalprice) + "','" + datenow + "')";
                    const char* q = insert_query.c_str();
                    qstate = mysql_query(conn, q);

                    if (!qstate)
                    {
                        cout << endl << "Your Order Has been Approved.\n" << endl;
                    }
                    else
                    {
                        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                    }
                }
            }
            else
                cout << "Total Price Failed to Calculate" << mysql_errno(conn) << endl;

            string query_quantity = "SELECT Quantity FROM book WHERE BookID = '" + BookID + "'";
            qstate = mysql_query(conn, query_quantity.c_str());

            if (!qstate)
            {
                res = mysql_store_result(conn);
                while (row = mysql_fetch_row(res))
                {

                    totalquantity = atof(row[0]) - stoi(quantity);


                    string insert_query = "UPDATE book SET Quantity = '" + to_string(totalquantity) + "' WHERE BookID = '" + BookID + "'";
                    const char* q = insert_query.c_str();
                    qstate = mysql_query(conn, q);

                    if (qstate)
                    {
                        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                    }

                    do
                    {
                        cout << "Do you want to buy more book? (y/n): ";
                        cin >> choose;
                        if (choose == 'y' || choose == 'Y')
                        {
                            PurchaseBook();
                        }
                        else if (choose == 'n' || choose == 'N')
                        {
                            Bookstore();
                        }
                    } while (choose == 'y' || choose == 'Y' || choose == 'n' || choose == 'N');


                    _getch();
                    Bookstore();
                }
            }
            else
                cout << "Total Quantity Failed to Calculate" << mysql_errno(conn) << endl;
        }
    }
  
}

void ViewPurchase()
{
    system("cls");

    cout << setw(50) << "Welcome to View Purchase Menu !\n" << endl;

    string query_price = "SELECT * FROM purchase WHERE UserID = '" + UserID + "'";
    qstate = mysql_query(conn, query_price.c_str());
    if (!qstate)
    {
        cout << "-----List of Book Purchase-----" << endl;

        res = mysql_store_result(conn);
        printf("-------------------------------------------------------------------------------------------------------------\n");
        printf("| %-20s | %-10s | %-10s | %-10s | %-20s | %-20s |\n", "Purchase ID", "Book ID", "User ID", "Quantity", "Total Price (RM)", "Time");

        while (row = mysql_fetch_row(res))
        {
            printf("| %-20s | %-10s | %-10s | %-10s | %-20s | %-20s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
        }
        printf("-------------------------------------------------------------------------------------------------------------\n");
    }

    cout << endl << "Press any key to continue ...";
    _getch();
    Bookstore();
}

void Feedback()
{
    system("cls");

    string fname, email, subject, message;
    char choose;

    cout << setw(50) << "Feedback Form\n" << endl;
    cout << "Please fill in the details and give your feedback:" << "\n--------------------------------\n" << endl;

    cout << "Full Name: ";
    getline(cin.ignore(), fname);

    cout << "Email: ";
    getline(cin, email);

    cout << "Subject: ";
    getline(cin, subject);

    cout << "Message: ";
    getline(cin, message);

    cout << endl;

    string insert_query = "INSERT INTO feedback (Fname, Email, Subject, Message) VALUES ('" + fname + "', '" + email + "', '" + subject + "', '" + message + "')";
    const char* q = insert_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        cout << "\nSuccessfully added a feedback, the details are as follows:" << "\n-----------------------------------------------------\n" << endl;

        cout << "Full Name: " << fname << endl
            << "Email: " << email << endl
            << "Subject: " << subject << endl
            << "Message: " << message << endl;

        cout << "\nDo you want to add another feedback? (y/n): ";
        cin >> choose;
        if (choose == 'Y' || choose == 'y')
        {
            system("cls");
            Feedback();
        }
        else if (choose == 'N' || choose == 'n')
        {
            system("cls");
            main();
        }
        else
        {
            exit(1);
        }

    }
    else
    {
        cout << "Insert Failed. Query execution problem!" << mysql_errno(conn) << endl;
    }
}



