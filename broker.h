#include "producer.h"
#include "consumer.h"
#include <iostream>
#include <queue>

#ifndef OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
#define OPERATINGSYSTEMSASSIGNMENT4_BROKER_H

using namespace std;

struct request {
    
};

class broker {
public:
    unsigned int maxRequests;
    queue<request> requestQueue;
};


#endif //OPERATINGSYSTEMSASSIGNMENT4_BROKER_H
