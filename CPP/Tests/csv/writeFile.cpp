
#include <string>
#include "tester.h"
#include "csv.h"
#include "stdWriter.h"
#include "dataLump.h"
#include <iostream>
#include "logger.h"
#include "binaryDescribe.h"
#include "stdReader.h"


using namespace std;

int AddRows(testLogger& log );
int AddRowsWithReserve(testLogger& log );
int CopyConstruct(testLogger& log );
int MoveConstruct(testLogger& log );
int CopyAssignment(testLogger& log );
int MoveAssignment(testLogger& log );
int CopyRows(testLogger& log);
int RemoveRows(testLogger& log);

int WriteFile(testLogger& log);
int ReadFile(testLogger& log);
int FastReadFile(testLogger& log);
const int rows = 1000;

using DataFile = CSV<double,float,int,long>;

int main(int argc, const char *argv[]) {
    Test("Adding rows without reserving new space...",AddRows).RunTest();
    Test("Adding rows with reserved space...",AddRowsWithReserve).RunTest();
    Test("Copy Constructing...",CopyConstruct).RunTest();
    Test("Move Constructing...",MoveConstruct).RunTest();
    Test("Copy Assigning...",CopyAssignment).RunTest();
    Test("Move Assigning...",MoveAssignment).RunTest();
    Test("Copying Rows...",CopyRows).RunTest();
    Test("Removing Rows...",RemoveRows).RunTest();

    Test("Writing data file...",WriteFile).RunTest();
    Test("Loading Data...",ReadFile).RunTest();
    Test("Loading Data...",FastReadFile).RunTest();

    return 0;
}

DataFile GetCSV() {
    DataFile csv;
    for (int i=0; i<rows; i++) {
        csv.AddRow(i*double(0.13),i*float(0.79),i*i,i*i*i);
    }
    return csv;
}

int ValidateCSVCells(DataFile& file) {
    for (int row=0; row<rows; row++) {
        if ( file.GetCell<0>(row) != row*double(0.13) ) {
            SPRINT("(Row: " << row << ") Expected file<d>(row) = " << double(row*0.13) << " but got: " << file.GetCell<0>(row)) 
            return 1;
        } else if ( file.GetCell<1>(row) != row*float(0.79) ) {
            SPRINT("(Row: " << row << ") Expected file<f>(row) = " << float(row*0.79) << " but got: " << file.GetCell<1>(row)) 
            return 1;
        } else if ( file.GetCell<2>(row) != row*row) {
            SPRINT("(Row: " << row << ") Expected file<i>(row) = " << row*row << " but got: " << file.GetCell<2>(row)) 
            return 1;
        } else if ( file.GetCell<3>(row) != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected file<l>(row) = " << row*row*row << " but got: " << file.GetCell<3>(row)) 
            return 1;
        }

        // All good, now modify the value
        file.GetCell<0>(row) = 0*row;
        file.GetCell<1>(row) = 1*row;
        file.GetCell<2>(row) = 2*row;
        file.GetCell<3>(row) = 3*row;

        if ( file.GetCell<0>(row) != row*double(0) ) {
            SPRINT("(Row: " << row << ") Expected file<d>(row) = " << row*double(0) << " but got: " << file.GetCell<0>(row)) 
            return 1;
        } else if ( file.GetCell<1>(row) != row*float(1) ) {
            SPRINT("(Row: " << row << ") Expected file<f>(row) = " << row*float(1) << " but got: " << file.GetCell<1>(row)) 
            return 1;
        } else if ( file.GetCell<2>(row) != row*2) {
            SPRINT("(Row: " << row << ") Expected file<i>(row) = " << row*2 << " but got: " << file.GetCell<2>(row)) 
            return 1;
        } else if ( file.GetCell<3>(row) != row*3 ) {
            SPRINT("(Row: " << row << ") Expected file<l>(row) = " << row*3 << " but got: " << file.GetCell<3>(row)) 
            return 1;
        }

        // OK, now reset back to the correct values
        file.GetCell<0>(row) = double(0.13)*row;
        file.GetCell<1>(row) = float(0.79)*row;
        file.GetCell<2>(row) = row*row;
        file.GetCell<3>(row) = row*row*row;
    }
    return 0;
}
int ValidateCSVColumns_Columns(DataFile& file) {
    auto& d = file.GetColumn<0>();
    auto& f = file.GetColumn<1>();
    auto& i = file.GetColumn<2>();
    auto& l = file.GetColumn<3>();
    for (int row=0; row<rows; row++) {
        if ( d[row] != row*double(0.13) ) {
            SPRINT("(Row: " << row << ") Expected d(row) = " << double(row*0.13) << " but got: " << d[row]) 
            return 1;
        } else if ( f[row] != row*float(0.79) ) {
            SPRINT("(Row: " << row << ") Expected f(row) = " << float(row*0.79) << " but got: " << f[row]) 
            return 1;
        } else if ( i[row] != row*row) {
            SPRINT("(Row: " << row << ") Expected i(row) = " << row*row << " but got: " << i[row]) 
            return 1;
        } else if ( l[row] != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected l(row) = " << row*row*row << " but got: " << l[row]) 
            return 1;
        }

        // All good, now modify the value
        d[row] = 0*row;
        f[row] = 1*row;
        i[row] = 2*row;
        l[row] = 3*row;

        if ( d[row] != row*double(0) ) {
            SPRINT("(Row: " << row << ") Expected d(row) = " << double(row*0) << " but got: " << d[row]) 
            return 1;
        } else if ( f[row] != row*float(1) ) {
            SPRINT("(Row: " << row << ") Expected f(row) = " << float(row*1) << " but got: " << f[row]) 
            return 1;
        } else if ( i[row] != 2*row) {
            SPRINT("(Row: " << row << ") Expected i(row) = " << 2*row << " but got: " << i[row]) 
            return 1;
        } else if ( l[row] != 3*row ) {
            SPRINT("(Row: " << row << ") Expected l(row) = " << 3*row << " but got: " << l[row]) 
            return 1;
        }

        // OK, now reset back to the correct values
        file.GetCell<0>(row) = double(0.13)*row;
        file.GetCell<1>(row) = float(0.79)*row;
        file.GetCell<2>(row) = row*row;
        file.GetCell<3>(row) = row*row*row;

    }
    return 0;
}

