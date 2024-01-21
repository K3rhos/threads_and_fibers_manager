#pragma once



#include <windows.h>
#include <string>
#include <map>
#include <memory>

#include "singleton.h"



class thread
{
	public:
		thread();
		~thread();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void stop() = 0;

		void close()
		{
			m_is_running = false;

			while (!m_has_stopped)
			{
				Sleep(1);
			}
		}

	private:
		HANDLE m_handle;
		bool m_is_running;
		bool m_has_stopped;
};



class threads_pool : public singleton<threads_pool>
{
	public:
		void emplace(const std::string& _name, thread* _thread);
		void erase(const std::string& _name);
		void clear();

	private:
		std::map<std::string, std::shared_ptr<thread>> m_threads;
};