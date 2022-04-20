#include "broker.h"

broker::broker(unsigned int maxR) {
    maxRequests = maxR;
    sem_init(&mutex, 0, 1);
    sem_init(&unconsumed, 0, 1);
    sem_init(&availableSlots, 0, MAXQUEUE);
    sem_init(&availableHumanSlots, 0, MAXHUMANDRIVER);
    sem_init(&allConsumed, 0, 0);

//    producedCounter = new int[2];
//    requestTracker = new int[2];
//    consumedCounter = new int[2][2];
}

pthread_info::pthread_info(broker* sharedBroker, unsigned int wait, RequestType requestType) {
    waitTime = wait;
    this->requestType = requestType;
    this->sharedBroker = sharedBroker;
}

cthread_info::cthread_info(broker* sharedBroker, unsigned int wait, ConsumerType consumerType) {
    waitTime = wait;
    this->consumerType = consumerType;
    this->sharedBroker = sharedBroker;
}
