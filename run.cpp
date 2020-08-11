#include <memory>
#include <chrono>
#include <stdlib.h>
#include "consumer.hpp"
#include "workerQueue.hpp"

int main(int argc, char** argv, char** envp) {
    bool isProducerFinished = false;
    if (argc != 2) {
        std::cout << "Needs command line arguments" << std::endl;
    }

    int iterations = std::atoi(argv[1]);


    WorkerQueue& workerInstance = WorkerQueue::getInstance();

    Consumer consumer1;
    Consumer consumer2;
    
    std::thread t1(&Consumer::run, consumer1);
    std::thread t2(&Consumer::run, consumer2);

    std::shared_ptr<WorkItem> item;
    for(int i = 0; i < iterations; i++) {
        item = std::make_shared<WorkItem>("abc", 123);
        workerInstance.add(item);
        std::cout << "Producer has produced an item." << std::endl;

        item = std::make_shared<WorkItem>("def", 456);
        workerInstance.add(item);
        std::cout << "Producer has produced an item." << std::endl;
        
        item = std::make_shared<WorkItem>("ghi", 789);
        workerInstance.add(item);
        std::cout << "Producer has produced an item." << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    isProducerFinished = true;

    t1.join();
    t2.join();

    if(isProducerFinished && workerInstance.size() == 0) {
        std::cout << "DONE" << std::endl;
        exit(0);
    }
}