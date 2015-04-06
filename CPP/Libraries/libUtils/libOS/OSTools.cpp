#include "OSTools.h"
#include <cstring>

#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include "env.h"
#include <mutex>
#include <glob.h>

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

class GlobWrapper {
public:
    static const int flags = GLOB_MARK | GLOB_TILDE_CHECK | GLOB_BRACE;
    static std::mutex glob_mutex;

    GlobWrapper(const char* pattern)
        : ok(0), globLock(glob_mutex)
    {
        ok = glob( pattern, flags, nullptr/* cust err func*/, &results);
    }

    vector<string> GetFileNames() {
        vector<string> fnames;

        if (ok == 0 ) {
            fnames.reserve(results.gl_pathc);

            for (size_t i = 0; i < results.gl_pathc; ++i) {
                const char* fname = results.gl_pathv[i];
                fnames.push_back(fname);
            }
        }

        return fnames;
    }

    ~GlobWrapper() {
        globfree(&results);
    }
private:
    int    ok;
    glob_t results;
    unique_lock<mutex> globLock;
};
std::mutex GlobWrapper::glob_mutex;


std::vector<std::string> OS::Glob(const std::string& path) {

    const char* pattern = path.c_str();

    return GlobWrapper(pattern).GetFileNames();
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

std::string OS::PWD() {
    char * c_pwd = get_current_dir_name();

    std::string pwd(c_pwd);

    free(c_pwd);

    return pwd;
}
