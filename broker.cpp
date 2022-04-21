/*
 * Christian James
 * 823672623
 */

#include "broker.h"

broker::broker(unsigned int maxR) {
    maxRequests = maxR;
    sem_init(&mutex, 0, 1);
    sem_init(&unconsumed, 0, 0);
    sem_init(&availableSlots, 0, MAXQUEUE); //instantiate max queue size
    sem_init(&availableHumanSlots, 0, MAXHUMANDRIVER); //instantiate max humans in queue
    sem_init(&allConsumed, 0, 0);
}

pthread_info::pthread_info(broker* sharedBroker, unsigned int wait, RequestType requestType) { //stores producer thread info
    waitTime = wait;
    this->requestType = requestType;
    this->sharedBroker = sharedBroker;
}

cthread_info::cthread_info(broker* sharedBroker, unsigned int wait, ConsumerType consumerType) { //stores consumer thread info
    waitTime = wait;
    this->consumerType = consumerType;
    this->sharedBroker = sharedBroker;
}
