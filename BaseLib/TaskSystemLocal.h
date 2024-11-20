#pragma once
#include <deque>
#include <functional>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>
namespace nims_n
{
	using lock_t = std::unique_lock<std::mutex>;
	class NotificationQueue
	{
		std::deque<std::function<void()>> _q;
		bool _done{ false };
		std::mutex _mutex;
		std::condition_variable _ready;

	public:
		bool try_pop(std::function<void()>& _job)
		{
			lock_t lock{ _mutex, std::try_to_lock };
			if (!lock || _q.empty()) return false;
			_job = std::move(_q.front());
			_q.pop_front();
			return true;
		}

		bool pop(std::function<void()>& job)
		{
			lock_t lock{ _mutex };
			while (_q.empty() && !_done)_ready.wait(lock);
			if (_q.empty()) return false;
			job = std::move(_q.front());
			_q.pop_front();
			return true;
		}

		template<typename F>
		bool try_push(F&& f)
		{
			{
				lock_t lock{ _mutex, std::try_to_lock };
				if (!lock) return false;
				_q.emplace_back(std::forward<F>(f)); 
			}
			_ready.notify_one();
			return true;
		}

		template<typename F>
		void push(F&& f)
		{
			{
				lock_t lock{ _mutex };
				_q.emplace_back(std::forward<F>(f));
			}
			_ready.notify_one();
		}

		void done()
		{
			{
				lock_t  lock{ _mutex };
				_done = true;
			}
			_ready.notify_all();
		}
	};
	
	class TaskSystemLocal
	{
		const unsigned _count{ std::thread::hardware_concurrency() };
		std::vector<std::thread> _threads;
		std::vector<NotificationQueue>_q{ _count };
		std::atomic<unsigned> _index{ 0 };

		void run(unsigned i)
		{
			while (true)
			{
				std::function<void()> f;
				for (unsigned n = 0; n != _count; ++n)
				{
					if (_q[(i + n) % _count].try_pop(f)) break;
				}
				if (!f && !_q[i].pop(f)) break;
				f();
			}
		}

	public:
		TaskSystemLocal() 
		{
			for (unsigned i = 0; i != _count; i++)
			{
				_threads.emplace_back([&, i] {run(i); });
			}
		}

		~TaskSystemLocal()
		{
			for (auto& e : _q) e.done();
			for (auto& e : _threads)e.join();
		}

		template<typename F>
		void async_(F&& f)
		{
			auto i = _index++;
			for (unsigned n = 0; n != _count * 100; n++)
			{
				if (_q[(i + n) % _count].try_push(std::forward<F>(f))) return;
			}
			_q[i % _count].push(std::forward<F>(f));
		}
	};
}


