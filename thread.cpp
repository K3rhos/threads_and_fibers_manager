#include "thread.h"



thread::thread()
{
	m_is_running = true;

	m_handle = CreateThread
	(
		NULL,
		NULL,
		[](LPVOID _param) -> DWORD
		{
			thread* self = (thread*)_param;

			if (self)
			{
				self->start();

				while (self->m_is_running)
				{
					self->update();

					Sleep(0);
				}

				self->stop();
			}

			return EXIT_SUCCESS;
		},
		this,
		NULL,
		NULL
	);
}



thread::~thread()
{
	CloseHandle(m_handle);
}



void threads_pool::emplace(const std::string& _name, thread* _thread)
{
	if (!m_threads.contains(_name))
	{
		m_threads.emplace(_name, _thread);
	}
}



void threads_pool::erase(const std::string& _name)
{
	if (m_threads.contains(_name))
	{
		m_threads[_name]->close();

		m_threads.erase(_name);
	}
}



void threads_pool::clear()
{
	for (const auto& [name, thread] : m_threads)
	{
		thread->close();
	}

	m_threads.clear();
}