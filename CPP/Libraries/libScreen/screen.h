#ifndef __DEV_TOOLS_CPP_LIBRARIES_LIB_SCREEN_SCREEN_H__
#define __DEV_TOOLS_CPP_LIBRARIES_LIB_SCREEN_SCREEN_H__
#include <string>
#include <vector>
#include <ncurses.h>
#include "search.h"

// A pane inside the Screen
class Window;
// A specialisation of a window for 
// terminal based input..
class Terminal;

class ScreenLogger;

/*
 * A simplistic wrapper around ncurses which can 
 * be used to drive a basic application with a main
 * terminal and optionally a sider bar and or topbar
 */
class Screen {
public:
    /*********************************
     *          C'tor / D'tor
     ********************************/
    /* * Return the ncurses instance
     */
    static Screen& Instance() {
        static Screen instance;
        return instance;
    }
    
    /*
     * Clean up the ncurses library and any related
     * resources
     */
    virtual ~Screen();

    /*********************************
     *          Mouse
     ********************************/
     /*
      * Triggered when an input device detects a queued mouse
      * event
      */
     virtual void OnMouseEvent();
    
    /*********************************
     *          Main
     ********************************/

    /*
     * Return the "main" terminal
     */
    Terminal& MainTerminal() {
        return *main;
    }

    /*********************************
     *          Top Bar
     ********************************/

    /*
     * Return the top bar object
     *
     * If the topbar isn't currently active it is initialised
     */
    Terminal& TopBar();

    // Is the topbar in use?
    bool TopBarShowing() {
        return topbar != nullptr;
    }

    /*
     * Destroy the top bar window 
     */
    void KillTopBar();

    /*
     * Resize the topbar
     */
    bool SetTopBarHeight(int lines);

    /*********************************
     *          Side Bar
     ********************************/

    /*
     * Return the side bar object
     *
     * If the sidebar isn't currently active it is initialised
     */
    Terminal& SideBar();

    // Is the topbar in use?
    bool SideBarShowing() {
        return sidebar != nullptr;
    }

    /*
     * Destroy the top bar window 
     */
    void KillSideBar();

    /*
     * Resize the topbar
     */
    bool SetSideBarWidth(int cols);



    /*********************************
     *          Data Access
     ********************************/

    /*
     * Access fundamental properties of the screen
     */
    int Height() const { return height; }
    int Width() const { return width; }
private:
    /*
     * Initialise the top bar
     */
    void ShowTopBar();

    /*
     * Initialise the side bar
     */
    void ShowSideBar();

    Terminal& GetEventOwner(MEVENT& event);

    /*
     * Initialise the ncurses library.
     *
     * We implement as a singleton since it doesn't
     * make any sence to initialise the library multiple
     * times...
     */
    Screen();

    /*
     * Sub-windows
     */
    Terminal*  main;
    Terminal*  topbar;
    Terminal*  sidebar;

    /*
     * Screen properties
     */
    int height;
    int width; 
    int topbar_height;
    int sidebar_width;

    /*
     * Helper Objects
     */
    ScreenLogger* logger;
};


/*
 * Wraps an ncurses window
 */
class Window {
public:
    struct WIN_INFO{
        int cols;
        int lines;
        int start_col;
        int start_line;
    };

    /*********************************
     *          C'tor / D'tor
     ********************************/
    /*
     * Take ownership of the window _win.
     *
     * Realisticly this should only be called by Screen
     */
    Window(WINDOW* _win,const Window::WIN_INFO& info);

    /*
     * Delete the window, and release all related resources...
     */
    virtual ~Window();

    /*********************************
     *          Output
     ********************************/

     /*
      * Refresh the Window();
      */
     virtual void Refresh();

    /*
     * Place the string at the position.
     *
     * Behaviour is undefined if x,y is not within the window bounds
     */
    virtual void PutString(int x, int y, const std::string& line);

    /*
     * Clear down the screen
     */
    virtual void Clear();


    /*********************************
     *          Scrolling
     ********************************/
    virtual void ScrollUp(int lines);
    virtual void ScrollDown(int lines);

    /*********************************
     *          Move Commands
     ********************************/

    /*
     * Move the window to X, Y
     */
    bool Move( int x, int y);

    /*
     * Move the window to X, Y
     */
    bool Resize( int cols, int lines);

    void Boxed(bool yesno) {
        boxed = yesno;
    }

    /*********************************
     *          Data
     ********************************/

     int Height() const { return info.lines; }
     int Width() const { return info.cols; }

protected:
    WINDOW* win;
    WIN_INFO info;
    bool boxed;
};

/*
 * Enhance Window with terminal based actions
 */
class Terminal : public Window {
public:

    Terminal(WINDOW* _win, const Window::WIN_INFO& info);

    /*********************************
     *          Printing
     ********************************/

    /*
     * Get a line from the user...
     *
     *   prompt: Text to display to the user before getting the input...
     *   storePrompt: If this is set to true, prompt is added to the output buffer,
     *                oterwise it will be cleared away when next time the screen is
     *                refreshed...
     */
    std::string GetLine(const std::string& prompt,
                        bool storePrompt = true);

    /*
     * Put a new line of output...
     */
    void PutString(const std::string& text);

    /*********************************
     *     Screen Manipulation
     ********************************/

    /*
     * Clear down the screen
     */
    virtual void Clear();

    /*
     * Feed the next n lines from the buffer
     */
    void FeedLines(int n);

    /*
     * Push all of the remaing lines to the buffer
     */
    void FeedAll();

    void Redraw();

    /*********************************
     *          Scrolling
     ********************************/

    /*
     * Start a less on the output, starting at line <start>
     */
    void Less(int start);

    virtual void ScrollUp(int lines);
    virtual void ScrollDown(int lines);

    /* Configure the scrolling behaviour....
     *
     *  Defines the behaviour when the number of lines in a
     *  string provided to print excedes the number of lines 
     *  available to the terminal...
     */
    enum FEED_MODE { NO_AUTO_SCROLL,  // Stop printing when the screen is full
                     AUTO_SCROLL,     // Print all lines, pushing old lines of the screen
                     LESS };          // Stop printing when the screen is full and engage less mode
    void StartAutoScroll () { feed_mode = AUTO_SCROLL;}
    void StartNoAutoScroll () { feed_mode = NO_AUTO_SCROLL; }
    void StartLessScroll () { feed_mode = LESS; }

    /*********************************
     *          Searching
     ********************************/

     bool Search(const std::string& pattern);
     void FindNext();
     void FindPrev();

private:
    FEED_MODE        feed_mode;
    std::vector<std::string> output;
    size_t           last_line;
    Searcher         searcher;
};


#endif
