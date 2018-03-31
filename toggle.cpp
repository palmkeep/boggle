#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <curses.h>
#include <vector>

#include "toggleSession.h"
#include "toggleProject.h"
#include "toggleDatabase.h"


#define KEY_ENTER 10 /* original code 0527 refers to the keypads ENTER key */

using std::vector;
using std::string;

void initNcursesColors()
{
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
}


void printMenu(WINDOW * win, const vector<string>& printVector)
{
    wmove(win, 0, 0);
    auto it = printVector.begin();

    wattron(win, COLOR_PAIR(2));
    waddstr(win, (*(it++)).c_str());
    wattroff(win, COLOR_PAIR(2));

    wattron(win, COLOR_PAIR(1));
    int y = 1;
    wmove(win, y, 0);
    for(it; it != printVector.end(); it++)
    {
        waddstr(win, (*it).c_str());
        y++;
        wmove(win, y, 0);
    }
    wrefresh(win);
}

int startMenu()
{
    vector<string> menuText;
    menuText.push_back("    Toggle");
    menuText.push_back("(1) New session");
    menuText.push_back("(2) New project");
    menuText.push_back("(3) Display statistics");
    menuText.push_back("(4) Settings");
    menuText.push_back("(5) Exit");

    printMenu(stdscr, menuText);


    int cur_x = 1;
    int cur_y = 1;
    wmove(stdscr, cur_y, cur_x);

    int numOptions = 5;
    int choice = 0;
    int ch;
    bool optionChosen = false;
    while(!optionChosen)
    {
        switch ( ch = getch() ) 
        {
            case KEY_UP:
                if (1 < cur_y)
                {
                    cur_y--;
                    wmove(stdscr, cur_y, cur_x);
                }
                break;
            case KEY_DOWN:
                if (cur_y < numOptions)
                {
                    cur_y++;
                    wmove(stdscr, cur_y, cur_x);
                }
                break;
            case KEY_ENTER:
                choice = cur_y;
                optionChosen = true;
                break;
            default:
                int integerVal = (int)ch % 48;
                if ( 1 <= integerVal && integerVal <= numOptions )
                {
                    choice = integerVal;
                    optionChosen = true;
                }
                break;
        }
    }
    return choice;
}


void timer(bool& keepCounting, int& secondsPassed)
{
    wattron(stdscr, COLOR_PAIR(1));
    wmove(stdscr, 2, 0);
    std::string timerPrompt = "Seconds passed: ";
    waddstr(stdscr, timerPrompt.c_str());
    wattroff(stdscr, COLOR_PAIR(1));

    wattron(stdscr, COLOR_PAIR(3));
    int timePos_y, timePos_x;
    getyx(stdscr, timePos_y, timePos_x);
    while(keepCounting)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        wmove(stdscr, timePos_y, timePos_x);
        waddstr( stdscr, (std::to_string(secondsPassed)).c_str() );
        wrefresh(stdscr);
        secondsPassed++;
    }
    wattroff(stdscr, COLOR_PAIR(3));
}

void newSession()
{
    wclear(stdscr);
    wmove(stdscr, 0, 0);
    wattron(stdscr, COLOR_PAIR(2));
    waddstr(stdscr, "    Session in progress");
    wattroff(stdscr, COLOR_PAIR(2));


    bool sessionInProgress = true;
    int sessionDelta = 0;
    std::thread timer_th(timer, std::ref(sessionInProgress), std::ref(sessionDelta));
    wmove(stdscr, 1, 0);
    int ch;
    while(sessionInProgress)
    {
        switch( ch = getch() )
        {
            case 'S':
                sessionInProgress = false;
                break;
            case 'X':
                sessionInProgress = false;
                break;
            default:
                break;
        }

    }
    timer_th.join();

    std::this_thread::sleep_for( std::chrono::milliseconds(2000) );
}

int toggle()
{
    initscr();  // Init Ncurses
    cbreak();   // Disable buffering by the TTY driver and get chars at time of input 
    noecho();   // Do not echo inputs
    keypad(stdscr, TRUE);
    start_color();
    initNcursesColors();


    bool goToExit = false;
    while(!goToExit)
    {
        wclear(stdscr);
        switch( startMenu() )
        {
            case 1:
                newSession();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                goToExit = true;
                break;
            default:
                break;
        }
    }

    /* DOES NOT WORK UNDER i3
    int x, y;
    int a = resizeterm(60, 60);
    getmaxyx(stdscr, y, x);
    */

    endwin();   //End Ncurses, restore terminal
    return 0;
}


int main(int argc, char** argv)
{
    toggle();
    return 0;
}
