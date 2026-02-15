#pragma once

#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
private:
	std::mutex queueMutex;
	std::queue<std::function<void()>> queue;
	std::vector<std::thread> workers;
	std::condition_variable condition;
	std::atomic<bool> stop;

public:
	ThreadPool(size_t n_Threads) {
		stop = false;
		for (size_t i = 0; i < n_Threads; i++) {
			workers.emplace_back([this] { start(); });
		}
	}
 
	template<class F>
	void enqueue(F&& task) {
		std::unique_lock<std::mutex> lock(queueMutex);
		queue.emplace(std::forward<F>(task));
		lock.unlock();
		condition.notify_one();
	}

	void start() {
		while (true) {
			std::unique_lock<std::mutex> lock(queueMutex);
			condition.wait(lock, [this] { return stop || !queue.empty(); });

			if (stop && queue.empty())
				return;

			auto task = queue.front();
			queue.pop();
			lock.unlock();
			task();
		}
	}


	~ThreadPool() {
		std::unique_lock<std::mutex> lock(queueMutex);
		stop = true;
		lock.unlock();
		condition.notify_all();
		for (std::thread& worker : workers)
			worker.join();
	}
};