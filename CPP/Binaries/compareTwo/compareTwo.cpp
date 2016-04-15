#include <csv.h>
#include <map>
#include <stdReader.h>
#include <iostream>
#include <iomanip>

using namespace std;

class InputFile{
public:
    InputFile(const char* fname)
    {
        IFStreamReader file(fname);
        CSV<std::string,long> data = CSV<std::string,long>::LoadCSV(file);
        run = fname;

        for (int i = 0; i <data.Rows(); ++i) {
            std::string name;
            long result;
            data.GetRow(i,name,result);
            TestMap::value_type pair(name,result);
            results.insert(pair);
        }
    }

    const std::string Name() const {
        return run;
    }

    typedef std::map<std::string,long> TestMap;

    const TestMap& Results() {
        return results;
    }
private:
    TestMap        results;
    std::string    run;
};

void LineBreak(size_t n) {
    cout << "|-";
    cout << setfill('-');
    cout << setw(50) << "";
    for (size_t i = 0; i< n; ++i) {
        cout << "-|-";
        cout << setw(25) << "";
    }
    cout << "-|";
    cout << endl;
    cout << setfill(' ');
}

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        cout << "Usage: compare reference.csv [result1.csv] [results2.csv] ..." << endl;
        return 1;
    }
    InputFile reference(argv[1]);

    std::map<std::string,InputFile> results;
    for (int i = 2; i < argc; ++i) {
        auto it = results.emplace(argv[i],argv[i]).first;
        cout << "Loaded: " << it->first << endl;
    }

    /**
     *  Header
     */
    cout << "| ";
    cout << setw(50) << left << "Test name";
    cout << " | ";
    cout << setw(25) << reference.Name();
    for (auto& pair: results) {
        cout << " | ";
        cout << setw(25) << pair.second.Name();
    }
    cout << " |";
    cout << endl;
    LineBreak(results.size()+1);

    /**
     *  Results
     */
    for (auto& pair: reference.Results()) {
        cout << "| ";
        long baseline = pair.second;
        const std::string& name = pair.first;
        cout << setw(50) << left << name;
        cout << " | ";
        cout << setw(25) << baseline;
        for (auto& pair: results) {
            auto& resultMap = pair.second.Results();
            auto it = resultMap.find(name);
            cout << " | ";
            if ( it != resultMap.end()) {
                long result = it->second;
                long diff = result - baseline;
                double pctDiff = (100.0 * diff) / baseline;
                std::stringstream buf;
                if ( pctDiff > 0 ) {
                    buf << setw(10) << result << " (+" << setprecision(2) << pctDiff<< "%)";
                } else {
                    buf << setw(10) << result << " (" << setprecision(2) << pctDiff<< "%)";
                }
                cout << setw(25) << buf.str();
            } else {
                cout << setw(25) << "";
            }
        }
        cout << " |";
        cout << endl;
    }
    return 0;
}
