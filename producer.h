/*
 * Christian James
 * 823672623
 */

#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H
#define OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H

class producer {
public:
    explicit producer(pthread_info* pInfo);
    void begin();
    unsigned int waitTime;
private:
    broker* sharedBroker;
    RequestType requestType;
    struct timespec	SleepTime{};
};

#endif