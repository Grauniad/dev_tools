/*
 * GCGVMessageRouter.h
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#ifndef GCGVMESSAGEROUTER_H_
#define GCGVMESSAGEROUTER_H_

#include <include/wrapper/cef_message_router.h>
#include <string>

class CefBaseJSMessageRouter {
public:
    /*
     * Create a new message router
     */
    CefBaseJSMessageRouter(std::string queryFn, std::string cancelFn);

    virtual ~CefBaseJSMessageRouter();

    CefRefPtr<CefMessageRouterBrowserSide> GetBrowserSideRouter();

    CefRefPtr<CefMessageRouterRendererSide> GetRendererSideRouter();


private:
    CefRefPtr<CefMessageRouterBrowserSide>  browserSideRouter_;
    CefRefPtr<CefMessageRouterRendererSide> rendererSideRouter_;

	IMPLEMENT_REFCOUNTING(CefBaseJSMessageRouter);
};

#endif /* GCGVMESSAGEROUTER_H_ */
