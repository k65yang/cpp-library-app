//TODO: header guard

#include<iostream>

#define ADMINPASSWORD "admin123"
#define DATABASEPATH "./data/database.db"

bool admin_helper_login(std::string password);

int admin_helper_list_patrons();

int admin_helper_add_patron(std::string name);

int admin_helper_ban_unban_patron(std::string name, std::string id, 
                                    int ban_value);

int admin_helper_list_books();

int admin_helper_add_book(std::string title, std::string genre,
                                 std::string first, std::string last);

int admin_helper_remove_book(std::string title);