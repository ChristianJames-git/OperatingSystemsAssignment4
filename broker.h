#include "io.h"
#include "semaphore.h"
#include <cerrno>
#include <ctime>
#include <iostream>
#include <queue>

/* One million nanoseconds per millisecond */
#define	NSPERMS		1000000
/* One thousand milliseconds per second */
#define MSPERSEC	1000

#define REQUESTSDEFAULT 120
#define MAXQUEUE 12
#define MAXHUMANDRIVER 4

#ifndef OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
#define OPERATINGSYSTEMSASSIGNMENT4_BROKER_H

using namespace std;

class broker {
public:
    broker(unsigned int maxR, unsigned int wait[4]);
    unsigned int maxRequests;
    queue<RequestType> requestQueue;
    int currThreads = 0;
    unsigned int* waitTimes;
    sem_t mutex{};
    sem_t availableSlots{};
    sem_t unconsumed{};
    sem_t availableHumanSlots{};
    int requestTracker[2]{};
    int producedCounter[2]{};
    int consumedCounter[2][2]{};
//    int* requestTracker;
//    int* producedCounter;
//    int (*)[2] consumedCounter;
    bool maxReached = false;
    sem_t allConsumed{};
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
