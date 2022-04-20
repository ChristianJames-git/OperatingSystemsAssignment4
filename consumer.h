#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
#define OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H


class consumer {
public:
    consumer(broker* thisBroker, unsigned int wait, ConsumerType consumerType);
    void begin();
    unsigned int waitTime;
private:
    broker* sharedBroker;
    ConsumerType consumerType;
    struct timespec	SleepTime{};
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
