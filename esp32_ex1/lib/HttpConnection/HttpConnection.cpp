#include "HttpConnection.h"
#include "Util.h"
#include <freertos/FreeRTOS.h>

HttpConnection::HttpConnection()
{
}

HttpConnection::~HttpConnection()
{
    delete listener;
}

void HttpConnection::init()
{
    wifiClientHttp.setInsecure();
}


void HttpConnection::setHttpAuthHeader(String name, String value)
{
    httpHeaderAuthName = name;
    httpHeaderAuthValue = value;
}


void HttpConnection::httpGet(String &url)
{
    if (http.begin(wifiClientHttp, url))
    {
        // http.addHeader(httpHeaderAuthName, httpHeaderAuthValue, true, true);
        int statusCode = http.GET();
        if (listener != NULL)
        {
            listener->onHttpResponse(url, statusCode, http.getString());
        }
        http.end();
    }
}

void HttpConnection::httpPost(String &url, String &payload)
{
    if (http.begin(wifiClientHttp, url))
    {
        http.addHeader(httpHeaderAuthName, httpHeaderAuthValue, true, true);
        http.addHeader("Content-Type", "application/json", false, false);

        int statusCode = http.POST(payload);
        if (listener != NULL)
        {
            listener->onHttpResponse(url, statusCode, http.getString());
        }
        http.end();
    }
}

void HttpConnection::setHttpConnectionListener(HttpConnectionListener &listener)
{
    this->listener = &listener;
}

HttpConnection restful;