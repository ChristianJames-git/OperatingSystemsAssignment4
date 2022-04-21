/*
 * Christian James
 * 823672623
 */

#include "consumer.h"

/*
 * Store information from cthread_info
 * Set Sleep time necessary for nanosleep
 */
consumer::consumer(cthread_info* cInfo) {
    waitTime = cInfo->waitTime;
    sharedBroker = cInfo->sharedBroker;
    this->consumerType = cInfo->consumerType;
    SleepTime.tv_sec = (long)waitTime / MSPERSEC;
    SleepTime.tv_nsec = (long)(waitTime % MSPERSEC) * NSPERMS;
}

/*
 * Consumer thread method
 * Loop until last request consumed, then exit
 */
void consumer::begin() {
    RequestType requestType;
    while (!sharedBroker->maxReached || !sharedBroker->requestQueue.empty()) { //loop until producers finish and all requests are consumed

        //Sleep and error checking
        errno = 0;
        if (nanosleep(&SleepTime, nullptr) == -1) {
            switch (errno) {
                case EINTR:
                    cout << "interrupted by a signal handler" << endl;
                    exit(EXIT_FAILURE);
                case EINVAL:
                    cout << "nsec or sec incorrect" << endl;
                    exit(EXIT_FAILURE);
                default:
                    perror("nanosleep");
                    exit(EXIT_FAILURE);
            }
        }

        sem_wait(&sharedBroker->unconsumed); //Consume
        sem_wait(&sharedBroker->mutex); //Lock queue

        requestType = sharedBroker->requestQueue.front(); //Get front requestType
        sharedBroker->requestQueue.pop(); //remove front
        if (requestType == HumanDriver) //if requestType is Human
            sem_post(&sharedBroker->availableHumanSlots); //track allowed Human Request to keep under 4
        sharedBroker->requestTracker[requestType]--; //decrement request tracker
        sharedBroker->consumedCounter[consumerType][requestType]++; //increment consumed counter

        io_remove_type(consumerType, requestType, sharedBroker->requestTracker, sharedBroker->consumedCounter[consumerType]); //output

        sem_post(&sharedBroker->mutex); //release queue
        sem_post(&sharedBroker->availableSlots); //release slot
    }
    sem_post(&sharedBroker->allConsumed); //signal main to stop waiting
}
