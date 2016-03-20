#pragma once

const char* UnicodeToUtf8(const wchar_t *str);
const wchar_t* Utf8ToUnicode(const char *str);

#ifndef UTF8
	#ifndef WIN32
		#define UTF8(code) (UnicodeToUtf8(L##code))
	#else
		#define UTF8(code) code
	#endif
#endif

#ifndef UTF16L
	#ifndef WIN32
		#define UTF16L(code) (L##code)
	#else
		#define UTF16L(code) (Utf8ToUnicode(code))
	#endif
#endif