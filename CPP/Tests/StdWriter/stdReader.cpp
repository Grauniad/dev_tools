#include <cstring>
#include <iostream>
#include "tester.h"
#include <sstream>
#include "stdReader.h"
#include <cstdio>
#include <vector>
#include "defer.h"

using namespace std;

//Tests
template<class Reader>
int VerifyRead(testLogger& log);
template<class Reader>
int VerifyReadString(testLogger& log);

string quote; 


const char secondString[] = "Oh look!, another string on the end..";

//Validators
bool ValidateMatch( const char *str1, const char *str2, 
                                      long len,
                                      testLogger& log);

template<class Reader>
int VerifyGet(testLogger& log);
template<class Reader>
int VerifyRead(testLogger& log);

int main(int argc, const char *argv[])
{
    stringstream q;
    q << "There is a computer disease that anybody who works with computers knows about." << endl;
    q << "It's a very serious disease and it interferes completely with the work. " << endl;
    q << "The trouble with computers is that you 'play' with them!" << endl;
    q << "Richard P. Feynman " << endl;
    quote = q.str();
    Test("Basic gets",  (loggedTest)VerifyGet<IFStreamReader>).RunTest();
    Test("Read hunks",  (loggedTest)VerifyRead<IFStreamReader>).RunTest();
    Test("Read hunks",  (loggedTest)VerifyReadString<IFStreamReader>).RunTest();
    return 0;
}

bool ValidateMatch( const char *str1, const char *str2, 
                                      long len,
                                      testLogger& log)  
{
    bool match = true;
    long i;
    for (i = 0; i < len; i++) {
        log << i << ": " << str1[i] << " , "  << str2[i] << endl;
        if ( str1[i] != str2[i] ) {
            log << "Character missmatch! (" << i << ")" << endl;
            log << (int)str1[i] << " -> " << (int)str2[i] << endl;
            match = false;
            break;
        }
    }
    if ( match ) {
         log << "arrays matched " << endl;
    }
    return match;
}


// Define a generator for each class to be tested
IFStreamReader* Generator() {

    static long count = 0;
    count++;

    //prepare the file
    stringstream fname;
    fname << "stdReaderTest" << count << ".tmp";
    fname.flush();
    ofstream writer(fname.str().c_str(), ios_base::binary | ios_base::out);
    writer << quote;
    writer.put('\0');
    writer << secondString << endl;
    writer.close();

    return new IFStreamReader(fname.str().c_str());
}


// Define a Delete for each class
void Delete ( IFStreamReader* reader ) { remove(reader->Fname().c_str()); delete reader;
}


template<class Reader>
int VerifyGet(testLogger& log) {
	Reader *reader = Generator();
    // remember to clean up after ourselves:
    DEFER(Delete(reader);)

    vector<int> testInds;
    testInds.reserve(4);
    testInds[0] = 0;
    testInds[1] = quote.length();
    testInds[2] = 10;
    testInds[3] = 50;

    for (int& ind: testInds) {
        char q = quote[ind];
        char r = reader->Get(ind);
        log << ind << ": " << q << " , " << r << endl;
        if (quote[ind] != reader->Get(ind) ){
            log << "MISSMATCH" << endl;
            return 1;
        }
    }
    return 0;
}

template<class Reader>
int VerifyRead(testLogger& log) {

	Reader *reader = Generator();
    // remember to clean up after ourselves:
    DEFER(Delete(reader);)

    char* buffer = new char[quote.length()];
    DEFER(delete [] buffer;)

    vector<int> testInds;
    testInds.reserve(5);
    testInds[0] = 0;
    testInds[1] = 200;
    testInds[2] = 10;
    testInds[3] = 0;
    testInds[4] = 50;

    for (int& ind: testInds) {
        memset(buffer,'\0',quote.length());
        const char *src = quote.c_str() + ind;
        reader->Read(ind, buffer,20);
        if ( !ValidateMatch(src,buffer,20,log) ) {
            log << "Match failure at index: " << ind << endl;
            log << "Quote: " << quote.c_str() + ind << endl;
            log << "Read: " << buffer << endl;
            return 1;
        }
    }
    return 0;
}

template<class Reader>
int VerifyReadString(testLogger& log) {
	Reader *reader = Generator();
    // remember to clean up after ourselves:
    DEFER(Delete(reader);)
    
    string s;
    reader->ReadString(0,s);
    if ( s != quote) {
        log << "Read string s: " << s << endl;
        log << "But should have got: " << quote << endl;
        return 1;
    }
    return 0;
}
