#ifndef MQTTConnectionListenner_H
#define MQTTConnectionListenner_H

class MQTTConnectionListenner
{
public:
    virtual void onSubMessage(char *payload) = 0;
};

#endif