#pragma once



template<typename T>
class singleton
{
	public:
		singleton(const singleton&) = delete;
		singleton& operator=(const singleton&) = delete;

		static T* get()
		{
			static T singleton;

			return &singleton;
		}

	protected:
		singleton() = default;
};