#pragma once

#include "cocos2d.h"

char* UnicodeToUtf8(const wchar_t *str);
wchar_t* Utf8ToUnicode(const char *str);

#ifndef UTF8
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		#define UTF8(code) (UnicodeToUtf8(L##code))
	#else
		#define UTF8(code) code
	#endif
#endif

#ifndef UTF16L
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		#define UTF16L(code) (L##code)
	#else
		#define UTF16L(code) (Utf8ToUnicode(code))
	#endif
#endif