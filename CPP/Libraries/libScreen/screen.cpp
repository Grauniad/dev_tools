#include "screen.h"
#include "logger.h"
#include <memory>
#include <array>

using namespace std;

/*
 * When running in screen mode we want to prevent cout 
 * style logging as it messes up the window...
 */
class ScreenLogger: public LogDevice { 
public:
    ScreenLogger()
    {
        Logger::Instance().RegisterLog(*this);

        // Prevent Logging directly to stdout / stderr
        Logger::Instance().RemoveLog(LogFactory::CERR());
        Logger::Instance().RemoveLog(LogFactory::CLOG());
        Logger::Instance().RemoveLog(LogFactory::COUT());
    }

    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level)
    {
        Screen::Instance().MainTerminal().PutString(
           GenericFormatLogger::Format(message,context,time,level));
    }

    ~ScreenLogger()
    {
        Logger::Instance().RemoveLog(*this);
    }
};


/*****************************************************
 *                       SCREEN
 ****************************************************/

Screen::Screen():
   main(nullptr), topbar(nullptr), topbar_height(20)
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

    // Now we have a main window, start logging...
    logger = new ScreenLogger();
}

Screen::~Screen() {
    // Shut down logging, *before* we kill the main
    // window it is trying to log to
    delete logger;

    delete main;
    endwin();
}

/*****************************************************
 *               SCREEN - Top Bar
 ****************************************************/
Terminal& Screen::TopBar() {
    ShowTopBar();
    return *topbar;
}
/*
 * Initialise the top bar
 */
void Screen::ShowTopBar() {
    /*
     * Push the main window down
     * and startup the top window
     */
    if ( !topbar ) {
        // Shrink the main window
        main->Resize(width,height-topbar_height);

        // Move the main window out of the way
        main->Move(0,topbar_height);

        // Create the topbar in the new space

        topbar = new Terminal(
             newwin(topbar_height,width,0,0),
             {topbar_height,width,0,0});
        topbar->Boxed(true);
        topbar->Refresh();


    } else {
        // top bar already visible...
    }
}

void Screen::KillTopBar() {
    if ( topbar ) {
        // Remove the topbar
        delete topbar;
        topbar = nullptr;

        // Move the main window back to the top
        main->Move(0,0);

        // Grow it to the size of the screen...
        main->Resize(width,height);
    }
}

/*****************************************************
 *                      WINDOW 
 ****************************************************/

Window::Window(WINDOW* _win, const Window::WIN_INFO& _info) 
    : win(_win), info(_info), boxed(false)
{
}

Window::~Window() {
    delwin(win);
}

/*****************************************************
 *            WINDOW - Output
 ****************************************************/
void Window::Refresh() {
    if ( boxed ) {
        box(win,0,0);
    }
    wrefresh(win);
}

void Window::Clear () {
    wclear(win);
    // Push the buffer to scren...
    Refresh();
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
        Refresh();
    }
}

/*****************************************************
 *            WINDOW - Move
 ****************************************************/

 bool Window::Move(int x, int y) {
     bool ok = mvwin(win,y,x) == OK;

     info.start_col = x;
     info.start_line = y;

     SLOG_FROM(LOG_OVERVIEW, "Window::Move",
          "Window moved: " << x << " , " << y << endl)

     // Push the buffer to scren...
     Refresh();

     return ok;
 }

 bool Window::Resize(int cols, int lines ) {
     bool ok = wresize(win,lines,cols) == OK ;

     info.cols = cols;
     info.lines = lines;
     SLOG_FROM(LOG_OVERVIEW, "Window::Resized",
          "Window resized: " << cols << " , " << lines << endl)

     Refresh();
     return ok;
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

void Terminal::PutString(const std::string& text) {
    wprintw(win,text.c_str());
    Refresh();
}

std::string Terminal::GetLine(const std::string& prompt) {
    const int BUF_SIZE = 20480;
    PutString(prompt);
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
