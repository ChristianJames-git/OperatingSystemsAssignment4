#include "consumer.h"

consumer::consumer(broker* thisBroker, unsigned int wait, ConsumerType consumerType) {
    waitTime = wait;
    sharedBroker = thisBroker;
    this->consumerType = consumerType;
    SleepTime.tv_sec = (long)waitTime / MSPERSEC;
    SleepTime.tv_nsec = (long)(waitTime % MSPERSEC) * NSPERMS;
}

void consumer::begin() {
    RequestType requestType;
    cout << sharedBroker->consumedCounter[0][0]+sharedBroker->consumedCounter[1][0]+sharedBroker->consumedCounter[0][1]+sharedBroker->consumedCounter[1][1] << endl;
    while (!sharedBroker->maxReached || !sharedBroker->requestQueue.empty()) {
        int value;
        sem_getvalue(&sharedBroker->unconsumed, &value);
        printf("%d %d %d %s\n", value, sharedBroker->consumedCounter[0][0]+sharedBroker->consumedCounter[1][0]+sharedBroker->consumedCounter[0][1]+sharedBroker->consumedCounter[1][1],sharedBroker->requestTracker[0]+sharedBroker->requestTracker[1] , sharedBroker->requestQueue.empty() ? "true" : "false");
        sem_wait(&sharedBroker->unconsumed);

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

        requestType = sharedBroker->requestQueue.front();
        sharedBroker->requestQueue.pop();
        if (requestType == HumanDriver)
            sem_post(&sharedBroker->availableHumanSlots);
        sharedBroker->requestTracker[requestType]--;
        sharedBroker->consumedCounter[consumerType][requestType]++;

        //io_remove_type(consumerType, requestType, sharedBroker->requestTracker, sharedBroker->consumedCounter[consumerType]);
        //printf("consume %d %s\n", sharedBroker->consumedCounter[0][0]+sharedBroker->consumedCounter[1][0]+sharedBroker->consumedCounter[0][1]+sharedBroker->consumedCounter[1][1], sharedBroker->requestQueue.empty() ? "true" : "false");
        //printf("%s by %s -- %d + %d = %d -- %d + %d = %d\n", consumerType, requestType, sharedBroker->requestTracker[0], sharedBroker->requestTracker[1], sharedBroker->requestTracker[0]+sharedBroker->requestTracker[1], sharedBroker->consumedCounter[consumerType][0], sharedBroker->consumedCounter[consumerType][1], sharedBroker->consumedCounter[consumerType][0]+sharedBroker->consumedCounter[consumerType][1]);

        sem_post(&sharedBroker->mutex);
        sem_post(&sharedBroker->availableSlots);
    }
    sem_post(&sharedBroker->allConsumed);
}
