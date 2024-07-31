#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

class ThreadPool
{
public:
	ThreadPool(size_t numThreads);
	~ThreadPool();

	template <class F, class... Args>
	auto enqueue(int priority, F&& f, Args&&... args) -> std::future<typename std::invoke_result<F(Args...)>::type>
	{
		using returnType = typename std::invoke_result<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<returnType> res = task->get_future();
		Task newTask{ priority, [task]() { (*task)(); } };
		size_t leastLoadedQueue = 0;
		size_t minLoad = queues[0].size();
		for (size_t i = 1; i < queues.size(); ++i) 
		{
			size_t load = queues[i].size();
			if (load < minLoad) 
			{
				minLoad = load;
				leastLoadedQueue = i;
			}
		}
		{
			std::lock_guard<std::mutex> lock(queueMutexes[leastLoadedQueue]);
			queues[leastLoadedQueue].push(newTask);
		}
		condition.notify_one();
		return res;
	}

private:
	struct Task
	{
		int priority;
		std::function<void()> func;
		bool operator<(const Task& other) const
		{
			return priority < other.priority;
		}
	};

	std::vector<std::thread> workers;
	std::vector<std::priority_queue<Task>> queues;
	std::vector<std::mutex> queueMutexes;
	std::condition_variable_any condition;
	std::atomic<bool> stop;

	void workerThread(size_t index);
};