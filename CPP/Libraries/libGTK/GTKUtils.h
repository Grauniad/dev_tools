/*
 * GTKUtils.h
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#ifndef DEV_TOOLS_GTKUTILS_H_
#define DEV_TOOLS_GTKUTILS_H_


#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <string>

namespace GTKUtils {
    /*
     * Set the title on a window
     *
     * @returns true on success, false otherwise
     *
     */
    bool SetWindowTitle(
        Window window,
        const std::string& titleStr,
        Display* display);
};




#endif /* GTKUTILS_H_ */
