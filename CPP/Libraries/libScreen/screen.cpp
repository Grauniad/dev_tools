#include "screen.h"
#include "logger.h"
#include <memory>
#include <array>
#include <iterator>

using namespace std;
/* TODO:
 *   - Optimize upwards scrolling
 *   - Draw lock?
 *   - Forward Search
 *   - Backwards Search
 */

/*
 * When running in screen mode we want to prevent cout 
 * style logging as it messes up the window...
 */
class ScreenLogger: public LogDevice { 
public:
    ScreenLogger()
    {
        //Logger::Instance().RegisterLog(*this);

        // Prevent Logging directly to stdout / stderr
        Logger::Instance().RemoveLog(LogFactory::CERR());
       // Logger::Instance().RemoveLog(LogFactory::CLOG());
        //Logger::Instance().RemoveLog(LogFactory::COUT());
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

/*
 * Known mouse events...
 */
const int MOUSE_WHEEL_UP   = BUTTON4_PRESSED;
const int MOUSE_WHEEL_DOWN = BUTTON2_PRESSED;

Screen::Screen():
   main(nullptr), topbar(nullptr), topbar_height(20)
{
    // Memory allocation, capability determination etc...
    initscr();

    // Get Input a char at a time rather than 
    // buffering lines.
    cbreak();

    // User input is echoed to the screen
    noecho();

    /* Cursor visibility
     *    0: Invisible
     *    1: Visibil
     *    2: Very Visible
     */
    curs_set(1);

    // Don't care about double clicks, just tell us about the event
    mouseinterval(0);

    // Register an interest in mouse clicks...
    mmask_t    oldMask;
    mousemask(MOUSE_WHEEL_UP | MOUSE_WHEEL_DOWN, &oldMask);

    // get our maximum window dimensions
    getmaxyx(stdscr, height, width);

    /*
     * Initialise main to the full screen
     */
    main = new Terminal(
             newwin(height,width,0,0),
             {width,height,0,0});

    main->StartLessScroll();
    topbar_height = 0.45 * height;
    sidebar_width = 0.45 * width;

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
 *            Screen - Mouse
 ****************************************************/
 void Screen::OnMouseEvent() {
     // Pick the event of the queue
     MEVENT event;

     getmouse(&event);
     Terminal& term = GetEventOwner(event);
     switch ( event.bstate ) {
     case MOUSE_WHEEL_UP:
         LOG_FROM(LOG_VERY_VERBOSE,"Screen::OnMouseEvent","Received Mouse UP")
         term.ScrollUp(1);
         break;
     case MOUSE_WHEEL_DOWN:
         LOG_FROM(LOG_VERY_VERBOSE,"Screen::OnMouseEvent","Received Mouse DOWN")
         term.ScrollDown(1);
         break;
     default:
        SLOG_FROM(LOG_ERROR,"Screen::OnMouseEvent()",
               "Unknown mouse event: " << event.bstate)
        break;
     }
 }

Terminal& Screen::GetEventOwner(MEVENT& event) {
    if ( sidebar && event.x > (width - sidebar_width) ) {
        return *sidebar;
    }
    /*
     * Now know it can't belong to the sidebar...
     */
    if ( !TopBarShowing() ) {
        // Easy - must be in main
        return *main;
    } else {
        // Check which half we are in...
        if ( event.y < topbar_height ) {
            return *topbar;
        } else {
            return *main;
        }
    }
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
        int top_width  = width;
        if ( sidebar ) {
            top_width = width -sidebar_width;
        }

        // Shrink the main window
        main->Resize(top_width,height-topbar_height);

        // Move the main window out of the way
        main->Move(0,topbar_height);
        // Create the topbar in the new space
        topbar = new Terminal(
             newwin(topbar_height,top_width,0,0),
             {top_width,topbar_height,0});
        topbar->Boxed(true);
        topbar->StartNoAutoScroll();
        topbar->Refresh();

        SLOG_FROM(LOG_VERBOSE,"Screen::ShowTopBar()",
           "Enabled top bar: " << endl
           << "Main: " << main->Height() << ", " << main->Width() << endl
           << "Top: " << topbar->Height() << ", " << topbar->Width() << endl
           << "Config: " << height << " , " << topbar_height )



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

bool Screen::SetTopBarHeight(int lines) {
    bool ok = false;
    topbar_height = lines;

    if ( lines < 1 || lines > (height-1) ) {
        SLOG_FROM(LOG_ERROR,"Screen::SetTopBarHeight",
                "Can not resize topbar, invalid height!: " << lines)
    } else if (topbar) {
        int top_width  = width;
        if ( sidebar ) {
            top_width = width -sidebar_width;
        }
        // Shrink the main window
        ok = main->Resize(top_width,height-topbar_height);

        // Move the main window out of the way
        ok &= main->Move(0,topbar_height);

        // Grow the topbar
        ok &= topbar->Resize(top_width,topbar_height);
    }

    return ok;
}

/*****************************************************
 *               SCREEN - Side Bar
 ****************************************************/
Terminal& Screen::SideBar() {
    ShowSideBar();
    return *sidebar;
}

/*
 * Initialise the side bar
 */
void Screen::ShowSideBar() {
    /*
     * Push the main and top window across 
     * and startup the side window
     */
    if ( !sidebar ) {
        // Shrink the main window leftwards, no change in height
        main->Resize(main->Width() - sidebar_width,main->Height());

        // Create the sidebar in the new space
        sidebar = new Terminal(
             newwin(height,sidebar_width,0,width-sidebar_width),
             {sidebar_width,height,0});
        sidebar->Boxed(true);
        sidebar->StartNoAutoScroll();
        sidebar->Refresh();

        SLOG_FROM(LOG_VERBOSE,"Screen::ShowSideBar()",
           "Enabled side bar: " << endl
           << "Main: " << main->Height() << ", " << main->Width() << endl
           << "Side: " << sidebar->Height() << ", " << sidebar->Width() << endl
           << "Config: " << height << " , " << topbar_height )



    } else {
        // top bar already visible...
    }
}

void Screen::KillSideBar() {
    if ( sidebar ) {
        // Remove the topbar
        delete sidebar;
        sidebar = nullptr;

        // Grow Main to re-fill the space...
        main->Resize(width,main->Height());
    }
}

bool Screen::SetSideBarWidth(int cols) {
    bool ok = false;
    sidebar_width = cols;

    if ( cols < 1 || cols > (width-1) ) {
        SLOG_FROM(LOG_ERROR,"Screen::SetSideBarWidth",
                "Can not resize topbar, invalid width!: " << cols)
    } else if (sidebar) {
        // Shrink the main window
        ok = main->Resize(width-sidebar_width,main->Height());

        // Move it across
        ok &= sidebar->Move(width - sidebar_width,0);

        // Grow the sidebar...
        ok &= sidebar->Resize(sidebar_width,height);

    }
    return ok;
}



/*****************************************************
 *                      WINDOW 
 ****************************************************/

Window::Window(WINDOW* _win, const Window::WIN_INFO& _info) 
    : win(_win), info(_info), boxed(false)
{
     SLOG_FROM(LOG_OVERVIEW, "Window::Window",
          "Window Created: " << info.cols << " , " << info.lines << endl)
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
} /*****************************************************
 *            WINDOW - Scrolling
 ****************************************************/
 void Window::ScrollUp(int lines ) {
     wscrl(win,lines);
     Refresh();
 }

 void Window::ScrollDown(int lines ) {
     wscrl(win,-1*lines);
     Refresh();
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
    : Window(_win, info), feed_mode(AUTO_SCROLL), last_line(0), searcher(output)
{

    // Enable scrolling behaviour
    scrollok(win,true);

    // Let curses calculate arrows keys etc for us
    keypad(win,true);

    // Let curses interpret newline (carriage return) for us
    nonl();
}

/*****************************************************
 *             TERMINAL - Printing
 ****************************************************/

void Terminal::PutString(const std::string& text) {
    stringstream feed(text);

    /*
     * ncurses doesn't buffer this for us :(
     */
    size_t last = 0, next = 0;
    int count = 0;
    for (next = text.find_first_of("\n",last);
         next != string::npos;
         next = text.find_first_of("\n",last) )
    {
        int len = next - last;
        if ( len > info.cols -2 ) {
            len = info.cols-2;
        }

        output.emplace_back(text.substr(last,len) + "\n");
        last = next+1;
        ++count;
    }
    if ( last < text.length() ) {
        // More data left in string
        if ( output.size() > 0 && last != 0 ) {
            // Final line didn't end in a newline:
            *(output.end()-1) += text.substr(last);
        } else {
            // There was only one line, and it didn't have a new line
            int len = text.length() - last;
            if ( len > info.cols -2 ) {
                len = info.cols -2;
            }
            output.emplace_back(text.substr(last,len));
            ++count;
        }
    }

    SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::PutString",
           "Pushed lines to terminal buffer, now " << output.size() << " in buffer"
           << "Source Line: " << text )
    FeedAll();
}

std::string Terminal::GetLine(const std::string& prompt, bool storePrompt) {
    if ( storePrompt ) {
        PutString(prompt);
    } else {
        // temorarily clear the current line...
        wclrtoeol(win);
        wprintw(win,prompt.c_str());
    }

    // Store start position so we can print the prompt...
    int startx, starty;
    getyx(win,starty,startx);
   
    echo();
    char buf[4096];
    wgetnstr(win,buf,4095);
    noecho();

    string command(buf);

    // Store the prompt for replay...
    wmove(win,starty,startx);
    if ( storePrompt) {
        PutString(command + "\n");
    }

    return command;
}

/*****************************************************
 *             TERMINAL - Screen Manipulation
 ****************************************************/

void Terminal::Clear () {
    output.clear();
    last_line = 0;
    SLOG_FROM(LOG_VERY_VERBOSE, "Terminal::Clear",
              "Cleared the output buffer, now " << output.size() << " lines in buffer")
    Window::Clear();
}

void Terminal::Redraw() {
    last_line-= info.lines +1;
    FeedLines(info.lines);
}

void Terminal::FeedAll() {
    int to_feed = 0;

    // How many lines between the the current line 
    // and the end of the output...
    int linesRemaining = output.size() - last_line;


    // Vector has been changed - reload the search cache...
    searcher.UpdateCache();

    switch (feed_mode) {
        case AUTO_SCROLL:
            // We have to feed everything...
            to_feed = linesRemaining;
            break;
        case LESS:
            /*
             * Each time we auto scroll, but if the input would go off the 
             * screen we provide less functionality on that output.
             *
             * Since we always feed everything, linesRemaining is the number
             * of lines added before this feed.
             */
            to_feed = linesRemaining;

            if ( linesRemaining > info.lines ) {
                /* Trigger less
                 * NOTE: We feed "nothing" because Less will do the feed for us when
                 *       the user leaves Less mode...
                 */
                to_feed = 0;
                Less(last_line);
            }
            break;
        case NO_AUTO_SCROLL:
            // Only the user is allowed to change last_line...
            if ( static_cast<int>(output.size()) < info.lines ) {
                // enough room, just feed it
                to_feed = linesRemaining;
            } else {
                // Buffer is full, just make sure everything 
                // we can has been printed
                if ( static_cast<int>(last_line) < info.lines ) {
                    to_feed = info.lines - last_line;
                }
            }
            break;
        default:
            break;
    }
    SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::FeedAll",
           "Starting at : " << last_line << endl
           << "lines left in buffer: " << output.size() << endl
           << "screen height: " << info.lines );

    // Feedllines will do the refresh
    FeedLines(to_feed);
}

void Terminal::FeedLines(int n) {
    if ( last_line < output.size() ) {
        /*
         * Check we actually have that many lines to feed
         */
        if ( (last_line + (size_t)n) >= output.size() ) {
            n = output.size() - last_line - 1;
        }

        /*
         * Feed each line...
         */
        for ( size_t stopLine = last_line + static_cast<size_t>(n); 
              last_line <= stopLine; 
              ++last_line ) 
        {
            const string& s = output[last_line];
            if ( searcher.IsMatch(output.begin() + last_line) ) {
                // Do highlighting...
                wattron(win,A_REVERSE);
                SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::Feedllines",
                      "Reversing: " << s)
            } else {
                SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::Feedllines",
                      "Not Reversing: " << s)
            }

            wprintw(win,s.c_str());

            // Reset the terminal colour
            wattroff(win,A_REVERSE);
        }
        SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::Feedllines",
               "Pushed " << n << " out of " << output.size() << " lines to terminal")
        Refresh();
    } else {
        // no more lines left in the buffer...  
        SLOG_FROM(LOG_VERY_VERBOSE,"Terminal::Feedllines", 
              "No lines left to push!" << endl
              << "last_line: " << last_line << endl
              << "Buffered lines: " << output.size() )
    }
}

