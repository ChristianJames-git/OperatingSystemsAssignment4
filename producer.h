#include "broker.h"

#ifndef OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H
#define OPERATINGSYSTEMSASSIGNMENT4_PRODUCER_H

class producer {
public:
    producer(broker* thisBroker, unsigned int wait);
    unsigned int waitTime;
};

#endif