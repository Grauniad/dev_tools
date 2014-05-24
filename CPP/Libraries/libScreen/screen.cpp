#include "screen.h"
#include "logger.h"
#include <memory>
#include <array>

using namespace std;

/*****************************************************
 *                       SCREEN
 ****************************************************/

Screen::Screen():
   main(NULL) 
{
    // Memory allocation, capability determination etc...
    initscr();

    // Get Input a char at a time rather than 
    // buffering lines.
    cbreak();

    // User input is echoed to the screen
    echo();

    /* Cursor visibility
     *    0: Invisible
     *    1: Visibil
     *    2: Very Visible
     */
    curs_set(1);

    // get our maximum window dimensions
    getmaxyx(stdscr, height, width);

    /*
     * Initialise main to the full screen
     */
    main = new Terminal(
             newwin(height,width,0,0),
             {height,width,0,0});

}

Screen::~Screen() {
    delete main;
    endwin();
}

/*****************************************************
 *                      WINDOW 
 ****************************************************/

Window::Window(WINDOW* _win, const Window::WIN_INFO& _info) 
    : win(_win), info(_info)
{
}

Window::~Window() {
    delwin(win);
}

void Window::PutString(int x, int y, const std::string& line) {
    if ( x >= info.cols || x < 0 || y >= info.lines || y < 0) {
        /*
         * It doesn't make any sense to do this write, log
         * an error and return.
         */
        SLOG_FROM(LOG_ERROR,"Window::PutString",
            "Out of bounds write! " << x << " , " << y << endl
            << "Limits: " << info.cols << " , "  << info.lines)
    } else {
        // Write the line to the buffer
        mvwprintw(win,y,x,line.c_str());
        // Push the buffer to scren...
        wrefresh(win);
    }
}

/*****************************************************
 *                      TERMINAL 
 ****************************************************/

Terminal::Terminal(WINDOW* _win, const Window::WIN_INFO& info) 
    : Window(_win, info)
{
    // Enable scrolling behaviour
    scrollok(_win,true);
}

std::string Terminal::GetLine(const std::string& prompt) {
    const int BUF_SIZE = 20480;

    wprintw(win,prompt.c_str());

    // This is not very elegant, nor efficient, but lets assume
    // that getting input from the user is not going to be part
    // of a tight loop...
    unique_ptr<array<char,BUF_SIZE>> buffer(new array<char,20480>());

    /*
     * wgetnstr's man page does not specify it gives us a null 
     * termination :(
     */
    int read = wgetnstr(win,buffer->data(),BUF_SIZE-1);

    if ( read == OK ) {
        return string(buffer->data());
    } else {
        LOG_FROM(LOG_ERROR,"Terminal::GetLine", "Failed to read line")
        return "";
    }
    // buffer goes out of scope...
}
