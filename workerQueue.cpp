#include "workerQueue.hpp"

WorkerQueue::WorkerQueue() {
}

WorkerQueue& WorkerQueue::getInstance() {
    static WorkerQueue instance;
    return instance;
}


void WorkerQueue::add(std::shared_ptr<WorkItem> item) {
    {
        std::lock_guard<std::mutex> lock_wQueue(mtx_);
        wQueue_.push(item);
        std::cout << "Producer added an item" << std::endl;
    }
    cv_.notify_one();
}

std::shared_ptr<WorkItem> WorkerQueue::remove() {
    std::shared_ptr<WorkItem> item;
    {
        std::unique_lock<std::mutex> wait_wQueue(mtx_);
        while(wQueue_.empty()) {
            cv_.wait(wait_wQueue);
        }
        item = wQueue_.front();
        wQueue_.pop();
        std::cout <<    "Removing an item." <<  "Message: " << item->getMessage() << 
                        " Number: " << item->getNumber() << std::endl;
    }
    return item;
}

int WorkerQueue::size() {
    int result = 0;
    {
        std::lock_guard<std::mutex> lock_wQueue(mtx_);
        result = wQueue_.size();
    }
    return result;
}