/*****************************************************
 *             TERMINAL - Scrolling
 ****************************************************/


/* * Currently last_line points to the next line to be printed
 *
 * We wish to roll this back screen_size + lines:
 *   screen_size: Rolling back this much results in as printing the same again
 *   lines: Further rolling back by lines, results in as print previous lines
 *
 */
void Terminal::ScrollUp(int lines ) {
     
    if ( info.lines + lines >= (int)last_line ) {
        // We're back at the start...
        last_line = 0;
    } else {
        // Rollback to the relevant line
        last_line-=lines + 1+info.lines;
    }
    SLOG_FROM(LOG_VERY_VERBOSE, "Terminal::ScrollUp",
             "Scrolling up, " << lines << endl
             << "last_line is now " << last_line << endl
             << "Lines in buffer: " << output.size()
             << "Terminal Height: " << info.lines)
    FeedLines(info.lines);
}

void Terminal::ScrollDown(int lines ) {
    /*
     * Scrolling down is essentially just feeding lines...
     */
     FeedLines(lines);
}

void Terminal::Less(int start) {
    last_line = start -1;
    FeedLines(info.lines);
    bool more = true;
    int gcount = 0;
    bool forwardSearch = true;
    while ( more ) {
        int c = wgetch(win);
        if ( c != 'g' ) {
            gcount = 0;
        }
        switch ( c ) {
        case KEY_END: // End
        case 'G': 
            last_line = output.size() - info.lines;
            FeedLines(info.lines);
            break;
        case 'g': 
            ++gcount;
            if ( gcount != 2 ) {
                break;
            } else {
                // FALL THROUGH TO KEY_HOME
            }
        case KEY_HOME:
            last_line = start -1;
            FeedLines(info.lines);
            break;
        case KEY_PPAGE: //Page up
            ScrollUp(info.lines);
            break;
        case KEY_NPAGE: //Page down
            ScrollDown(info.lines);
            break;
        case KEY_MOUSE:
            // Notify the screen of queued mouse event
            Screen::Instance().OnMouseEvent();
            break;
        case '/':
            forwardSearch = true;
            if ( Search(GetLine("/",false)) ) {
                FindNext();
            }
            break;
        case '?':
            forwardSearch = false;
            if ( Search(GetLine("?",false)) ) {
                FindPrev();
            }
            break;
        case 'n':
            if ( forwardSearch ) {
                FindNext();
            } else {
                FindPrev();
            }
            break;
        case 'N':
            if ( forwardSearch ) {
                FindPrev();
            } else {
                FindNext();
            }
            break;
        case KEY_UP:
        case 'k':
             if (  ( static_cast<int>(last_line) - info.lines) > start ) {
                 ScrollUp(1);
             }
             break;
        case 13:  // carraige return
        case KEY_DOWN:
        case 'j': 
             ScrollDown(1);
             break;
        case 'q':
            more = false;
            break;
        default:
            SPRINT("Got: " << c)
            break;
        }
        if ( more && ( static_cast<int>(last_line) - info.lines ) < start ) {
            last_line = start -1;
            FeedLines(info.lines);
        }
    }

    // All Done, reset the window back to its previous state...
    last_line = output.size() - info.lines;
    FeedLines(info.lines);
}

