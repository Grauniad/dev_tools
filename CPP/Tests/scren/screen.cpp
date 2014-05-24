#include "screen.h"
#include <unistd.h>
#include <iostream>

using namespace std;
int main(int argc, const char *argv[])
{
    Terminal& t = Screen::Instance().MainTerminal();
    string s = t.GetLine("> ");
    t.PutString(0,5,"Got: " + s);
    sleep(3);
    return 0;
}
