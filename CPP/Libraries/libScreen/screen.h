#ifndef __DEV_TOOLS_CPP_LIBRARIES_LIB_SCREEN_SCREEN_H__
#define __DEV_TOOLS_CPP_LIBRARIES_LIB_SCREEN_SCREEN_H__
#include <string>
#include <vector>
#include <ncurses.h>

// A pane inside the Screen
class Window;
// A specialisation of a window for 
// terminal based input..
class Terminal;

/*
 * A simplistic wrapper around ncurses which can 
 * be used to drive a basic application with a main
 * terminal and optionally a sider bar and or topbar
 */
class Screen {
public:
    /*
     * Return the ncurses instance
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
    
    /*
     * Return the "main" terminal
     */
    Terminal& MainTerminal() {
        return *main;
    }

    int Height() const { return height; }
    int Width() const { return width; }
private:
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
    /*
     * Screen properties
     */
    int height;
    int width; 
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
    /*
     * Take ownership of the window _win.
     *
     * Realisticly this should only be called by Screen
     */
    Window(WINDOW* _win,const Window::WIN_INFO& info);

    /*
     * Place the string at the position.
     *
     * Behaviour is undefined if x,y is not within the window bounds
     */
    void PutString(int x, int y, const std::string& line);

    /*
     * Delete the window, and release all related resources...
     */
    virtual ~Window();

protected:
    WINDOW* win;
    WIN_INFO info;
};

/*
 * Enhance Window with terminal based actions
 */
class Terminal : public Window {
public:
    Terminal(WINDOW* _win, const Window::WIN_INFO& info);

    /*
     * Get a line from the user...
     */
    std::string GetLine(const std::string& prompt);

private:
    std::vector<std::string> history;
};


#endif
