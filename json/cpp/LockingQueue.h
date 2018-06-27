#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class LockingQueue
{
public:

	bool try_pop_for_ms(T& item, int ms) {
		std::unique_lock<std::mutex> lock(mu);
		if (q.empty()) {
			auto status = cv.wait_for(lock, std::chrono::milliseconds(ms));
			if (status == std::cv_status::timeout) {
				return false;
			}
		}
		if (!q.empty()) {
			item = q.front();
			q.pop();
			return true;
		}
		return false;
	}

	void push(const T& item) {
		{
			std::lock_guard<std::mutex> lock(mu);
			q.push(item);
		}
		cv.notify_one();
	}

	void push(T&& item) {
		{
			std::lock_guard<std::mutex> lock(mu);
			q.push(std::move(item));
		}
		cv.notify_one();
	}

private:
	std::queue<T> q;
	std::mutex mu;
	std::condition_variable cv;
};
