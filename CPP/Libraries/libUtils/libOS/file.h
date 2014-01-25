#ifndef __DEV_TOOLS_CPP_LIBRARIES_UTILS_OS_FILE__
#define __DEV_TOOLS_CPP_LIBRARIES_UTILS_OS_FILE__

#include <string>
#include <vector>
#include "boost/filesystem/path.hpp"

class IFStreamReader;
class OFStreamWriter;
class FilePointer;

/*
 * An object on the file system
 */
class FileSystemObject{
public:
   enum FILE_TYPE {
       FILE_TYPE_FILE,
       FILE_TYPE_DIRECTORY
   };

   /*
   * Open a path - return NULL if the path is stupid
   */
   static FileSystemObject* Open(const std::string& path);
   static FileSystemObject* Open(const boost::filesystem::path& path);

   virtual FILE_TYPE Type() const= 0;

   virtual ~FileSystemObject(){}

   std::string Path() const;

   std::string ParentDirectory() const;

   std::string Name() const;

protected:
   FileSystemObject( const std::string& spath,
                     boost::filesystem::path&& bpath);

   boost::filesystem::path boost_path;
   std::string path;
private:
};

class File: public FileSystemObject {
public:
   File(const std::string& path,
        boost::filesystem::path&& bpath);

   // Satisfy base class
   virtual FILE_TYPE Type() const {
       return FILE_TYPE_FILE;
   }

   std::string Contents();

   // File Access
   OFStreamWriter Writer();
   IFStreamReader Reader();
private:
};

class Directory: public FileSystemObject {
public:
   Directory(const std::string& path,
        boost::filesystem::path&& bpath);

   // Satisfy base class
   virtual FILE_TYPE Type() const {
       return FILE_TYPE_DIRECTORY;
   }
   std::vector<FilePointer> Contents() ;
private:
};

/*
 * Convienience pointer if you don't want to manage the memory...
 *
 * Points to a representation of an object on the file system.
 *
 */
class FilePointer {
public:
   virtual ~FilePointer();

   FileSystemObject* operator->() const {
       return file;
   }
   FileSystemObject& operator*() const {
       return *file;
   }

   bool IsOk();
    
   static FilePointer Open(const char* path) {
       return Open(std::string(path));
   }
   static FilePointer Open(const std::string& path);

   static FilePointer Open(const boost::filesystem::path path);

    /*
     *  Pointer may be moved, but not copied
     *
     *  (We're deleting on destruction)
     */
    FilePointer(FilePointer&&);

private:
    FilePointer(FileSystemObject* f);

    FilePointer(const FilePointer&) = delete;
    FilePointer& operator=(const FilePointer&) = delete;

    FileSystemObject* file;
};

class InvalidFileException {
public:
    InvalidFileException(std::string txt): reason(txt) {}
    std::string reason;
};

#endif
