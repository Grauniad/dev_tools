#include "csvDynamic.h"
#include "tester.h"
#include "stdReader.h"
#include "tester.h"

using namespace std;

int ReadFile(testLogger& log);

int main(void) {
    Test("Reading file...",ReadFile).RunTest();
    return 0;
}

int ReadFile(testLogger& log) {
    IFStreamReader in_f("test2.data");
    CSV_rows<double> csv(CSV_rows<double>::LoadCSV(in_f));
    if ( csv.Rows() != 2 ) {
        log << "Expected 2 rows, but got " << csv.Rows() << endl;
        return 1;
    }
    // Check row 1
    std::vector<double>& v = csv.GetRow(0);
    if ( v.size() != 2 ) {
        log << "Expected size of v to be 2, but it is " << v.size() << endl;
        return 1;
    }
    double errTol = 0.0000001;
    if ( v[0]  < (0.13 - errTol) || v[0]  > (0.13 + errTol) || 
         v[1] < (0.79 - errTol)  || v[1]  > (0.79 + errTol )) {
        log << " Data error" << endl;
        log << "v0: " << v[0] << endl;
        log << "v1: " << v[1] << endl;
        return 1;
    }
    // Check row 2
    std::vector<double>& v2 = csv.GetRow(1);
    if ( v2.size() != 4 ) {
        log << "Expected size of v2 to be 2, but it is " << v2.size() << endl;
        return 1;
    }
    if ( v2[0] < (1 - errTol)  || v2[0]  > (1 + errTol ) || 
         v2[1] < (2 - errTol)  || v2[1]  > (2 + errTol ) ||
         v2[2] < (3 - errTol)  || v2[2]  > (3 + errTol ) ||
         v2[3] < (4 - errTol)  || v2[3]  > (4 + errTol )
       ) 
    {
        log << " Data error" << endl;
        log << "v0: " << v[0] << endl;
        log << "v1: " << v[1] << endl;
        log << "v2: " << v[2] << endl;
        log << "v3: " << v[3] << endl;
        return 1;
    }
    return 0;
}
