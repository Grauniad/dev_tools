#include <iostream>
#include <fstream>
#include <sstream>
#include <SimpleJSON.h>


using namespace std;
int main(int argc, const char *argv[])
{
    if ( argc != 2 ) {
        cout << "Usage: jsonGen template.json" << endl;
        return 1;
    }
    ifstream file(argv[1]);
    stringstream input;
    input << file.rdbuf();
    cout << spJSON::Gen("OutputJSON", input.str()) << endl;
    return 0;
}
