#pragma once
#include <string>

class StringManager
{
	public:
		static const char* format(const char* str, ...);

		static const char* fromInt(long long n);
		static const char* fromUint(unsigned long long n);
		static const char* fromFloat(double n);

		static long long toInt(const char* str);
		static unsigned long long toUint(const char* str);
		static double toFloat(const char* str);
	
	private:
};