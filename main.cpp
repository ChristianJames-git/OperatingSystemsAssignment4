/*
 * Christian James
 * 823672623
 */

#include <getopt.h>
#include "producer.h"
#include "consumer.h"
#include <pthread.h>

#define PHUMAN 0
#define PAUTONOMOUS 1
#define CCOSTSAVING 2
#define CFASTMATCHING 3

/*
 * pThread(void* pInfo)
 * passes info to new producer thread controller and calls begin() method
 *
 * Expects:
 *     pInfo: unique information for specific thread
 */
extern "C" void * pThread( void * pInfo )
{
    auto* producerController = new producer((pthread_info *)pInfo);

    producerController->begin();

    pthread_exit(pInfo);
    return nullptr;
}

/*
 * cThread(void* cInfo)
 * passes info to new consumer thread controller and calls begin() method
 *
 * Expects:
 *     cInfo: unique information for specific thread
 */
extern "C" void * cThread( void * cInfo )
{
    auto* consumerController = new consumer((cthread_info *)cInfo);

    consumerController->begin();

    pthread_exit(cInfo);
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
                    waitTimes[CCOSTSAVING] = stoi(optarg); //store c
                break;
            case 'f':
                if (optarg)
                    waitTimes[CFASTMATCHING] = stoi(optarg); //store f
                break;
            case 'h':
                if (optarg)
                    waitTimes[PHUMAN] = stoi(optarg); //store h
                break;
            case 'a':
                if (optarg)
                    waitTimes[PAUTONOMOUS] = stoi(optarg); //store a
                break;
            default:
                break;
        }
    }

    //Create shared broker
    auto* sharedBroker = new broker(totalRequests);
    //Create unique information containers
    auto* phuman = new pthread_info(sharedBroker, waitTimes[PHUMAN], HumanDriver);
    auto* pauto = new pthread_info(sharedBroker, waitTimes[PAUTONOMOUS], RoboDriver);
    auto* ccost = new cthread_info(sharedBroker, waitTimes[CCOSTSAVING], CostAlgoDispatch);
    auto* cfast = new cthread_info(sharedBroker, waitTimes[CFASTMATCHING], FastAlgoDispatch);

    pthread_t pHumanThread, pAutoThread, cCostSavingThread, cFastMatchingThread;

    pthread_create(&pHumanThread, nullptr, &pThread, phuman); //create producer thread 1
    pthread_create(&pAutoThread, nullptr, &pThread, pauto); //create producer thread 2
    pthread_create(&cCostSavingThread, nullptr, &cThread, ccost); //create customer thread 1
    pthread_create(&cFastMatchingThread, nullptr, &cThread, cfast); //create customer thread 2

    sem_wait(&sharedBroker->allConsumed); //wait until consumer thread signals completion
    int* a[2] = {sharedBroker->consumedCounter[0], sharedBroker->consumedCounter[1] }; //format output as needed for io

    io_production_report(sharedBroker->producedCounter, a); //output
}
