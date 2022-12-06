#ifndef HttpConnection_H
#define HttpConnection_H
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "HttpConnectionListenner.h"

class HttpConnection
{
private:
    String httpHeaderAuthName;
    String httpHeaderAuthValue;
    HTTPClient http;
    WiFiClientSecure wifiClient;
    WiFiClientSecure wifiClientHttp;
    HttpConnectionListener *listener;

private:

public:
    HttpConnection();
    ~HttpConnection();
    void init();
    void setHttpAuthHeader(String name, String value);
    void httpGet(String &url);
    void httpPost(String &url, String &payload);
    void setHttpConnectionListener(HttpConnectionListener &listener);
};

extern HttpConnection restful;

#endif