#ifndef CEFTESTAPP_TESTS_H_
#define CEFTESTAPP_TESTS_H_

#include <tester.h>

class CefTestBase {
public:
    /**
     * Will be invoked on the renderer thread when the test should be started.
     *
     * If the test needs to run in the browser, a call to DoOnUI should be made
     */
    virtual int RunTest(testLogger& log) = 0;

    virtual const std::string& Name() const = 0;

    virtual ~CefTestBase() { } 
private:
};

class SimpleCefTest: public CefTestBase {
public:
    typedef int (TestFun) (testLogger& log);
    SimpleCefTest(const std::string& testName, TestFun* fptr)
       : name(testName), func(fptr) { }

    virtual int RunTest(testLogger& log) {
       return (*func)(log);
    }

    virtual const std::string& Name() const {
        return name;
    }

private:
    std::string name;
    TestFun* func;
};

#endif
