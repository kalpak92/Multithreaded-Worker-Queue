#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <climits>
#include <condition_variable>
#include <fstream>

#define INF INT_MAX / 2

using namespace std;

mutex mu_;

condition_variable cv_;

enum class ret_status
{
    SUCCESS,
    FAILURE
};

class Logger {
public:
    void operator()(std::string log_str, int log_int) {
        //Below line will gurantee, that logfile will open once by all threads
        std::call_once(flag_, [&]() { logfile_.open("pub_sub_log.txt"); });

        // adopt_lock_t : assume the calling thread already has ownership of the mutex
        std::lock_guard<std::mutex> glock(mtx_, adopt_lock);

        logfile_ << "Thread id " << this_thread::get_id << " :\t";
        logfile_ << log_str << log_int << endl;
    }

    ~Logger() {
        logfile_.close();
    }

private:
    mutex mtx_;
    once_flag flag_;
    ofstream logfile_;
};

class Publish {
public:
    ret_status operator()(std::queue<int> &msg_que, std::vector<int> data_bus, Logger &log) {
        // After processing of Data Bus the termination condition for subscriber will trigger
        data_bus.push_back(INF);

        try {
            for (auto data : data_bus) {
                std::unique_lock<std::mutex> ulock(mu_, defer_lock); // defer_lock_t: do not acquire ownership of the mutex

                ulock.lock();
                msg_que.push(data);
                log("Published:\t", data);
                ulock.unlock();

				// Notify All other threads, who are waiting on this thread
                cv_.notify_all();
            }
        }
        catch (exception e) {
            cerr << e.what() << endl;
            return ret_status::FAILURE;
        }
        return ret_status::SUCCESS;
    }
};

class Subscribe {
public:
    ret_status operator()(std::queue<int> &msg_que, Logger &log) {
        int data = 0;
        try {
            while (true) {
                unique_lock<mutex> ulock(mu_);
                /**
                 * Waiting using the same cv_ variable.
                 * Also, to protect Spurious wake, we are using one extra condition in the lambda function.
                 */
                cv_.wait(ulock, [=]() { return !msg_que.empty(); });

                data = msg_que.front();
                msg_que.pop();
                ulock.unlock();

                if (data == INF) {
                    break;
                }
                log("Subscribed:\t", data);
            }
        }
        catch (exception e) {
            cerr << e.what() << endl;
            return ret_status::FAILURE;
        }
        return ret_status::SUCCESS;
    }
};

int main() {
    try {
        std::queue<int> msg_que; //Act as a shared memory
        
        Publish pub;
        Subscribe sub;
        
        vector<int> data_bus = {5, 9, 6, 8, 99, 999, 158};
        Logger log;

        thread t1(pub, ref(msg_que), data_bus, ref(log));
        thread t2(sub, ref(msg_que), ref(log));

        t1.join();
        t2.join();
    }
    catch (exception e) {
        cerr << e.what() << endl;
    }

    return 0;
}