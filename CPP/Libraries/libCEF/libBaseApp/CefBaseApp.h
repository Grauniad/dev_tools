/*
 * CefBaseApp.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEAPP_H_
#define CEFBASEAPP_H_

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "CefBaseBrowserProcess.h"
#include "CefBaseRendererProcess.h"
#include "CefBaseClient.h"

/**
 * @title CEF Application
 *
 * The base-application in the single access point created by all processes.
 *
 * Application wishing to hook into CEF events should install handlers before
 * calling RunAndExit().
 */
class CefBaseApp: public CefApp {
public:
    CefBaseApp();
    virtual ~CefBaseApp();


    /**********************************************************************
     *                         CEF APP
     **********************************************************************/
    /**
     * PROCESS: Browser
     * THREAD:  ANY
     *
     * Return a reference to the object responsible for handling browser level
     * events
     */
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
            OVERRIDE;

    /**
     * PROCESS: Renderer
     * THREAD:  ANY
     *
     * Return a reference to the object responsible for handling browser level
     * events
     */
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
            OVERRIDE;

    CefRefPtr<CefClient> GetClient() {
        return client_;
    }

    /**
     * Run the application, and return a exit code.
     *
     * The exit code returned by the function should be used immediately to
     * return from the program's main loop
     *
     */
    int Main(int argc, char *argv[]);

    /**********************************************************************
     *                         Accessors
     **********************************************************************/

    CefBaseRendererProcess& Renderer();
    CefBaseBrowserProcess& Browser();

    CefBaseClient& Client();

private:
    CefRefPtr<CefBrowserProcessHandler> browser_;
    CefRefPtr<CefRenderProcessHandler> renderer_;
    CefRefPtr<CefClient>               client_;
    IMPLEMENT_REFCOUNTING(CefBaseApp);
};

#endif /* CEFBASEAPP_H_ */
