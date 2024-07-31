#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t numThreads) : stop(false), queues(numThreads), queueMutexes(numThreads)
{
	for (size_t i = 0; i < numThreads; ++i)
		workers.emplace_back([this, i] { workerThread(i); });
}

ThreadPool::~ThreadPool()
{
	stop.store(true);
	condition.notify_all();
	for (auto& worker : workers)
		worker.join();
}

void ThreadPool::workerThread(size_t index)
{
	while (!stop.load())
	{
		Task task;
		bool foundTask = false;
		{
			std::unique_lock<std::mutex> lock(queueMutexes[index]);
			condition.wait(lock, [this, &task, &foundTask, index]
				{
					if (!queues[index].empty())
					{
						task = queues[index].top();
						queues[index].pop();
						foundTask = true;
						return true;
					}
					return stop.load();
				});
		}

		if (foundTask)
			task.func();
		else
		{
			for (size_t i = 0; i < queues.size(); ++i)
			{
				if (i == index) continue;
				std::lock_guard<std::mutex> lock(queueMutexes[i]);
				if (!queues[i].empty())
				{
					task = queues[i].top();
					queues[i].pop();
					foundTask = true;
					break;
				}
			}
			if (foundTask)
				task.func();
		}
	}
}