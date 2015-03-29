/*
 * CefBaseRequestReplies.cpp
 *
 *  Created on: 29 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseRequestReplies.h"


bool CefBaseJSRequestReplyHandler::InstallHandler(
    const std::string& name,
    HandlerPointer handler)
{
    auto it = handlers.find(name);
    bool ok = false;

    if (it == handlers.end()) {
        handlers.insert(HandlerMap::value_type(name, std::move(handler)));
    }

    return ok;
}


bool CefBaseJSRequestReplyHandler::OnQuery(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id,
    const CefString& request,
    bool persistent,
    CefRefPtr<Callback> callback)
{
    std::string reqString = request;

    bool handled = false;

    if ( strncmp("REQUEST_",reqString.c_str(),8) == 0 ) {
        handled = GetResponse(reqString, callback);
    }

    return handled;
}

bool CefBaseJSRequestReplyHandler::GetResponse(
    const std::string& reqString,
    const CefRefPtr<Callback>& callback)
{
    bool handled = false;

    std::string name;
    std::stringstream reqSStream(reqString);
    reqSStream >> name;

    const size_t prefixLen = 8; // "REQUEST_"

    const char* reqName = "";
    if (name.length() > prefixLen) {
        reqName = name.c_str() + prefixLen;
    }

    auto it = handlers.find(reqName);

    if (it != handlers.end()) {
        CefBaseJSRequestReply& handler = *it->second;
        handled = GetResponse(reqString, name, callback, handler);
    }

    return handled;
}

bool CefBaseJSRequestReplyHandler::GetResponse(
    const std::string&       request,
    const std::string&       name,
    const CefRefPtr<Callback> callback,
    CefBaseJSRequestReply& handler)
{
    bool handled = false;

    try {
        const size_t prefix = name.length() + 1; // include the space
        std::string response =
            request.length() > prefix ?
                handler.OnRequest(request.substr(prefix)):
                handler.OnRequest("");

        handled = true;
        callback->Success(std::move(response));
    } catch (CefBaseJSRequestReply::CefBaseInvalidRequestException& e) {
        handled = true;
        callback->Failure(e.code, e.errMsg);
    }
    return handled;
}

void CefBaseJSRequestReplyHandler::OnQueryCanceled(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id)
{
    // Nothing to do...
}
