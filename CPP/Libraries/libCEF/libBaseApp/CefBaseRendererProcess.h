/*
 * CefBaseRendererProcess.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASERENDERERPROCESS_H_
#define CEFBASERENDERERPROCESS_H_

#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"
#include "CefBaseMultiHandler.h"

class CefBaseRendererProcess:
        public CefBaseMultiHandler<CefRenderProcessHandler>
{
public:
    CefBaseRendererProcess();

    virtual ~CefBaseRendererProcess();

    /**
     * PROCESS: Renderer
     * THREAD:  RENDERER
     *
     * Event: The java script (v8) context has been created
     *
     * This callback is called immediately after a new java script context has
     * been created, and can be used to set up any variables or install new
     * handlers.
     *
     * @param CefBrowser    The browser in which the js context was created
     * @param CefFrame      The frame (within the browser) in which the js
     *                      context was created
     * @param CefV8Context  The newly created JS context.
     */
    CefBaseMultiHandler_Forward3Refs(
        OnContextCreated, CefBrowser, CefFrame, CefV8Context);
private:
};
#endif /* CEFBASERENDERERPROCESS_H_ */
