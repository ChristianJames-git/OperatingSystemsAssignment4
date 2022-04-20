#include "producer.h"

producer::producer(broker* thisBroker, unsigned int wait, RequestType requestType) {
    waitTime = wait;
    sharedBroker = thisBroker;
    this->requestType = requestType;
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

        if (sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1] >= sharedBroker->maxRequests)
            break;
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
        sem_wait(&sharedBroker->mutex);

        sharedBroker->requestQueue.push(newRequest);

        sharedBroker->producedCounter[newRequest]++;
        sharedBroker->requestTracker[newRequest]++;

        //io_add_type(requestType, sharedBroker->requestTracker, sharedBroker->producedCounter);
        printf("output: %d  %d\n", sharedBroker->requestTracker[0]+sharedBroker->requestTracker[1], sharedBroker->producedCounter[0] + sharedBroker->producedCounter[1]);
        printf("%d\n", sharedBroker->requestQueue.size());
        //printf("%s -- %d + %d = %d -- %d + %d = %d\n", requestType, sharedBroker->requestTracker[0], sharedBroker->requestTracker[1], sharedBroker->requestTracker[0]+sharedBroker->requestTracker[1], sharedBroker->producedCounter[0], sharedBroker->producedCounter[1], sharedBroker->producedCounter[0]+sharedBroker->producedCounter[1]);

        sem_post(&sharedBroker->mutex);

        sem_post(&sharedBroker->unconsumed);
    }
    sharedBroker->maxReached = true;
}
