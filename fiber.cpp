#include "fiber.h"



fiber::fiber()
{
	m_wake_at = GetTickCount64();
	m_allow_update = true;

	m_main_fiber = INVALID_HANDLE_VALUE;
	m_curr_fiber = CreateFiber(NULL, [](LPVOID _param)
	{
		fiber* self = (fiber*)_param;

		if (self)
		{
			self->start();

			while (self->m_allow_update)
			{
				self->update();

				self->wait(0);
			}

			self->wait(LLONG_MAX);
		}
	}, this);
}



fiber::~fiber()
{
	CloseHandle(m_main_fiber);
	DeleteFiber(m_curr_fiber);
}



void fiber::tick()
{
	m_main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);

	if (GetTickCount64() < m_wake_at)
	{
		return;
	}

	SwitchToFiber(m_curr_fiber);
}



void fiber::wait(ULONGLONG _ms)
{
	m_wake_at = GetTickCount64() + _ms;

	SwitchToFiber(m_main_fiber);
}



void fibers_pool::do_run()
{
	for (const auto& [name, fiber] : m_fibers)
	{
		fiber->tick();
	}
}



void fibers_pool::emplace(const std::string& _name, fiber* _fiber)
{
	erase(_name);

	m_fibers.emplace(_name, _fiber);
}



void fibers_pool::erase(const std::string& _name)
{
	if (m_fibers.contains(_name))
	{
		m_fibers.erase(_name);
	}
}



void fibers_pool::clear()
{
	m_fibers.clear();
}