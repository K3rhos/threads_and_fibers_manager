#pragma once



#include <windows.h>
#include <string>
#include <map>
#include <memory>

#include "singleton.h"



class fiber
{
	public:
		fiber();
		~fiber();

		void tick();
		void wait(ULONGLONG _ms);
		void close();

	protected:
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void stop() = 0;

	private:
		bool m_is_running;

		void* m_curr_fiber;
		void* m_main_fiber;
		
		ULONGLONG m_wake_at;
};



class fibers_pool : public singleton<fibers_pool>
{
	public:
		void do_run();
		
		void emplace(const std::string& _name, fiber* _fiber);
		void erase(const std::string& _name);
		void clear();

	private:
		std::map<std::string, std::shared_ptr<fiber>> m_fibers;

		bool m_locked = false;
};