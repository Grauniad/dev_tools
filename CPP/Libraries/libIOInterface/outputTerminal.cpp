
#include "outputTerminal.h"
#include "tokenizor.h"

using namespace std;

void TestTerminal::PutString(const string& output) {
    Parts outputLines(output,"\n");
    for ( const std::string& line: outputLines ) {
        lines.push_back(line);
    }
}

std::string TestTerminal::OutputAsString() {
    string output;
    for ( const string& line: lines) {
        if ( output != "" ) {
            output += "\n";
        }
        output += line;
    }
    return output;
}

void TestTerminal::Reset() {
    lines.clear();
}
