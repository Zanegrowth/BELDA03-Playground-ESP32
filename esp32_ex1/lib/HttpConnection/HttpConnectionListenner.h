#ifndef HttpConnectionListenner_H
#define HttpConnectionListenner_H

enum DownloadEvent
{
    DOWNLOAD_FAIL,
    DOWNLOAD_RESPONSE_ERROR
};

class HttpConnectionListener
{
public:
    virtual void onHttpResponse(const String &url, int statusCode, const String &responseText) = 0;
};

#endif
