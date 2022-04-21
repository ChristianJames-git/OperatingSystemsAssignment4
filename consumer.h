/*
 * Christian James
 * 823672623
 */

#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
#define OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H


class consumer {
public:
    explicit consumer(cthread_info* cInfo);
    void begin();
    unsigned int waitTime;
private:
    broker* sharedBroker;
    ConsumerType consumerType;
    struct timespec	SleepTime{};
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
