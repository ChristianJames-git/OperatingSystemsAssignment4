#include "producer.h"

producer::producer(pthread_info* pInfo) {
    waitTime = pInfo->waitTime;
    sharedBroker = pInfo->sharedBroker;
    this->requestType = pInfo->requestType;
    SleepTime.tv_sec = (long)waitTime / MSPERSEC;
    SleepTime.tv_nsec = (long)(waitTime % MSPERSEC) * NSPERMS;
}

void producer::begin() {
    RequestType newRequest;
    while (sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1] < sharedBroker->maxRequests) {
        newRequest = requestType;
        if (newRequest == HumanDriver)
            sem_wait(&sharedBroker->availableHumanSlots);
        sem_wait(&sharedBroker->availableSlots);

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
        if (sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1] >= sharedBroker->maxRequests)
            break;
        sem_wait(&sharedBroker->mutex);

        sharedBroker->requestQueue.push(newRequest);

        sharedBroker->producedCounter[newRequest]++;
        sharedBroker->requestTracker[newRequest]++;

        //io_add_type(requestType, sharedBroker->requestTracker, sharedBroker->producedCounter);

        sem_post(&sharedBroker->mutex);

        sem_post(&sharedBroker->unconsumed);
    }
    sharedBroker->maxReached = true;
}
