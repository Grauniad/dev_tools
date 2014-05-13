#ifndef DEV_TOOLS_CSV_H
#define DEV_TOOLS_CSV_H

#include<boost/tokenizer.hpp>
#include <string>
#include <sstream>
#include <cstring>
#include <array>
#include <vector>
#include "binaryReader.h"
#include "binaryWriter.h"
#include <type_traits>
#include <iomanip>

#define TYPE(i) CSV<Types...>::ColTypes::type<i>
#define THIS CSV<Types...>

typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokeniser;

//forward declare
class CSV_Column_Base;
class CSV_Cell;
template<class T>
class CSV_CellInstance;
template<class T>
class CSV_Column;


// TODO: Write file
template<class...Types>
class CSV {
private:
    using colsType = std::tuple<CSV_Column<Types>...>;
public:
    // Type Info
    using rowType = std::tuple<Types&...>;
    using forwardRowType = std::tuple<Types&&...>;
    struct ColTypes {
        template<std::size_t N>
        using type = typename std::tuple_element<N, colsType>::type::DataType;
        template<std::size_t N>
        using colType = typename std::tuple_element<N, colsType>::type;
    };
    static const int ncols = sizeof...(Types);
    int Rows();

    // Empty file
    CSV(){}

    // Move constructor
    CSV(CSV&& src) {
        std::swap(this->columns, src.columns);
    }
    CSV& operator=(CSV&& src) {
        std::swap(this->columns, src.columns);
        return *this;
    }

    // Copy constructor
    CSV(const CSV& rhs): columns(rhs.columns){}
    CSV& operator=(CSV& rhs) {
        this->columns = rhs.columns;
        return *this;
    }

    // Get a Row
    rowType GetRow(int i);
    void GetRow(int i, Types&...args);

    void Reserve(int rows);

    // Remove a Row
    void RemoveRow(int i);

    // Add a Row
    void AddRow(Types&&...args);
    void AddRow(rowType&& row);
    //TODO: Add row at
    //TODO: Replace row at

    // Print a Row
    std::string PrintRow(int i);

    // Get a Column
    template <int i> 
    inline ColTypes::colType<i>&
    GetColumn() { 
        return std::get<i>(columns);
    }

    // Get a cell
    template<int col>
    inline TYPE(col)& GetCell(int row) {
        return std::get<col>(columns)[row];
    }

    /*
     * Read the CSV in a line at a time, and get the boost iterator
     * to properlly parse it for escaped quotes / commas etc...
     */
    static CSV<Types...> LoadCSV(BinaryReader reader);

    /*
     * Read in each line, and just search for the delim...
     *
     * The input file is assumed to be perfect, Loo the the caller who gives it 
     * bad date...
     */
    static CSV<Types...> FastLoadCSV(BinaryReader reader, char delim);

    void WriteCSV(BinaryWriter writer);

private:

    // Utility Functions
    void NewRow(const Tokeniser& tok);

    // Data
    colsType columns;

    // Intenal Template Utilities

    //*********************************
    // template to add from tokens
    //*********************************
    template<int index, class IT>
    inline typename std::enable_if< index!=0,void>::type
    AddCell(IT& it);

    template<int index, class IT>
    inline typename std::enable_if< index==0,void>::type
    AddCell(IT& it);
    //*********************************
      
    //*********************************
    // template to add from values
    //*********************************
    template<int index>
    inline typename std::enable_if< index!=0,void>::type
    AddCell(forwardRowType&& row);

    template<int index>
    inline typename std::enable_if< index==0,void>::type
    AddCell(forwardRowType&& row);
    //*********************************
      
    //*********************************
    // template to add from Row
    //*********************************
    template<int index>
    inline typename std::enable_if< index!=0,void>::type
    AddCell(rowType&& row);

    template<int index>
    inline typename std::enable_if< index==0,void>::type
    AddCell(rowType&& row);
    //*********************************

    //*********************************
    // template to get a row
    //*********************************

    // Used to by-pass the "must initialise reference" rule
    // WARNING - This is inherently dangerous, use with EXTREME CAUTION
    template<class T>
    class pT {
        public:
        inline operator T&() {  return *i; }
        inline pT& operator=(T& rhs) { 
            i = &rhs; 
            return *this;  
        }
        T *i;
    };

