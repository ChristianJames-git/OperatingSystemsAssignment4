#include "broker.h"

broker::broker(unsigned int maxR, unsigned int wait[4]) {
    maxRequests = maxR;
    waitTimes = wait;
    sem_init(&mutex, 0, 1);
    sem_init(&unconsumed, 0, 0);
    sem_init(&availableSlots, 0, MAXQUEUE);
    sem_init(&availableHumanSlots, 0, MAXHUMANDRIVER);
    sem_init(&allConsumed, 0, 0);

//    producedCounter = new int[2];
//    requestTracker = new int[2];
//    consumedCounter = new int[2][2];
}
