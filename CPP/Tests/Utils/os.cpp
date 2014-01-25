#include "tester.h"
#include "dataLump.h"
#include "OSTools.h"
#include "stdReader.h"
#include "file.h"

using namespace std;

int FileContents(testLogger& log);
int OpenDirs(testLogger& log);
int Paths(testLogger& log);

int main(int argc, const char** argv) {
    Test("Check Path operations....", Paths).RunTest();
    Test("Check Directory Iteration....", OpenDirs).RunTest();
    Test("Check File contents read...", FileContents).RunTest();
    return 0;
}

int Paths (testLogger& log) {
    if ( OS::Basename("/home/shusaku/wins.txt") != "wins.txt") {
        log << "Incorrect basename returned by OS::Basename" << endl;
        return 1;
    }

    if ( OS::Dirname("/home/shusaku/wins.txt") != "/home/shusaku") {
        log << "Incorrect directory returned by OS::Dirname" << endl;
        return 1;
    }

    if ( OS::Join("/home/shusaku", "wins.txt") != "/home/shusaku/wins.txt" ) {
        log << "Incorrect path returned by OS::Join" << endl;
        return 1;
    }

    return 0;
}

int FileContents(testLogger& log ) {
    FilePointer f = FilePointer::Open("ROOT/home/test/.testrc");
    File& file = static_cast<File&>(*f);

    std::string content = file.Contents();
    if ( content != "Hello World! \n") {
        log << "Invalid file contents!" << endl;
        log << ">" << content << "<" << endl;
        return 1;
    }
    return 0;
}

int OpenDirs(testLogger& log) {
    FilePointer f = FilePointer::Open("ROOT/home/test/");
    if ( !f.IsOk() ) {
        log << "Failed to open directory :(" <<endl;
        return 1;
    }

    if ( f->ParentDirectory() != "ROOT/home" ) {
        log << "Failed to identify parent directory: " << f->ParentDirectory() << endl;
        return 2;
    }

    if ( f->Name() != "test" ) {
        log << "Failed to identify directory name: " << f->Name() << endl;
        return 3;
    }

    if ( f->Type() != FileSystemObject::FILE_TYPE_DIRECTORY) {
        log << "Invalid file type!" << endl;
        return 4;
    } else {
        // Open the directory....
        Directory& dir= static_cast<Directory&>(*f);
        auto files = dir.Contents();

        if ( files.size() != 2 ) {
            log << "Invalid dir size!" << endl;
            return 5;
        }

        for ( FilePointer& file : files) {
            if ( file->Name() == "Documents") {
                if ( file->Type() != FileSystemObject::FILE_TYPE_DIRECTORY ) {
                    log << "Documents should have been a directory!" << endl;
                    return 9;
                } else {
                    auto documents =static_cast<Directory&>(*file).Contents(); 

                    if ( documents.size() != 2 ) {
                        log << "Invalid documents size!: " << documents.size();
                        return 5;
                    }

                    for ( FilePointer& doc : documents) {
                        if (    doc->Name() != "doc1.txt" 
                             && doc->Name() != "doc2.txt") 
                        {
                            log << "Invalid document name!: " << doc->Name();
                            return 10;
                        }
                    }
                }
            } else if (file->Name() == ".testrc" ) {
                if ( file->Type() != FileSystemObject::FILE_TYPE_FILE ) {
                    log << ".testrc should be afile" << endl;
                    return 7;
                } else {
                    File& testrc = static_cast<File&>(*file);
                    IFStreamReader reader = testrc.Reader();
                    char buffer[100];
                    reader.Read(0,buffer,12);
                    buffer[12] = '\0';
                    if ( std::string(buffer) != "Hello World!") {
                        log << "Inavlid file contents: " << buffer << endl;
                        return 8;
                    }
                }
            } else {
                log << "Unexpected file: >" << file->Name() << "<" << endl;
                return 6;
            }
        }
    }
    return 0;
}