    template< int index>
    inline typename std::enable_if< index!=0,void>::type
    GetCell(std::tuple<pT<Types>...>& t, int row);

    template< int index>
    inline typename std::enable_if< index==0,void>::type
    GetCell(std::tuple<pT<Types>...>& t, int row);
    //*********************************
      
    //*********************************
    // template to print a row
    //*********************************
    template<int index>
    inline typename std::enable_if< index!=0,void>::type
    PrintCell(std::stringstream& s, int row);

    template<int index>
    inline typename std::enable_if< index==0,void>::type
    PrintCell(std::stringstream& s, int row);
    //*********************************
      
    //*********************************
    // template to remove a row
    //*********************************
    template< int index>
    inline typename std::enable_if< index!=0,void>::type
    RemoveCell(int row);

    template< int index>
    inline typename std::enable_if< index==0,void>::type
    RemoveCell(int row);
    //*********************************
      
    //*********************************
    // template to reserve space
    //*********************************
    template< int index>
    inline typename std::enable_if< index!=0,void>::type
    ReserveColumn(int rows);

    template< int index>
    inline typename std::enable_if< index==0,void>::type
    ReserveColumn(int rows);

    //*********************************
};

template<class T>
class CSV_Column_Common {
public:
    typedef T DataType;
    // CopyConstruct
    CSV_Column_Common() = default;
    CSV_Column_Common(const CSV_Column_Common& rhs): data(rhs.data){}

    // Raw data acccess
    T* array() { return data.data(); }

    // Element access
    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }

    // Remove an element
    void remove(int i) {
        data.erase(data.begin() + i);
    }

    // Insert an element
    void emplace_back(T&& v) {
        data.emplace_back(v);
    }
    void emplace_back(const T& v) {
        data.emplace_back(v);
    }

    int size() {
        return data.size();
    }

    void reserve(int rows) {
        data.reserve(rows);
    }

    //
    // Enable itereation / range based for loops
    //
    inline typename std::vector<T>::iterator begin() { 
        return data.begin();
    }
    inline typename std::vector<T>::iterator end() { 
        return data.end();
    }
    inline typename std::vector<T>::const_iterator begin() const { 
        return data.cbegin();
    }
    inline typename std::vector<T>::const_iterator end() const { 
        return data.cend();
    }
protected:
    std::vector<T>  data;
};


template<class T>
class CSV_Column: public CSV_Column_Common<T> {
public:
    inline void NewRow(const string& token) {
        this->data.emplace_back(token);
    }

    inline void Print(std::stringstream& str, int row) {
        str << this->data[row];
    }
};

template<>
class CSV_Column<string>: public CSV_Column_Common<string> {
public:
    inline void NewRow(const string& token) {
        this->data.emplace_back(token);
    }

    inline void Print(std::stringstream& str, int row) {
        str << "\"";
        for(int i=0; i<static_cast<int>(data[row].length()); i++ ) {
            const char& c = data[row][i];
            if ( c == '"' ) {
                str << '\\' << '"';
            } else if ( c == '\n') {
                str << '\\' << 'n';
            } else {
                str << c;
            }
        }
        str << "\"";
    }
};

template<>
class CSV_Column<double>: public CSV_Column_Common<double> {
public:
    inline void NewRow(const string& token) {
        double d;
        istringstream(token) >> d;
        data.emplace_back(std::move(d));
    }

    inline void Print(std::stringstream& str, int row) {
        str << std::scientific << setprecision(15) << data[row];
    }
};

template<>
class CSV_Column<float>: public CSV_Column_Common<float> {
public:
    inline void NewRow(const string& token) {
        float f;
        istringstream(token) >> f;
        data.emplace_back(std::move(f));
    }

    inline void Print(std::stringstream& str, int row) {
        str << std::scientific << setprecision(7) << data[row];
    }
};

template<>
class CSV_Column<int>: public CSV_Column_Common<int> {
public:
    inline void NewRow(const string& token) {
        data.emplace_back(atoi(token.c_str()));
    }

    inline void Print(std::stringstream& str, int row) {
        str << data[row];
    }
};

template<>
class CSV_Column<long>: public CSV_Column_Common<long> {
public:
    void NewRow(const string& token) {
        data.emplace_back(atol(token.c_str()));
    }

    inline void Print(std::stringstream& str, int row) {
        str << data[row];
    }
};


#include "csv.hpp"

#endif
