#include "OSTools.h"

#include <libgen.h>

using namespace std;

string OS::Join(const string& path1, const string& path2) {
    return path1 + "/" + path2;
}

string OS::Basename(const string& path) {
    /*
     * Blame the POSIX standard! 
     *
     * The GNU libc library will not modify the input parameter, so it is safe
     * to cast away constness here.
     */
    return basename((char *)path.c_str());
}

string OS::Dirname(const string& path) {
    /*
     * Blame the POSIX standard! 
     *
     * The GNU libc library will not modify the input parameter, so it is safe
     * to cast away constness here.
     */
    return dirname((char *)path.c_str());
}
