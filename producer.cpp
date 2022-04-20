#include "producer.h"

producer::producer(broker* thisBroker, unsigned int wait) {
    waitTime = wait;
}