int ValidateCSVColumns_Raw(DataFile& file) {
    double* d = file.GetColumn<0>().array();
    float* f = file.GetColumn<1>().array();
    int*   i = file.GetColumn<2>().array();
    long*  l = file.GetColumn<3>().array();
    for (int row=0; row<rows; row++) {
        if ( d[row] != row*double(0.13) ) {
            SPRINT("(Row: " << row << ") Expected d[row] = " << double(row*0.13) << " but got: " << d[row]) 
            return 1;
        } else if ( f[row] != row*float(0.79) ) {
            SPRINT("(Row: " << row << ") Expected f[row] = " << float(row*0.79) << " but got: " << f[row]) 
            return 1;
        } else if ( i[row] != row*row) {
            SPRINT("(Row: " << row << ") Expected i[row] = " << row*row << " but got: " << i[row]) 
            return 1;
        } else if ( l[row] != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected l[row] = " << row*row*row << " but got: " << l[row]) 
            return 1;
        }

        // Can't change values
    }
    return 0;
}

int ValidateCSVRows_Rows(DataFile& file) {
    for (int row=0; row<rows; row++) {
        auto&& r = file.GetRow(row);
        if ( std::get<0>(r) != row*double(0.13) ) {
            SPRINT("(Row: " << row << ") Expected d = " << double(row*0.13) << " but got: " << std::get<0>(r)) 
            return 1;
        } else if ( std::get<1>(r) != row*float(0.79) ) {
            SPRINT("(Row: " << row << ") Expected f = " << float(row*0.79) << " but got: " << std::get<1>(r)) 
            return 1;
        } else if ( std::get<2>(r) != row*row) {
            SPRINT("(Row: " << row << ") Expected i = " << row*row << " but got: " << std::get<2>(r)) 
            return 1;
        } else if ( std::get<3>(r) != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected l = " << row*row*row << " but got: " << std::get<3>(r)) 
            return 1;
        }

        std::get<0>(r) = 0*row;
        std::get<1>(r) = 1*row;
        std::get<2>(r) = 2*row;
        std::get<3>(r) = 3*row;

        if ( std::get<0>(r) != row*double(0) ) {
            SPRINT("(Row: " << row << ") Expected d = " << double(row*0) << " but got: " << std::get<0>(r)) 
            return 1;
        } else if ( std::get<1>(r) != row*float(1) ) {
            SPRINT("(Row: " << row << ") Expected f = " << float(row*1) << " but got: " << std::get<1>(r)) 
            return 1;
        } else if ( std::get<2>(r) != 2*row) {
            SPRINT("(Row: " << row << ") Expected i = " << row*row << " but got: " << std::get<2>(r)) 
            return 1;
        } else if ( std::get<3>(r) != 3*row ) {
            SPRINT("(Row: " << row << ") Expected l = " << row*row*row << " but got: " << std::get<3>(r)) 
            return 1;
        }

        // OK, now reset back to the correct values
        file.GetCell<0>(row) = double(0.13)*row;
        file.GetCell<1>(row) = float(0.79)*row;
        file.GetCell<2>(row) = row*row;
        file.GetCell<3>(row) = row*row*row;
    }
    return 0;
}

