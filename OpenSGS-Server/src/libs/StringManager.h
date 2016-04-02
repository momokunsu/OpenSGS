#pragma once
#include <string>
#include <vector>

class StringManager
{
	public:
		static const char* format(const char* str, ...);
		static const char* trimBegin(const char* str, va_list ap);
		static const char* trimBegin(const char* str, ...);
		static const char* trimEnd(const char* str, va_list ap);
		static const char* trimEnd(const char* str, ...);
		static const char* replace(const char* str, char src, char dst);
		static const char* replace(const char* str, const char* src, const char* dst);
		static const void split(const char* str, std::vector<std::string>& arr, va_list ap);
		static const void split(const char* str, std::vector<std::string>& arr, ...);

		static int indexOf(const char* str, const char* dst);
		static int indexOf(const char* str, char dst);

		static bool isContainsChar(const char* str, va_list ap);
		static bool isContainsChar(const char* str, ...);
		static bool isContainsChar(char cvar, va_list ap);
		static bool isContainsChar(char cvar, ...);

		static const char* fromInt(long long n);
		static const char* fromUint(unsigned long long n);
		static const char* fromFloat(double n);

		static long long toInt(const char* str);
		static unsigned long long toUint(const char* str);
		static double toFloat(const char* str);
		static void toFormatValue(const char* str, const char* fmt, void *value);
	
	private:
};