#pragma once
#include <mutex>
#include <memory>
#include <concepts>
template<typename T>
	requires std::is_default_constructible_v<T>
struct ThreadingResourceShared 
{
	ThreadingResourceShared() 
	{
		rsc = std::make_shared<T>();
		mtx = std::make_shared<std::recursive_mutex>();
	}
	ThreadingResourceShared(const ThreadingResourceShared<T>& src) 
	{
		rsc = src.rsc;
		mtx = src.mtx;
	}
	ThreadingResourceShared(ThreadingResourceShared<T>&& src)
	{
		rsc = std::move(src.rsc);
		mtx = std::move(src.mtx);
	}
	ThreadingResourceShared& operator=(const ThreadingResourceShared<T>& src) 
	{
		rsc = src.rsc;
		mtx = src.mtx;
		return *this;
	}
	ThreadingResourceShared& operator=(ThreadingResourceShared<T>&& src) 
	{
		rsc = std::move(src.rsc);
		mtx = std::move(src.mtx);
		return *this;
	}
	std::shared_ptr<T> rsc;
	std::shared_ptr<mutable std::recursive_mutex> mtx;
};
template<typename T>
	requires std::is_default_constructible_v<T>
struct ThreadingResourceLight 
{
	ThreadingResourceLight() 
	{
		rsc = std::make_unique<T>();
	}
	ThreadingResourceLight(ThreadingResourceLight<T>&& src) 
	{
		rsc = std::move(src.rsc);
		mtx = std::move(src.mtx);
	}
	ThreadingResourceLight& operator=(ThreadingResourceLight<T>&& src) 
	{
		rsc = std::move(src.rsc);
		mtx = std::move(src.mtx);
		return *this;
	}
	ThreadingResourceLight(const ThreadingResourceLight<T>& src) = delete;
	ThreadingResourceLight& operator=(const ThreadingResourceLight<T>& src) = delete;
	std::unique_ptr<T> rsc;
	mutable std::recursive_mutex mtx;
};