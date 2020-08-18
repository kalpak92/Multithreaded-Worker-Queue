#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>

class MyLog {
public:
	MyLog() = default;
	
	void shared_print1(std::string str) {
		//Avoiding dead lock here
		std::lock(mtx1_, mtx2_);
		
		//Two mutex locks used in different orders may cause deadlock
		std::lock_guard<std::mutex> locker1(mtx1_, std::adopt_lock);
		std::lock_guard<std::mutex> locker2(mtx2_, std::adopt_lock);

		std::cout << "Thread Info: " << str << std::endl;
	}
	
	void shared_print2(std::string str) {
		std::lock(mtx1_, mtx2_);
		std::lock_guard<std::mutex> locker2(mtx2_, std::adopt_lock);
		std::lock_guard<std::mutex> locker1(mtx1_, std::adopt_lock);
		std::cout << "Thread Info: " << str << std::endl;
	}

private:
    std::mutex mtx1_;
	std::mutex mtx2_;
};

static MyLog log;

void thread_func() {
	for(size_t idx = 0; idx < 100; idx++)
	{
		std::stringstream m_str;
		m_str << "From Child Thread -> " << idx;
		log.shared_print1(m_str.str());
	}
}

int main() {
	std::thread t1(thread_func);
	
	for(size_t idx = 0; idx < 100; idx++) {
		std::stringstream m_str;
		m_str << "From Main Thread -> " << idx;
		log.shared_print2(m_str.str());
	}

	t1.join();
}