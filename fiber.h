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

		bool m_allow_update;

	protected:
		virtual void start() = 0;
		virtual void update() = 0;

	private:
		HANDLE m_curr_fiber;
		HANDLE m_main_fiber;
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
		std::map<std::string, fiber*> m_fibers;
};