int ValidateCSVRows_Vars(DataFile& file) {
    for (int row=0; row<rows; row++) {
        int i;
        long l;
        float f;
        double d;
        file.GetRow(row,d,f,i,l);
        if ( d != row*double(0.13) ) {
            SPRINT("(Row: " << row << ") Expected d = " << row*0.13 << " but got: " << d) 
            return 1;
        } else if ( f != row*float(0.79) ) {
            SPRINT("(Row: " << row << ") Expected f = " << row*0.79 << " but got: " << f) 
            return 1;
        } else if ( i != row*row) {
            SPRINT("(Row: " << row << ") Expected i = " << row*row << " but got: " << i) 
            return 1;
        } else if ( l != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected l = " << row*row*row << " but got: " << l) 
            return 1;
        }
        // Can't change value
    }
    return 0;
}

int ValidateCSV(DataFile& file) {
    if ( ValidateCSVCells(file) != 0 ) {
        return 1;
    } else if ( ValidateCSVColumns_Columns(file) != 0 ) {
        return 2;
    } else if ( ValidateCSVColumns_Raw(file) != 0 ) {
        return 2;
    } else if ( ValidateCSVRows_Rows(file) != 0 ) {
        return 3;
    } else if ( ValidateCSVRows_Vars(file) != 0 ) {
        return 4;
    } else if ( file.Rows() != rows) {
        return 4;
    } else {
        return 0;
    }
}

int AddRows(testLogger& log) {
    DataFile csv;
    for (int i=0; i<rows; i++) {
        csv.AddRow(i*double(0.13),i*float(0.79),i*i,i*i*i);
    }
    return ValidateCSV(csv);
}

int AddRowsWithReserve(testLogger& log) {
    DataFile csv;
    csv.Reserve(rows);
    for (int i=0; i<rows; i++) {
        csv.AddRow(i*double(0.13),i*float(0.79),i*i,i*i*i);
    }
    return ValidateCSV(csv);
}

int CopyConstruct(testLogger& log) {
    DataFile csv_moved = GetCSV();
    DataFile csv(csv_moved);
    return ValidateCSV(csv);
}

int MoveConstruct(testLogger& log) {
    DataFile csv(GetCSV());
    ValidateCSV(csv);
    return ValidateCSV(csv);
}

int CopyAssignment(testLogger& log) {
    DataFile t = GetCSV();
    DataFile csv;
    csv = t;
    return ValidateCSV(csv);
}

int MoveAssignment(testLogger& log) {
    DataFile csv;
    csv = GetCSV();
    return ValidateCSV(csv);
}

int CopyRows(testLogger& log ) {
    DataFile source(GetCSV());
    DataFile csv;
    for (int i=0; i<rows; i++) {
        csv.AddRow(source.GetRow(i));
    }
    return ValidateCSV(csv);
}

