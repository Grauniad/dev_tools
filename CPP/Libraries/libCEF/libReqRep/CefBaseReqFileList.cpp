/*
 * CefBaseReqFileList.cpp
 *
 *  Created on: 5 Apr 2015
 *      Author: lhumphreys
 */

#include "CefBaseReqFileList.h"
#include <OSTools.h>

CefBaseReqFileList::CefBaseReqFileList() {
}

std::string CefBaseReqFileList::OnRequest(const std::string& rawRequest) {
    static std::string error;
    request.Clear();
    reply.Clear();

    if ( !request.Parse(rawRequest.c_str(),error) ) {
        throw CefBaseInvalidRequestException{0,error};
    }

    if ( request.Get<pattern>() == "")
    {
        throw CefBaseInvalidRequestException{0,"No pattern provided!"};
    }

    reply.Get<files>() = OS::Glob(request.Get<pattern>());

    return reply.GetJSONString();
}

CefBaseReqFileList::~CefBaseReqFileList() {
}

