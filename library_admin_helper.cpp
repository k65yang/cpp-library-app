#include <iostream>
#include<fstream>
#include <sqlite3.h> 
#include "library_admin_helper.h"

// TODO: experiment with a static pointer to DB, so it does not need to be 
//       instantiated every time a helper function is called

// TODO: either get rid of char* error_message or use it instead 
//       of sqlite3_errmsg(DB)


bool admin_helper_login(std::string password) {
    if (password != ADMINPASSWORD) return false;
    else return true;
}

void print_header(std::string type) {
    if (type == "patrons") {
        std::cout << "\tID" << "\tName" << "\tIs Banned?" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    } else if (type == "books") {
        std::cout << "\tTitle" << "\t\t\tFirst" << "\tLast" << "\tGenre" << "\tAvailable" << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    } else std::cerr << "!@!@!@!@!@!@!@!@!@!@ unrecognized header type !@!@!@!@!@!@!@!@!@!@";
    
}


static int callback_list_all(void *unused, int count, 
                                 char **data, char **columns)
{
    /*

    */
    int idx;
    // TODO: print YES or NO for the "boolean" columns
    for (idx = 0; idx < count; idx++) {
        std::cout << "\t" << data[idx];
    }

    std::cout <<  std::endl;

    return 0;
}

static int callback_check_if_exists(void *pUser, int count, 
                                 char **data, char **columns)
{
    /*
        returns 0 if the query yielded a response
        returns 1 if the query did not yield a response
    */
    int *flag = (int*)pUser;
    *flag = 1;
    return 0;
}


int admin_helper_list_patrons() {
    /*

    */
    sqlite3* DB;
    int exit = 0;
    std::string sql_cmd = "select * from patrons;";
    char* error_message;

    exit = sqlite3_open(DATABASEPATH, &DB);
    print_header("patrons");
    exit = sqlite3_exec(DB, sql_cmd.c_str(), callback_list_all, 
                        0, &error_message);
  
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    sqlite3_close(DB);
    std::cout << "\n";

    return exit;
}

int admin_helper_add_patron(std::string name) {
    sqlite3* DB;
    int exit = 0;
    std::string sql_cmd = "insert into patrons (NAME, IS_BANNED)" \
                          "values (\'" + name + "\', 0);";
    char* error_message;

    exit = sqlite3_open(DATABASEPATH, &DB);
    exit = sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, &error_message);
    if (exit) {
        std::cerr << "Error inserting into DB " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    sqlite3_close(DB);

    return exit;
}


int admin_helper_ban_unban_patron(std::string name, std::string id, int ban_value) {
    if (ban_value != 0 && ban_value != 1) {
        // This should not be reached.
        std::cerr << "Invalid ban value" << std::endl;
        return 1;
    }

    sqlite3* DB;
    int exit = 0;
    char* error_message;
    int patron_exists = 0;

    std::string sql_cmd_check_existence;
    std::string attribute_query;
    if (!name.empty()) {
        attribute_query = "NAME=\'" + name + "\'";
    }
    else {
        attribute_query = "ID=" + id;
    }
    sql_cmd_check_existence = "select * from patrons where " + attribute_query + ";";
    // std::cout << sql_cmd_check_existence << std::endl

    exit = sqlite3_open(DATABASEPATH, &DB);
    exit = sqlite3_exec(DB, sql_cmd_check_existence.c_str(), 
                        callback_check_if_exists, &patron_exists, &error_message);
    if (exit) {
        std::cerr << "Error occured in when checking DB " << sqlite3_errmsg(DB)<< std::endl;
        return -1;
    } 
    if (patron_exists) {
        std::string sql_cmd = "update patrons set IS_BANNED=" + std::to_string(ban_value) \
                                + " where " + attribute_query + ";";
        exit = sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, &error_message);
        if (exit) {
            std::cerr << "Error updating DB " << sqlite3_errmsg(DB) << std::endl;
            return -1;
        }
    } else {
        std::cout << "The specified patron does not exist!" << std::endl;
    }
    sqlite3_close(DB);

    return exit;
}

int admin_helper_list_books() {
    sqlite3* DB;
    int exit = 0;
    char* error_message;

    std::string sql_cmd = "select * from books;";
    exit = sqlite3_open(DATABASEPATH, &DB);
    print_header("books");
    exit = sqlite3_exec(DB, sql_cmd.c_str(), callback_list_all, 0, &error_message);
    if (exit) {
        std::cerr << "Error inserting int DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    sqlite3_close(DB);

    return 0;
}

int admin_helper_add_book(std::string title, std::string genre,
                          std::string first, std::string last) {
    sqlite3* DB;
    int exit = 0;
    char* error_message;
    
    std::string sql_cmd = "insert into books "\
                          "(TITLE, AUTHOR_FIRST_NAME, AUTHOR_LAST_NAME, GENRE, IS_AVAILABLE) " \ 
                          "values (\'" + title + "\', " \
                                  "\'" + first + "\', " \
                                  "\'" + last + "\', " \
                                  "\'" + genre + "\', 0);";

    exit = sqlite3_open(DATABASEPATH, &DB);
    exit = sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, NULL);
    if (exit) {
        std::cerr << "Error inserting int DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }
    sqlite3_close(DB);

    return exit;                
}

int admin_helper_remove_book(std::string title) {
    sqlite3* DB;
    int exit = 0;
    char* error_message;
    int book_exists = 0;

    std::string sql_cmd = "select * from books where TITLE=\'" + title + "\';";
    std::cout << sql_cmd << std::endl;
    
    exit = sqlite3_open(DATABASEPATH, &DB);
    exit = sqlite3_exec(DB, sql_cmd.c_str(), callback_check_if_exists, 
                        &book_exists, &error_message);
    if (exit) {
        std::cerr << "Error occured in when checking DB " << sqlite3_errmsg(DB)<< std::endl;
        return -1;
    } 
    if (book_exists) {
        sql_cmd = "delete from books where TITLE=\'" + title + "\' limit 1;";
        exit = sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, &error_message);
        if (exit) {
            std::cerr << "Error removing book from DB: " << sqlite3_errmsg(DB) << std::endl;
            return -1;
        }
    } else {
        std::cout << "The specified book does not exist! Double check spelling." << std::endl;
    }

    return exit;
}