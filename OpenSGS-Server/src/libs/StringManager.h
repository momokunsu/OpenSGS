#pragma once
#include <string>

class StringManager
{
	public:
		static std::string formatString(const char* str, ...);
		static const char* format(const char* str, ...);

		static std::string stringFromInt(long long n);
		static std::string stringFromUint(unsigned long long n);
		static std::string stringFromFloat(double n);
		static const char* fromInt(long long n) { return stringFromInt(n).c_str(); }
		static const char* fromUint(unsigned long long n) { return stringFromUint(n).c_str(); }
		static const char* fromFloat(double n) { return stringFromFloat(n).c_str(); }

		static long long toInt(const char* str);
		static unsigned long long toUint(const char* str);
		static double toFloat(const char* str);
	
	private:
};