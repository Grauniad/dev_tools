/*
 * CefBaseApp.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseApp.h"

#include <X11/Xlib.h>


CefBaseApp::CefBaseApp()
    : browser_(new CefBaseBrowserProcess),
      renderer_(new CefBaseRendererProcess),
      client_(new CefBaseClient(this))
{
    // TODO Auto-generated constructor stub

}

CefBaseApp::~CefBaseApp() {
    // TODO Auto-generated destructor stub
}

CefRefPtr<CefBrowserProcessHandler> CefBaseApp::GetBrowserProcessHandler() {
    return browser_;
}

CefRefPtr<CefRenderProcessHandler> CefBaseApp::GetRenderProcessHandler() {
    return renderer_;
}

CefBaseRendererProcess& CefBaseApp::Renderer() {
    return static_cast<CefBaseRendererProcess&>(*renderer_.get());
}

CefBaseBrowserProcess& CefBaseApp::Browser() {
    return static_cast<CefBaseBrowserProcess&>(*browser_.get());
}

CefBaseClient& CefBaseApp::Client() {
    return static_cast<CefBaseClient&>(*client_.get());
}

namespace {
    int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
      LOG(WARNING)
            << "X error received: "
            << "type " << event->type << ", "
            << "serial " << event->serial << ", "
            << "error_code " << static_cast<int>(event->error_code) << ", "
            << "request_code " << static_cast<int>(event->request_code) << ", "
            << "minor_code " << static_cast<int>(event->minor_code);
      return 0;
    }

    int XIOErrorHandlerImpl(Display *display) {
      return 0;
    }
}

int CefBaseApp::Main(int argc, char* argv[]) {
    CefRefPtr<CefBaseApp> THIS(this);
        // Provide CEF with command-line arguments.
        CefMainArgs main_args(argc, argv);

    /************************************************************
     *                  Sub-process Handling
     ************************************************************/

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, THIS, NULL);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }

    /************************************************************
     *                  Main (Browser) Handling
     ************************************************************/

    CefSettings settings;

    if ( !settings.remote_debugging_port)
    {
        settings.remote_debugging_port = 9222;
    }

    // Install xlib error handlers so that the application won't be terminated
    // on non-fatal errors.
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);

    // Initialize CEF for the browser process.
    CefInitialize(main_args, settings, THIS, NULL);

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is
    // called.
    CefRunMessageLoop();

    CefShutdown();

    return 0;
}
