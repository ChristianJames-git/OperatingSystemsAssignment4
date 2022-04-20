#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H
#define OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H

class producer {
public:
    producer(broker* thisBroker, unsigned int wait, RequestType requestType);
    void begin();
    unsigned int waitTime;
private:
    broker* sharedBroker;
    RequestType requestType;
    struct timespec	SleepTime{};
};

#endif