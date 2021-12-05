#include<iostream>
#include<ctype.h>
#include<boost/algorithm/string.hpp>
#include<sqlite3.h> 
#include<vector>
#include "library_admin_helper.h"
#include "library_util.h"

int admin_user_options_menu() {
    /*

    */
    int response;
    std::string option;
    std::cout << "\nWhat do you want to do today, o great admin?" 
              << "\n1. List all patrons"
              << "\n2. Add patron"
              << "\n3. Ban patron"
              << "\n4. Unban patron"
              << "\n5. List all books"
              << "\n6. Add book"
              << "\n7. Remove book"
              << "\nEnter option number: ";
    std::getline(std::cin, option);
    std::cout << std::endl;

    if (option == "1") {
        std::cout << "Listing all patrons... " << std::endl;
        admin_helper_list_patrons();

    } else if (option == "2") {
        std::string name;
        std::cout << "Enter name of patron: ";
        std::getline(std::cin, name);
        response = admin_helper_add_patron(name);
        if (!response) {
            std::cout << "Successfully added " << name << std::endl;
            admin_helper_list_patrons();
        } else std::cerr << "Error adding patron." << std::endl;

    } else if (option == "3") {
        std::string name; std::string id;
        std::cout << "\nUh oh. The ban hammer is coming."
                  << "\nEnter the name of the patron you wish to ban " 
                  << "(leave blank to ban using ID number): ";
        std::getline(std::cin, name);
        if (name.empty()) {
            std::cout << "\nEnter the id number of the patron: ";
            std::getline(std::cin, id);
        }

        response = admin_helper_ban_unban_patron(name, id, 1);
        if (!response) std::cout << "Ban successful." << std::endl;
        else std::cerr << "Error banning patron." << std::endl;

    } else if (option == "4") {
        std::string name; std::string id;
        std::cout << "\nFeeling merciful today, eh?"
                  << "\nEnter the name of the patron you wish to unban " 
                  << "(leave blank to unban using ID number): ";
        std::getline(std::cin, name);
        if (name.empty()) {
            std::cout << "\nEnter the id number of the patron: ";
            std::getline(std::cin, id);
        }

        response = admin_helper_ban_unban_patron(name, id, 0);
        if (!response) std::cout << "Unban successful" << std::endl;
        else std::cerr << "Error unbanning patron." << std::endl;

    } else if (option == "5") {
        std::cout << "Listing all books... " << std::endl;
        admin_helper_list_books();

    } else if (option == "6") {
        std::string title;       std::string genre;
        std::string last_name;  std::string first_name;
        
        std::cout << "Adding a new book..." << std::endl;
        std::cout << "Enter title of the book: ";
        std::getline(std::cin, title);
        std::cout << "Enter first name of book author: ";
        std::getline(std::cin, first_name);
        std::cout << "Enter last name of book author: ";
        std::getline(std::cin, last_name);
        std::cout << "Enter book genre (one word description only): ";
        std::getline(std::cin, genre);

        response = admin_helper_add_book(title, genre, first_name, last_name);
        if (!response) std::cout << "Book add successful" << std::endl;
        else std::cerr << "Error adding book" << std::endl;
    } else if (option == "7") {
        std::string title;
        std::cout << "Removing a book..." << std::endl;
        std::cout << "Enter title of bok: ";
        std::getline(std::cin, title);

        response = admin_helper_remove_book(title);
        if (!response) std::cout << "Book removal successful" << std::endl;
        else std::cerr << "Error removing book patron." << std::endl;
    }

    return 0;
}

int admin_user_login_menu() {
    /*

    */
    std::cout << "\nWelcome to the admin user menu" << std::endl;

    int retries = 3;
    while (retries-- > 0) {
        // TODO: hide the password as it is typed.
        std::string password = getpass("Please enter the admin password: ",true);
        // std::getline(std::cin, password);
        if (admin_helper_login(password)) {
            std::cout << "\nSuccessfully logged in as admin user." << std::endl;
            while (true) {
                admin_user_options_menu();
            }
            break;
        } else std::cout << "\nIncorrect password. Please try again. Retries remaining: " << retries << std::endl;
    }

    return 0;
}

int patron_user_login_menu() {
    /*

    */
    std::cout << "Welcome to the guest user menu" << std::endl;
    return 0;
}

int init_app() {
    sqlite3* DB;
    int exit = 0;
    char* error_message;
    exit = sqlite3_open(DATABASEPATH, &DB);
    if (exit) {
        std::cerr << "Error occured opening the DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    std::string sql_cmd_init_patrons_table = "create table if not exists patrons " \
                                             "(ID        INT PRIMARY KEY," \ 
                                             " NAME      TEXT             NOT NULL,"\
                                             " IS_BANNED INT              NOT NULL);";
    std::string sql_cmd_init_books_table = "create table if not exists books ("\
                                           "TITLE              TEXT  NOT NULL, "\
                                           "AUTHOR_FIRST_NAME  TEXT  NOT NULL, "\
                                           "AUTHOR_LAST_NAME   TEXT  NOT NULL, "\
                                           "GENRE              TEXT  NOT NULL, "\
                                           "IS_AVAILABLE       INT   NOT NULL);";

    std::vector<std::string> sql_cmds;
    sql_cmds.push_back(sql_cmd_init_books_table);
    sql_cmds.push_back(sql_cmd_init_patrons_table);
    for (auto &cmd : sql_cmds) {
        exit = sqlite3_exec(DB, cmd.c_str(), NULL, 0, &error_message);
        if (exit) {
            std::cerr << "Error performing DB operations: " << sqlite3_errmsg(DB) << std::endl;
            return -1;
        }
    }

    return exit;
}

int main() {
    /*

    */
    // Command line window for selecting options
    // TODO: initialize the sql tables
    int init_response = init_app();
    if (init_response != 0) {
        std::cerr << "Error during initialization. Exiting..." << std::endl;
    }
    while (true) {
        std::string user;
        std::cout << "Select User Type (Enter number)" 
                  << "\n1. ADMIN" 
                  << "\n2. PATRON" 
                  << "\nUser Type:";
        std::getline(std::cin, user);
        boost::to_upper(user);

        if (user == "ADMIN") {
            admin_user_login_menu();
            break;
        } else if (user == "PATRON") {
            patron_user_login_menu();
            break;
        } else {
            std::cout << "Invalid user type. Please try again." << std::endl;
        }
    }


    return 0;
}