int RemoveRows(testLogger& log ) {
    DataFile csv;
    for (int i=0; i<rows; i++) {
        csv.AddRow(i*double(0.13),i*float(0.79),i*i,i*i*i);
        csv.AddRow(i*double(0),i*float(0),i*i*0,i*i*i*0);
    }
    for (int i=0; i<rows; i++) {
        csv.RemoveRow(i+1);
    }
    return ValidateCSV(csv);
}

int WriteFile(testLogger& log ) {
    CSV<int,long,float,double,string> data;
    DataLump<1024> file;
    data.AddRow(0,1,2.1234567089,3.00000001,"\"four\" - \n      a magic number for some");
    data.AddRow(5,6,0.007,800009,"This is just a boring string with some data");
    data.WriteCSV(file);

    string expectedData = "0,1,2.1234567e+00,3.000000010000000e+00,\"\\\"four\\\" - \\n      a magic number for some\"\n5,6,7.0000002e-03,8.000090000000000e+05,\"This is just a boring string with some data\"\n";

    for ( int i=0; i<static_cast<int>(expectedData.length()); i++ ) {
        if ( expectedData[i] != file.Get(i) ) {
            SPRINT ( "Missmatch in file, at index " << i )
            SPRINT ( "Expected: " << expectedData[i] )
            SPRINT ( "Got: " <<  file.Get(i) )
            SPRINT ( "Expected data: " )
            PRINT  (expectedData)
            PRINT  ( BinaryDescribe::Describe(file, 1024) )
            return 1;
        }
    }
    return 0;
}

int ReadFile (testLogger& log ) {
    OFStreamWriter f("test.data");
    GetCSV().WriteCSV(f);
    f.close();
    IFStreamReader in_f("test.data");
    DataFile csv(DataFile::LoadCSV(in_f));
    // Allow room for floating point errors
    for (int row=0; row<rows; row++) {
        if ( fabs(csv.GetCell<0>(row) - row*double(0.13)) > 0.000000000001 ) {
            SPRINT("(Row: " << row << ") Expected csv<d>(row) = " << double(row*0.13) << " but got: " << csv.GetCell<0>(row)) 
            return 1;
        } else if ( fabs(csv.GetCell<1>(row) - row*float(0.79) ) > 0.00001 ) {
            SPRINT("(Row: " << setprecision(15) << row << ") Expected csv<f>(row) = " << float(row*0.79) << " but got: " << csv.GetCell<1>(row)) 
            return 1;
        } else if ( csv.GetCell<2>(row) != row*row) {
            SPRINT("(Row: " << row << ") Expected csv<i>(row) = " << row*row << " but got: " << csv.GetCell<2>(row)) 
            return 1;
        } else if ( csv.GetCell<3>(row) != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected csv<l>(row) = " << row*row*row << " but got: " << csv.GetCell<3>(row)) 
            return 1;
        }
    }
    return 0;
}
int FastReadFile (testLogger& log ) {
    OFStreamWriter f("test.data");
    GetCSV().WriteCSV(f);
    f.close();
    IFStreamReader in_f("test.data");
    DataFile csv(DataFile::FastLoadCSV(in_f,','));
    // Allow room for floating point errors
      for (int row=0; row<rows; row++) {
        if ( fabs(csv.GetCell<0>(row) - row*double(0.13)) > 0.000000000001 ) {
            SPRINT("(Row: " << row << ") Expected csv<d>(row) = " << double(row*0.13) << " but got: " << csv.GetCell<0>(row)) 
            return 1;
        } else if ( fabs(csv.GetCell<1>(row) - row*float(0.79) ) > 0.00001 ) {
            SPRINT("(Row: " << setprecision(15) << row << ") Expected csv<f>(row) = " << float(row*0.79) << " but got: " << csv.GetCell<1>(row)) 
            return 1;
        } else if ( csv.GetCell<2>(row) != row*row) {
            SPRINT("(Row: " << row << ") Expected csv<i>(row) = " << row*row << " but got: " << csv.GetCell<2>(row)) 
            return 1;
        } else if ( csv.GetCell<3>(row) != row*row*row ) {
            SPRINT("(Row: " << row << ") Expected csv<l>(row) = " << row*row*row << " but got: " << csv.GetCell<3>(row)) 
            return 1;
        }
    }
    return 0;
}
