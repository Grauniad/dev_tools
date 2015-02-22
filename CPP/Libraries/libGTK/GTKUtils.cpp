/*
 * GTKUtils.cpp
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#include "GTKUtils.h"

bool GTKUtils::SetWindowTitle(
    Window window,
    const std::string& titleStr,
    Display* display)
{
    // Retrieve the atoms required by the below XChangeProperty call.
    const char* kAtoms[] = { "_NET_WM_NAME", "UTF8_STRING" };
    Atom atoms[2];
    int result = XInternAtoms(
        display,
        const_cast<char**>(kAtoms),
        2,
        false,
        atoms);

    if (result)
    {
        // Set the window title.
        XChangeProperty(
            display,
            window,
            atoms[0],
            atoms[1],
            8,
            PropModeReplace,
            reinterpret_cast<const unsigned char*>(titleStr.c_str()),
            titleStr.size());

        // TODO(erg): This is technically wrong. So XStoreName and friends expect
        // this in Host Portable Character Encoding instead of UTF-8, which I believe
        // is Compound Text. This shouldn't matter 90% of the time since this is the
        // fallback to the UTF8 property above.
        XStoreName(display, window, titleStr.c_str());
    }

    return result;

}
