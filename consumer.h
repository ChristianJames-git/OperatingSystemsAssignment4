#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
#define OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H


class consumer {
public:
    consumer(broker* thisBroker, unsigned int wait);
    unsigned int waitTime;
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_CONSUMER_H
