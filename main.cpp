#include <getopt.h>
#include "broker.h"
#include <pthread.h>

#define REQUESTSDEFAULT 120
#define MAXQUEUE 12
#define MAXHUMANDRIVER 4

#define CCOSTSAVING 0
#define CFASTMATCHING 1
#define PHUMAN 2
#define PAUTONOMOUS 3

extern "C" void * cThread( void * VoidPtr )
{
//    auto *popTree = new populatetree();
//    popTree->threadMain(VoidPtr); //calls populatetree thread controller
//    pthread_exit(popTree);
//    return nullptr;
    auto* producerController = new producer();
}

extern "C" void * pThread( void * VoidPtr )
{
    auto* consumeController = new consumer();
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

    auto* mainBroker = new broker(totalRequests);

    pthread_t cCostSavingThread, cFastMatchingThread, pHumanThread, pAutoThread;

    pthread_create(&pHumanThread, nullptr, &pThread, mainBroker); //create producer thread 1
    pthread_create(&pAutoThread, nullptr, &pThread, mainBroker); //create producer thread 2
    pthread_create(&cCostSavingThread, nullptr, &cThread, mainBroker); //create customer thread 1
    pthread_create(&cFastMatchingThread, nullptr, &cThread, mainBroker); //create customer thread 2
}
