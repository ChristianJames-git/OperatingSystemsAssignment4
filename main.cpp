#include <getopt.h>
#include "producer.h"
#include "consumer.h"
#include <pthread.h>

#define CCOSTSAVING 0
#define CFASTMATCHING 1
#define PHUMAN 2
#define PAUTONOMOUS 3

extern "C" void * pThread( void * sharedBroker )
{
    auto* tempBroker = static_cast<broker *>(sharedBroker);
    auto* producerController = new producer(tempBroker, tempBroker->waitTimes[tempBroker->currThreads], tempBroker->currThreads++ == PHUMAN ? HumanDriver : RoboDriver);

    producerController->begin();

    cout << "pthread" << endl;

    pthread_exit(tempBroker);
    return nullptr;
}

extern "C" void * cThread( void * sharedBroker )
{
    auto* tempBroker = static_cast<broker *>(sharedBroker);
    auto* consumerController = new consumer(tempBroker, tempBroker->waitTimes[tempBroker->currThreads], tempBroker->currThreads++ == CCOSTSAVING ? CostAlgoDispatch : FastAlgoDispatch);

    consumerController->begin();

    cout << "cthread" << endl;

    pthread_exit(tempBroker);
    return nullptr;
}

int main(int argc, char **argv) {
    int temp;
    unsigned int totalRequests = REQUESTSDEFAULT;
    unsigned int waitTimes[4]{}; //cost-saving consumer, fast-matching consumer, human driver, autonomous driver - set all to 0 default - milliseconds
    while ((temp = getopt (argc, argv, "-:n:c:f:h:a:")) != -1) { //handle arguments. '-' causes it to not skip mandatory ones, they have temp=1
        switch (temp) {
            case 'n':
                if (optarg)
                    totalRequests = stoi(optarg); //store n
                break;
            case 'c':
                if (optarg)
                    waitTimes[CCOSTSAVING] = stoi(optarg);
                break;
            case 'f':
                if (optarg)
                    waitTimes[CFASTMATCHING] = stoi(optarg);
                break;
            case 'h':
                if (optarg)
                    waitTimes[PHUMAN] = stoi(optarg);
                break;
            case 'a':
                if (optarg)
                    waitTimes[PAUTONOMOUS] = stoi(optarg);
                break;
            default:
                break;
        }
    }

    auto* sharedBroker = new broker(totalRequests, waitTimes);

    pthread_t pHumanThread, pAutoThread, cCostSavingThread, cFastMatchingThread;

    pthread_create(&pHumanThread, nullptr, &pThread, sharedBroker); //create producer thread 1
    pthread_create(&pAutoThread, nullptr, &pThread, sharedBroker); //create producer thread 2
    pthread_create(&cCostSavingThread, nullptr, &cThread, sharedBroker); //create customer thread 1
    pthread_create(&cFastMatchingThread, nullptr, &cThread, sharedBroker); //create customer thread 2

    sem_wait(&sharedBroker->allConsumed);

    //io_production_report(sharedBroker->producedCounter, sharedBroker->consumedCounter);
    printf("input");
    //printf("Human: %d ; Robot: %d ; CostAlgo: %d + %d = %d ; FastAlgo: %d + %d = %d\n", sharedBroker->producedCounter[0], sharedBroker->producedCounter[1], sharedBroker->consumedCounter[0][0], sharedBroker->consumedCounter[0][1], sharedBroker->consumedCounter[0][0]+sharedBroker->consumedCounter[0][1], sharedBroker->consumedCounter[1][0], sharedBroker->consumedCounter[1][1], sharedBroker->consumedCounter[1][0]+sharedBroker->consumedCounter[1][1]);
}
