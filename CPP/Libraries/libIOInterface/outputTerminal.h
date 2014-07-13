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

    /*
     * Get a line from the user...
     *
     *   prompt: Text to display to the user before getting the input...
     *   storePrompt: If this is set to true, prompt is added to the output buffer,
     *                oterwise it will be cleared away when next time the screen is
     *                refreshed...
     */
    virtual std::string GetLine(const std::string& prompt, 
                                bool storePrompt = true) = 0;

    /*
     * Clear down the screen
     */
    virtual void Clear() = 0;
};

/*
 * An inplementation of the OutputTerminal which can be used 
 * to validate objects
 */
class TestTerminal: public OutputTerminal {
public:
    virtual void PutString(const std::string& output);
    virtual std::string GetLine(const std::string& prompt, bool storePrompt = true) {
        return "";
    }
    virtual void Clear () { }

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
