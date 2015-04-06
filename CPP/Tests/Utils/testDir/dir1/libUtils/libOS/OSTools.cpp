#include "OSTools.h"
#include <cstring>

#include <libgen.h>
#include <sys/stat.h>

using namespace std;

string OS::Join(const string& path1, const string& path2) {
    return path1 + "/" + path2;
}

string OS::Basename(const string& path) {
    /*
     * Blame the POSIX standard! 
     */
    char * data = new char[path.length()+1];
    strncpy(data,path.c_str(),path.length()+1);
    string fname(basename(data));

    delete[] data;
    return fname;
}

string OS::Dirname(const string& path) {
    /*
     * Blame the POSIX standard! 
     */
    char * data = new char[path.length()+1];
    strncpy(data,path.c_str(),path.length()+1);
    string directory(dirname(data));

    delete[] data;
    return directory;
}

bool OS::Exists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
