#pragma once
#include <string>
#include <vector>

class StringManager
{
	public:
		static const char* format(const char* str, ...);
		static const char* trimBegin(const char* str, ...);
		static const char* trimBegin(const char* str, const std::vector<char> &vec);
		static const char* trimEnd(const char* str, ...);
		static const char* trimEnd(const char* str, const std::vector<char> &vec);

		static bool isContainChar(const char* str, ...);

		static const char* fromInt(long long n);
		static const char* fromUint(unsigned long long n);
		static const char* fromFloat(double n);

		static long long toInt(const char* str);
		static unsigned long long toUint(const char* str);
		static double toFloat(const char* str);
	
	private:
};