#ifndef DEV_TOOLS_CPP_LIBRARIES_UTILS_CSV_CSV_DYNAMIC_H
#define DEV_TOOLS_CPP_LIBRARIES_UTILS_CSV_CSV_DYNAMIC_H

#include "binaryReader.h"
#include "binaryWriter.h"

#include <vector>
#include<boost/tokenizer.hpp>

typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokeniser;

template<class T>
class CSV_rows {
    std::vector< vector<T> > rows;
public:
    CSV_rows(CSV_rows&& source) = default;

    static CSV_rows<T> LoadCSV(BinaryReader reader);
    static CSV_rows<T> MakeEmpty();

    std::vector<T>& GetRow(size_t r) {
        return rows[r];
    }
    size_t Rows() { return rows.size(); }

    template <class ...Args>
    void AddRow(Args&&... items);

    void AddRow(std::vector<T>&& row);

    decltype(rows.begin()) begin() {
        return rows.begin();
    }

    decltype(rows.end()) end() {
        return rows.end();
    }
private:
    CSV_rows() = default;
    void NewRow(const Tokeniser& tok);
};

#include "csvDynamic.hpp"

#endif
