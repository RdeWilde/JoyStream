#include "RPCRequest.h"

RPCRequest::RPCRequest()
    : status_(READY)
{ }

void RPCRequest::proceed(bool fok)
{
    if (!fok) {
        this->deleteLater();
    } else {
        if (status_ == READY) {
            onCall();
        } else {
            delete this;
        }
    }
}
