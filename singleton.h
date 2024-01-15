#pragma once



template<typename T>
class singleton
{
	public:
		static T* get()
		{
			static T singleton;

			return &singleton;
		}
};