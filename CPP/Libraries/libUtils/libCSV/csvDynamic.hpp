#include <iterator>
#include "csvDynamic.h"
#include <cstdlib>
#include "dataVector.h"
#include <sstream>

template<class T>
CSV_rows<T> CSV_rows<T>::LoadCSV(BinaryReader reader) {
    CSV_rows csv;
    DataVector buf(1024);
    BinaryWriter w(buf);
    BinaryReader r(buf);
    while ( reader.Offset() < reader.End() ) {
        buf.Clear();
        BinaryReader next = reader.Find('\n');
        // Extract the line
        reader.Read(w,next-reader+1);
        // Replace the '\n' with '\0'
        buf[next-reader] = '\0';
        // Tokenize
        string s = r.ReadString();
        csv.NewRow(Tokeniser(s));
        reader = next.Offset()+1;
    }
    return std::move(csv);
}

template<class T>
inline CSV_rows<T> CSV_rows<T>::MakeEmpty() {
    return CSV_rows<T>();
}

template<class T>
template<class... Args>
void CSV_rows<T>::AddRow(Args&&... items) {
    rows.emplace_back(std::vector<T>({items...}));
}

template<class T>
inline void CSV_rows<T>::AddRow(std::vector<T>&& row) {
    rows.emplace_back(row);
}

template<class T>
void CSV_rows<T>::NewRow(const Tokeniser& tok) {
    vector<T> row;
    for ( Tokeniser::iterator it = tok.begin();
          it != tok.end();
          it++)
    {
        T val;
        istringstream(*it) >> val;
        row.push_back(std::move(val));
    }
    rows.push_back(row);
}
