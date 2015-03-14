/*
 * jsObject.cpp
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */
#include <CEFJSObject.h>
#include <env.h>
#include <iostream>

#include <CefTestApp.h>

int EmptyObject(testLogger& log);

int main(int argc, char **argv) {

    DummyCefApp::Instance().AddTest(
        "Initialising an empty object",
        EmptyObject);

    DummyCefApp::Instance().RunTestsAndExit(argc, argv);

    cout << "ERROR: Run and exit should have exited!: " << endl;
    return 1;
}

int EmptyObject(testLogger& log) {
    log << "Hello World!" << endl;
    CefString str = "Hello";
    CEFJSObject object;
    object.Add("Hello", "World");
    log << "VALUE: " << object.JSON() << endl;
    return 0;
}
