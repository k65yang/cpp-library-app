#include<iostream>
#include <termios.h>
#include<unistd.h>
#include "library_util.h"


int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

std::string getpass(const char *prompt, bool show_asterisk=true) {
    /*
        From http://www.cplusplus.com/articles/E6vU7k9E/
    */

    const char BACKSPACE=127;
    const char RETURN=10;

    std::string password;
    unsigned char ch=0;

    std::cout <<prompt;

    while((ch=getch())!=RETURN)
    {
        if(ch==BACKSPACE)
            {
            if(password.length()!=0)
                {
                    if(show_asterisk)
                    std::cout <<"\b \b";
                    password.resize(password.length()-1);
                }
            }
        else
            {
                password+=ch;
                if(show_asterisk)
                    std::cout <<'*';
            }
    }
    std::cout << std::endl;
    return password;
}