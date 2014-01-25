#include "file.h"
#include "OSTools.h"
#include "stdReader.h"
#include "stdWriter.h"
#include "logger.h"
#include "boost/filesystem/operations.hpp"

using namespace std;

namespace fs = boost::filesystem;
/*
 ***************************************************
 *                FILE POINTER
 ***************************************************
 */
  /*
   * C'tor 
   */
FilePointer::FilePointer(FileSystemObject* f) : file(f) {}
  /*
   * Move C'tor 
   */
FilePointer::FilePointer(FilePointer&& from) {
    // Move the pointer...
    file = from.file;
    from.file = NULL;
}
  /*
   * D'tor 
   */
FilePointer::~FilePointer() {
    if ( file) {
        delete file;
    }
}

bool FilePointer::IsOk() {
    return file!=NULL;
}

  /*
   * Open a file or directory...
   */
FilePointer FilePointer::Open(const string& path) {
    FilePointer file(FileSystemObject::Open(path));
    return std::move(file);
}

FilePointer FilePointer::Open(const boost::filesystem::path path) {
    FilePointer file(FileSystemObject::Open(path));
    return std::move(file);
}

/*
 ***************************************************
 *                FILE SYSTEM OBJECT
 ***************************************************
 */

/*
* C'tor 
*/
FileSystemObject::FileSystemObject( const std::string& spath,
                                    fs::path&& bpath)
    : boost_path(bpath), path(spath)
{ }


/*
* Open
*/
FileSystemObject* FileSystemObject::Open(const std::string& fpath) {
    FileSystemObject* f = NULL;

    // NOTE: Handled off to f in construction
    fs::path boost_path(fpath); 

    if ( is_directory(boost_path))  {
        f = new Directory(fpath, std::move(boost_path));
    } else if ( is_regular_file(boost_path))  {
        f = new File(fpath, std::move(boost_path));
    } else {
       throw InvalidFileException(fpath + ": Unknown file type!");
    }

    return f;
}

FileSystemObject* FileSystemObject::Open(const boost::filesystem::path& path) {
    return Open(path.generic_string());
}

/*
* Path processing...
*/
string FileSystemObject::Path() const {
    return path;
}

string FileSystemObject::ParentDirectory() const {
    return OS::Dirname(path);
}

string FileSystemObject::Name() const {
    return OS::Basename(path);
}

/*
 ***************************************************
 *               FILE 
 ***************************************************
 */
/*
* C'tor 
*/
File::File(const std::string& path, fs::path&& bpath)
    : FileSystemObject(path,std::move(bpath)) {}

/*
* File Access
*/
OFStreamWriter File::Writer() {
    return OFStreamWriter(path.c_str());
}
IFStreamReader File::Reader() {
    return std::move(IFStreamReader(path.c_str()));
}

/*
 ***************************************************
 *               DIRECTORY
 ***************************************************
 */
/*
* C'tor 
*/
Directory::Directory(const std::string& path, fs::path&& bpath)
    : FileSystemObject(path,std::move(bpath)) {}

/*
* Contents
*/
vector<FilePointer> Directory::Contents() {

    vector<FilePointer> files;

    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( boost_path );
          dir_itr != end_iter;
          ++dir_itr )
    {
          
          SLOG_FROM(LOG_VERBOSE, "Directory::Contents",
                    "Opening path: " << dir_itr->path())
          files.emplace_back(
              FilePointer::Open(dir_itr->path()));
    }

    return std::move(files);
}
