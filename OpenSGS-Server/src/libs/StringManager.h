#pragma once

#include <string>
#include <vector>

namespace utli
{
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
			static const void split(std::vector<std::string>& arr, const char* str, va_list ap);
			static const void split(std::vector<std::string>& arr, const char* str, ...);

			static int compare(const char* src, const char* dst);
			static bool equals(const char* src, const char* dst) { return compare(src, dst) == 0; }

			static int indexOf(const char* str, const char* dst);
			static int indexOf(const char* str, char dst);

			static int endIndexOf(const char* str, const char* dst);
			static int endIndexOf(const char* str, char dst);

			static const char* subString(const char* str, int start_index, int len = 0);

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

			static const char* UTF16LToUTF8(const wchar_t *str);
			static const wchar_t* UTF8ToUTF16L(const char *str);
		private:
			static char* g_buffer;
			static int g_buffer_len;

			inline static const char* getBuffer(int len)
			{
				if (len > g_buffer_len)
				{
					delete[] g_buffer;
					while (len > g_buffer_len)
						g_buffer_len += 4 * 1024;
					g_buffer = new char[g_buffer_len];
				}
				return g_buffer;
			}

			inline static bool isRangeOfBuffer(const char* ptr)
			{
				return ptr >= g_buffer && ptr < g_buffer + g_buffer_len;
			}
	};
};

#ifndef UTF8
	#ifndef WIN32
		#define UTF8(code) (StringManager::UTF16LToUTF8(L##code))
	#else
		#define UTF8(code) code
	#endif
#endif

#ifndef UTF16L
	#ifndef WIN32
		#define UTF16L(code) (L##code)
	#else
		#define UTF16L(code) (StringManager::UTF8ToUTF16L(code))
	#endif
#endif