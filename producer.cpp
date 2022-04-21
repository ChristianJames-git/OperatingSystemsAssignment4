/*
 * Christian James
 * 823672623
 */

#include "producer.h"

/*
 * Store information passed from pthread_info
 * Calculate sleeps times needed for nanosleep
 */
producer::producer(pthread_info* pInfo) {
    waitTime = pInfo->waitTime;
    sharedBroker = pInfo->sharedBroker;
    this->requestType = pInfo->requestType;
    SleepTime.tv_sec = (long)waitTime / MSPERSEC;
    SleepTime.tv_nsec = (long)(waitTime % MSPERSEC) * NSPERMS;
}

/*
 * Producer thread method
 * Loop until last request created, then exit
 */
void producer::begin() {
    RequestType newRequest;
    while (sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1] < sharedBroker->maxRequests) { //loop until all requests have been created
        newRequest = requestType;
        if (newRequest == HumanDriver) //check if Human Producer
            sem_wait(&sharedBroker->availableHumanSlots); //Ensure only 4 Human Requests at a time
        sem_wait(&sharedBroker->availableSlots); //Wait for available slot

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

        sem_wait(&sharedBroker->mutex); //Lock queue

        if (sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1] >= sharedBroker->maxRequests) { //if other producer created the last request while waiting, release resources and exit
            sem_post(&sharedBroker->mutex);
            sem_post(&sharedBroker->availableSlots);
            break;
        }

        sharedBroker->requestQueue.push(newRequest); //add to queue

        sharedBroker->producedCounter[newRequest]++; //request produced counter increment
        sharedBroker->requestTracker[newRequest]++; //current request tracker increment

        io_add_type(requestType, sharedBroker->requestTracker, sharedBroker->producedCounter); //output

        sem_post(&sharedBroker->mutex); //release queue

        sem_post(&sharedBroker->unconsumed); //signal unconsumed
    }
    sharedBroker->maxReached = true; //Set maxReached to true
}