/*****************************************************
 *             TERMINAL - Searching
 ****************************************************/
bool Terminal::Search(const string& pattern) {
    bool match = searcher.Search(pattern);
    if ( match ) {
        SLOG_FROM(LOG_VERBOSE,"Terminal::Search",
                  "Found matches for: " << pattern)
    } else {
        SLOG_FROM(LOG_VERBOSE,"Terminal::Search",
                  "Found no matches for: " << pattern)
    }
    return match;
}

void Terminal::FindNext() {
    // If we don't have enough to fill the screen, we can't move
    if ( static_cast<int>(output.size()) > info.lines ) {
        int topOfWin = last_line - info.lines;
        int distanceToEnd = output.size() - topOfWin;
        // We can't scroll past the end
        if ( distanceToEnd > info.lines ) {
            auto it = searcher.Next(output.begin() + topOfWin + 1);

            // Check there is actualyl a result
            if ( it != searcher.NoPos() ) {
                // last_line points to the next line which will be printed
                // Bottom of the screen will be used for input
                last_line = std::distance(output.cbegin(),it) -2;
                FeedLines(info.lines);
            }
        }
    }
}

void Terminal::FindPrev() {
    // If we don't have enough to fill the screen, we can't move
    if ( static_cast<int>(output.size()) > info.lines ) {
        int topOfWin = last_line - info.lines;
        // Check we actually have room to go backwards
        if ( topOfWin > 0 ) {
            auto it = searcher.Prev(output.begin() + topOfWin);

            // Check there is actualy a result
            if ( it != searcher.NoPos() ) {
                // last_line points to the next line which will be printed
                // Bottom of the screen will be used for input
                last_line = std::distance(output.cbegin(),it) -2;
                FeedLines(info.lines);
            }
        }
    }
}

void Terminal::SearchOff() {
    searcher.Reset();
}
