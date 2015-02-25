#include "csvDynamic.h"
#include "tester.h"
#include "stdReader.h"
#include "tester.h"

using namespace std;

int ReadFile(testLogger& log);
int ManuallyAddRows(testLogger& log);
int ManuallyAddVectors(testLogger& log);

int main(void) {
    Test("Reading file...",ReadFile).RunTest();
    Test("Manually adding rows...",ManuallyAddRows).RunTest();
    Test("Manually adding rows (as vectors)...",ManuallyAddVectors).RunTest();
    return 0;
}

int ManuallyAddVectors(testLogger& log) {
    CSV_rows<long>  rows = CSV_rows<long>::MakeEmpty();
    rows.AddRow(std::vector<long>({1,2,3}));
    rows.AddRow(std::vector<long>({}));
    rows.AddRow(std::vector<long>({4,5,8,12,15}));
    rows.AddRow(std::vector<long>({-1,-2}));

    auto& row = rows.GetRow(0);
    if ( row.size() != 3 ) {
        log << "Expected size of row 0 to be 3, but is " << row.size() << endl;
        return 1;
    }

    if ( row[0] != 1 || row[1] != 2 || row [2] != 3) {
        log << "Data error on row 1!" << endl;
        return 1;
    }

    auto& row2 = rows.GetRow(1);
    if ( row2.size() != 0) {
        log << "Expected size of row 1 to be 0, but is " << row2.size() << endl;
        return 1;
    }

    auto& row3 = rows.GetRow(2);
    if ( row3.size() != 5) {
        log << "Expected size of row 0 to be 3, but is " << row3.size() << endl;
        return 1;
    }

    if ( row3[0] != 4 || row3[1] != 5 || row3[2] != 8 || row3[3] != 12
         || row3[4] != 15 )
    {
        log << "Data error on row 3!" << endl;
        return 1;
    }

    auto& row4 = rows.GetRow(3);
    if ( row4.size() != 2) {
        log << "Expected size of row 4 to be 2, but is " << row4.size() << endl;
        return 1;
    }

    if ( row4[0] != -1 || row4[1] != -2 )
    {
        log << "Data error on row 4!" << endl;
        return 1;
    }
    return 0;
}
int ManuallyAddRows(testLogger& log) {
    CSV_rows<long>  rows = CSV_rows<long>::MakeEmpty();
    rows.AddRow(1,2,3);
    rows.AddRow();
    rows.AddRow(4,5,8,12,15);
    rows.AddRow(-1,-2);

    auto& row = rows.GetRow(0);
    if ( row.size() != 3 ) {
        log << "Expected size of row 0 to be 3, but is " << row.size() << endl;
        return 1;
    }

    if ( row[0] != 1 || row[1] != 2 || row [2] != 3) {
        log << "Data error on row 1!" << endl;
        return 1;
    }

    auto& row2 = rows.GetRow(1);
    if ( row2.size() != 0) {
        log << "Expected size of row 1 to be 0, but is " << row2.size() << endl;
        return 1;
    }

    auto& row3 = rows.GetRow(2);
    if ( row3.size() != 5) {
        log << "Expected size of row 0 to be 3, but is " << row3.size() << endl;
        return 1;
    }

    if ( row3[0] != 4 || row3[1] != 5 || row3[2] != 8 || row3[3] != 12
         || row3[4] != 15 )
    {
        log << "Data error on row 3!" << endl;
        return 1;
    }

    auto& row4 = rows.GetRow(3);
    if ( row4.size() != 2) {
        log << "Expected size of row 4 to be 2, but is " << row4.size() << endl;
        return 1;
    }

    if ( row4[0] != -1 || row4[1] != -2 )
    {
        log << "Data error on row 4!" << endl;
        return 1;
    }
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
