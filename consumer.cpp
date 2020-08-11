#include "consumer.hpp"

Consumer::Consumer() {
}

/*
 * The run() method continually waits for and removes items to be processed which in this case just means 
 * displaying the string message and number assigned in the producer thread. 
 * The ID of each thread is also displayed to differentiate them in the printed output.
 */
void Consumer::run() {
    WorkerQueue& instance = WorkerQueue::getInstance();

    for(int i = 0; ; i++) {
        std::cout << "Thread id " << std::this_thread::get_id() << " ,loop " << i
            << " waiting for item..." << std::endl;
        
        instance.remove();

        std::cout << "Thread id " << std::this_thread::get_id() << std::endl;
    }
}