#include <iostream>
#include <memory>
#include <thread>
#include "workerQueue.hpp"

class Consumer {
public:
    Consumer();
    void run();
};
