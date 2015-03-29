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
#include <CefTests.h>

int InvalidCallback(CefTestContext& context);
int Ping(CefTestContext& context);

int main(int argc, char **argv) {



    DummyCefApp::Instance().AddTest(
        "Initialising an empty object",
        InvalidCallback);

    DummyCefApp::Instance().AddTest(
        "Sending a ping request",
        Ping);

    DummyCefApp::Instance().RunTestsAndExit(argc, argv);

    cout << "ERROR: Run and exit should have exited!: " << endl;
    cout.sync_with_stdio();
    return 1;
}

int InvalidCallback(CefTestContext& context) {
    context.Log() << "Sending request..." << endl;
    context.ExecuteCleanJS(R"JSCODE(
        var result="RUNNING";

        function Success(response) {
            DEV_TOOLS_Test_Log(">>> Request handled successfully!");
            result = "SUCCESS";
        }

        function Failure(err_code, err_message) {
            DEV_TOOLS_Test_Log(
                ">>> Failed to handle query: " + err_message + "(" + err_code + ")");
            result = "FAIL"
        }

        DEV_TOOLS_Test_Log("Sending query request");

        window.testQuery({
            request: "__DEV_TOOLS_CEF_TESTS__THIS_QUERY_IS_NOT_DEFINED!___",
            onSuccess: Success,
            onFailure: Failure });

        result;
    )JSCODE" , "RUNNING");

    context.WaitForResult([&] () -> bool {
        context.ExecuteCleanJS(R"JSCODE(
            result;
        )JSCODE" , "FAIL");
        return true;
    },1e6, "Waiting for framework to notify of us of the fail.",0.5e6);
    return 0;
}

int Ping(CefTestContext& context) {
    context.Log() << "Sending request..." << endl;
    context.ExecuteCleanJS(R"JSCODE(
        var result="RUNNING";

        function Success(response) {
            DEV_TOOLS_Test_Log(">>> Request handled successfully!");
            result = response;
        }

        function Failure(err_code, err_message) {
            DEV_TOOLS_Test_Log(
                ">>> Failed to handle query: " + err_message + "(" + err_code + ")");
            result = "FAIL: " + err_message;
        }

        DEV_TOOLS_Test_Log("Sending ping request");

        window.testQuery({
            request: "REQUEST_DEV_TOOLS_Ping Pong",
            onSuccess: Success,
            onFailure: Failure });

        result;
    )JSCODE" , "RUNNING");

    context.WaitForResult([&] () -> bool {
        context.ExecuteCleanJS(R"JSCODE(
            result;
        )JSCODE" , "Pong");
        return true;
    },1e6, "Waiting for framework to respond to our ping request");

    context.ExecuteCleanJS(R"JSCODE(
        DEV_TOOLS_Test_Log("Sending ping request");
        result="RUNNING";

        window.testQuery({
            request: "REQUEST_DEV_TOOLS_Ping ",
            onSuccess: Success,
            onFailure: Failure });

        result;
    )JSCODE" , "RUNNING");

    context.WaitForResult([&] () -> bool {
        context.ExecuteCleanJS(R"JSCODE(
            result;
        )JSCODE" , "");
        return true;
    },1e6, "Waiting for framework to respond to our blank ping request");

    context.ExecuteCleanJS(R"JSCODE(
        DEV_TOOLS_Test_Log("Sending ping request");
        result="RUNNING";

        window.testQuery({
            request: "REQUEST_DEV_TOOLS_Ping",
            onSuccess: Success,
            onFailure: Failure });

        result;
    )JSCODE" , "RUNNING");

    context.WaitForResult([&] () -> bool {
        context.ExecuteCleanJS(R"JSCODE(
            result;
        )JSCODE" , "");
        return true;
    },1e6, "Waiting for framework to respond to our empty ping request");

    context.ExecuteCleanJS(R"JSCODE(
        DEV_TOOLS_Test_Log("Sending ping request");
        result="RUNNING";

        window.testQuery({
            request: "REQUEST_DEV_TOOLS_Ping this one has many words in it",
            onSuccess: Success,
            onFailure: Failure });

        result;
    )JSCODE" , "RUNNING");

    context.WaitForResult([&] () -> bool {
        context.ExecuteCleanJS(R"JSCODE(
            result;
        )JSCODE" , "this one has many words in it");
        return true;
    },1e6, "Waiting for framework to respond to our multi-word ping request");

    return 0;
}
