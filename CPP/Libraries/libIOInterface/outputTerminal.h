#ifndef __DEV_TOOLS_LIBRARIES_IO_INTERFACE_OUTPUT_TERMINAL_H__
#define __DEV_TOOLS_LIBRARIES_IO_INTERFACE_OUTPUT_TERMINAL_H__


#include <string>
#include <vector>

/*
 * An interface for working with a terminal like 
 * output
 */
class OutputTerminal{
public:
    virtual void PutString(const std::string& line) = 0;
};

/*
 * An inplementation of the OutputTerminal which can be used 
 * to validate objects
 */
class TestTerminal: public OutputTerminal {
public:
    virtual void PutString(const std::string& output);

    /*
     * Expose the output buffer for validation
     */
    std::vector<std::string>& Output();

    std::string OutputAsString();

    void Reset();
private:
    std::vector<std::string> lines;

};

#endif
