#include "consumer.h"

consumer::consumer(cthread_info* cInfo) {
    waitTime = cInfo->waitTime;
    sharedBroker = cInfo->sharedBroker;
    this->consumerType = cInfo->consumerType;
    SleepTime.tv_sec = (long)waitTime / MSPERSEC;
    SleepTime.tv_nsec = (long)(waitTime % MSPERSEC) * NSPERMS;
}

void consumer::begin() {
    RequestType requestType;
    while (!sharedBroker->maxReached || !sharedBroker->requestQueue.empty()) {
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
        sem_wait(&sharedBroker->unconsumed);
        sem_wait(&sharedBroker->mutex);

        requestType = sharedBroker->requestQueue.front();
        sharedBroker->requestQueue.pop();
        if (requestType == HumanDriver)
            sem_post(&sharedBroker->availableHumanSlots);
        sharedBroker->requestTracker[requestType]--;
        sharedBroker->consumedCounter[consumerType][requestType]++;

        io_remove_type(consumerType, requestType, sharedBroker->requestTracker, sharedBroker->consumedCounter[consumerType]);

        sem_post(&sharedBroker->mutex);
        sem_post(&sharedBroker->availableSlots);
    }
    sem_post(&sharedBroker->allConsumed);
}
