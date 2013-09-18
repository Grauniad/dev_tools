#include "csv.h"
#include "dataVector.h"
#include "logger.h"
#include "binaryDescribe.h"


template<class...Types>
int CSV<Types...>::Rows() {
    return std::get<0>(this->columns).size();
}

template<class...Types>
void CSV<Types...>::WriteCSV(BinaryWriter writer) {
    for ( int i = 0; i< Rows(); i++) {
        string && l = PrintRow(i);
        writer.Write(l.c_str(),l.length());
        writer+=l.length();
        writer.Fill(1,'\n');
        writer+=1;
    }
}

template<class...Types>
CSV<Types...> CSV<Types...>::LoadCSV(BinaryReader reader) {
    CSV<Types...> csv;
    SLOG_FROM ( LOG_VERBOSE, "CSV::LoadCSV", "Reading a new CSV file" << endl << BinaryDescribe::Describe(reader, reader.Size()))

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
        SLOG_FROM ( LOG_VERBOSE, "CSV::LoadCSV", "Read a new line from the CSV: (size: " << next - reader << ")" << endl << BinaryDescribe::Describe(r, 1024))
        // Tokenize
        string s = r.ReadString();
        csv.NewRow(Tokeniser(s));
        reader = next.Offset()+1;
    }
    return std::move(csv);
}

//*********************************
// template to add from tokens
//*********************************
template<class...Types>
void CSV<Types...>::NewRow(const Tokeniser& tok) {
    Tokeniser::iterator it = tok.begin();
    AddCell<ncols-1>(it);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::AddCell(Tokeniser::iterator& it) {
    std::get<ncols-1-index>(this->columns).NewRow(*it);
    it++;
    this->AddCell<index -1>(it);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::AddCell(Tokeniser::iterator& it) {
    std::get<ncols-1>(this->columns).NewRow(*it);
}
//*********************************

//*********************************
// template to add from values
//*********************************
template<class...Types>
void CSV<Types...>::AddRow(Types&&...args) { 
    this->AddCell<ncols-1>(forward_as_tuple(std::move(args)...));
}

template<class...Types>
template<int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::AddCell(forwardRowType&& row) {
    std::get<index>(this->columns).emplace_back(std::move(std::get<index>(row)));
    this->AddCell<index-1>(std::move(row));
}

template<class...Types>
template<int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::AddCell(forwardRowType&& row) {
    std::get<index>(this->columns).emplace_back(std::move(std::get<index>(row)));
}
//*********************************
  
//*********************************
// template to add from row
//*********************************
template<class...Types>
void CSV<Types...>::AddRow(rowType&& row) { 
    this->AddCell<ncols-1>(std::move(row));
}

template<class...Types>
template<int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::AddCell(rowType&& row) {
    std::get<index>(this->columns).emplace_back(std::get<index>(row));
    this->AddCell<index-1>(std::move(row));
}

template<class...Types>
template<int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::AddCell(rowType&& row) {
    std::get<index>(this->columns).emplace_back(std::get<index>(row));
}
//*********************************

//*********************************
// template to get a row
//*********************************
template<class...Types>
void CSV<Types...>::GetRow(int i, Types&...args) {
    tie(args...) = std::move(GetRow(i));
}

template<class...Types>
typename CSV<Types...>::rowType
CSV<Types...>::GetRow(int i) {
    std::tuple<pT<Types>...> t;
    this->GetCell<ncols-1>(t,i);
    return std::move(t);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
THIS::GetCell(std::tuple<pT<Types>...>& t, int row) {
    std::get<index>(t) = std::get<index>(this->columns)[row];
    GetCell<index-1>(t,row);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
THIS::GetCell(std::tuple<pT<Types>...>& t, int row) {
    std::get<index>(t) = std::get<index>(this->columns)[row];
}
//*********************************
  
//*********************************
// template to print a row
//*********************************
template<class...Types>
std::string CSV<Types...>::PrintRow(int i) {
    std::stringstream s;
    this->PrintCell<ncols-1>(s,i);
    return s.str();
}

template<class...Types>
template<int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::PrintCell(std::stringstream& s, int row) {
    std::get<ncols-1-index>(this->columns).Print(s,row);
    s << ",";
    this->PrintCell<index-1>(s,row);
}

template<class...Types>
template<int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::PrintCell(std::stringstream& s, int row) {
    std::get<ncols-1>(this->columns).Print(s,row);
}
//*********************************


//*********************************
// template to remove a row
//*********************************
template<class...Types>
void CSV<Types...>::RemoveRow(int i) {
    this->RemoveCell<ncols-1>(i);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::RemoveCell(int row) {
    std::get<index>(this->columns).remove(row);
    this->RemoveCell<index -1>(row);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::RemoveCell(int row) {
    std::get<index>(this->columns).remove(row);
}
//*********************************
  
//*********************************
// template to reserve for more rows
//*********************************
template<class...Types>
void CSV<Types...>::Reserve(int rows) {
    this->ReserveColumn<ncols-1>(rows);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index!=0,void>::type
CSV<Types...>::ReserveColumn(int rows) {
    std::get<index>(this->columns).reserve(rows);
    this->ReserveColumn<index -1>(rows);
}

template<class...Types>
template< int index>
inline typename std::enable_if< index==0,void>::type
CSV<Types...>::ReserveColumn(int rows) {
    std::get<index>(this->columns).reserve(rows);
}
//*********************************

