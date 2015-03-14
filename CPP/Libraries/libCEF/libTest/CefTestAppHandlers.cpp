#include "CefTestAppHandlers.h"
#include "CefTestClient.h"
#include "CefTestApp.h"

#include "include/wrapper/cef_helpers.h"
#include "include/cef_task.h"
#include <env.h>
#include <iostream>

bool DummyCefAppHandlers::exitClean = true;

DummyCefAppHandlers::DummyCefAppHandlers()
{
}

void DummyCefAppHandlers::OnContextInitialized() {
    // This should be called from the browser process..
    CEF_REQUIRE_UI_THREAD()
    ;

    // Information used when creating the native window.
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0,false);

    // SimpleHandler implements browser-level callbacks.
    std::shared_ptr<DummyCefClient> handler(new DummyCefClient);
    std::shared_ptr<CefClient> client = handler;
    std::shared_ptr<CefLifeSpanHandler> lifeHandler = handler;
    std::shared_ptr<CefRenderHandler> renderHandler = handler;

    DummyCefApp::Instance().Client().InstallMessagerHandler(client);
    DummyCefApp::Instance().Client().LifeSpanHandler().InstallHandler(
        lifeHandler);
    DummyCefApp::Instance().Client().RenderHandler().InstallHandler(
        renderHandler);


    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    std::string root = ENV::GetEnvString("PROJECT_ROOT_DIR");

    std::string url = "file://" + root + "/DEV_TOOLS/CPP/Tests/CEF/test.html";

    // Create the first browser window.
    CefBrowserHost::CreateBrowser(
        window_info,
        DummyCefApp::Instance().GetClient().get(),
        url,
        browser_settings,
        NULL);
}

void DummyCefAppHandlers::Exit() {
    if (CefCurrentlyOn(TID_UI)) {
        exitClean = true;
        CefQuitMessageLoop();
    } else {
    }
}

void DummyCefAppHandlers::Abort() {
    if (CefCurrentlyOn(TID_UI)) {
        exitClean = false;
        CefQuitMessageLoop();
    } else {
    }
}

void DummyCefAppHandlers::AddTest(std::unique_ptr<CefTestBase>&& test) {
    tests.emplace_back(std::move(test));
}

void DummyCefAppHandlers::OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    // JS initialised - run tests
    cout << "*****************************************************" << endl;
    try
    {
        for (auto& test : tests ) {
            Test(test->Name(),[&test] (testLogger& log) -> int {
                return test->RunTest(log);
            }).RunTest(Test::PRINT_LOG_AND_THROW);
        }

        // When done - kill the process...
        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("EXIT");
        browser->SendProcessMessage(PID_BROWSER,msg);
    }
    catch (Test::TestFailureException& fail)
    {
        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("ABORT");
        browser->SendProcessMessage(PID_BROWSER,msg);
    }
    cout << "*****************************************************" << endl;
}

DummyCefAppHandlers::~DummyCefAppHandlers() {

}

