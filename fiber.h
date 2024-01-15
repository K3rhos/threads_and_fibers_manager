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

	protected:
		virtual void start(bool& _allow_update) = 0;
		virtual void update() = 0;

	private:
		void* m_curr_fiber;
		void* m_main_fiber;
		bool m_allow_update;
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
};