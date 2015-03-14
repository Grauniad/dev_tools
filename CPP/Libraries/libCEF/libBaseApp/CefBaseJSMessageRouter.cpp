/*
 * GCGVMessageRouter.cpp
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#include "CefBaseJSMessageRouter.h"

CefBaseJSMessageRouter::CefBaseJSMessageRouter(
    std::string queryFn,
    std::string cancelFn)

{
    CefMessageRouterConfig config;
    config.js_query_function = queryFn;
    config.js_cancel_function = cancelFn;

    browserSideRouter_ = CefMessageRouterBrowserSide::Create(config);
    rendererSideRouter_ = CefMessageRouterRendererSide::Create(config);

}

CefBaseJSMessageRouter::~CefBaseJSMessageRouter() {
}

CefRefPtr<CefMessageRouterBrowserSide> CefBaseJSMessageRouter::GetBrowserSideRouter()
{
    return browserSideRouter_;
}

CefRefPtr<CefMessageRouterRendererSide> CefBaseJSMessageRouter::GetRendererSideRouter()
{
    return rendererSideRouter_;
}
