#include "ridesharing.h"
#include "semaphore.h"
#include <iostream>
#include <queue>

#define REQUESTSDEFAULT 120
#define MAXQUEUE 12
#define MAXHUMANDRIVER 4

#ifndef OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
#define OPERATINGSYSTEMSASSIGNMENT4_BROKER_H

using namespace std;

struct request {
    unsigned int wait;

};

class broker {
public:
    broker(unsigned int maxR, unsigned int wait[4]);
    unsigned int maxRequests;
    queue<request> requestQueue;
    int currThreads = 0;
    unsigned int* waitTimes;
    sem_t mutex{};
    sem_t availableSlots{};
    sem_t unconsumed{};
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
