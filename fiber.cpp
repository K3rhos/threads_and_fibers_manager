#include "fiber.h"



fiber::fiber()
{
	m_is_running = true;

	m_wake_at = GetTickCount64();

	m_main_fiber = INVALID_HANDLE_VALUE;
	m_curr_fiber = CreateFiber(NULL, [](LPVOID _param)
	{
		fiber* self = (fiber*)_param;

		if (self)
		{
			self->start();

			while (self->m_is_running)
			{
				self->update();

				self->wait(0);
			}

			SwitchToFiber(self->m_main_fiber);
		}
	}, this);
}



fiber::~fiber()
{
	DeleteFiber(m_curr_fiber);
}



void fiber::tick()
{
	m_main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);

	if (!m_is_running)
	{
		return;
	}

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



void fiber::close()
{
	m_is_running = false;

	stop();
}



void fibers_pool::do_run()
{
	// This make the thing thread safe and prevent to iterate in the loop and removing fibers at the same, that could result in a crash !
	if (m_locked)
	{
		return;
	}

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
	m_locked = true; // We are preventing the 'fibers_pool::do_run()' to iterate through the loop when removing fibers (So in this way it's thread safe !)

	if (m_fibers.contains(_name))
	{
		m_fibers[_name]->close();

		m_fibers.erase(_name);
	}

	m_locked = false;
}



void fibers_pool::clear()
{
	m_locked = true; // We are preventing the 'fibers_pool::do_run()' to iterate through the loop when removing fibers (So in this way it's thread safe !)

	for (const auto& [name, fiber] : m_fibers)
	{
		fiber->close();
	}

	m_fibers.clear();

	m_locked = false;
}