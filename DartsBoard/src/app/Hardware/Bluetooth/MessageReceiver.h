#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

class MessageReceiver {

public:
    virtual void receiveMessage(String) = 0;
};

#endif