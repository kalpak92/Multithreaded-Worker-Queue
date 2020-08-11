#ifndef WORKERQUEUE_HPP
#define WORKERQUEUE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "workItem.hpp"

class WorkerQueue {
public :
    static WorkerQueue& getInstance();   
    void add(std::shared_ptr<WorkItem> item);
    std::shared_ptr<WorkItem> remove();
    int size();

private :
    WorkerQueue();
    std::queue<std::shared_ptr<WorkItem>> wQueue_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
#endif