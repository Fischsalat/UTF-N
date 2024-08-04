#pragma once


/*
* Requirements:
*	* Typedefs (no char, no wchar_t, no char32_t)
*	* wchar_t is 0x2 bytes on windows, 0x4 on linux
* 
* Typedefs:
*	* Replace with actual feature macros later
*	*
*	* if (cppVersion >= c++20)
*	*	* typedef char8_t utf_char8_t;
*	*	* typedef char16_t utf_char16_t;
*	*	* typedef char32_t utf_char32_t;
*	* else if (cppVersion < c++11)
*	*	* typedef unsigned char utf_char8_t;
*	*	* typedef char16_t utf_char16_t;
*	*	* typedef char32_t utf_char32_t;
*	* else
*	*	* typedef unsigned char utf_char8_t;
*	*	* typedef uint16_t utf_char16_t;
*	*	* typedef uint32_t utf_char32_t;
* 
* Planned functionality:
*	* PUBLIC INTERFACE:
*	* bool hasXID_Start([char8, char16, char32]);
*	* bool hasXID_Continue([char8, char16, char32]);
* 
*	* bool replace(string_iterator)
* 
*	* void appendCharacterToUtf8String(string_iterator, utf_char16_t)
*	* void appendCharacterToUtf8String(string_iterator, utf_char32_t)
* 
*	* void writeMutlibytesToString(string_iterator, utf_char16_t)
*	* void writeMutlibytesToString(string_iterator, utf_char32_t)
* 
*	* utf_char8_t[2] utf16ToUtf8(utf_char16_t);
*	* utf_char8_t[4] utf16ToUtf8(utf_char32_t);
* 
*	* utf_char16_t[2] utf16ToUtf8(utf_char16_t);
*	* utf_char16_t[2] utf16ToUtf8(utf_char32_t);
*	* wchar_t conv_CharToWChar(char);
* 
*	* std::string wstrToStr(const std::wstring&)
* 
*	* INTERNAL IMPLEMENTATION:
*	* void writeMutlibytesToString(const utf_char8_t*, utf_char16_t)
*	* void writeMutlibytesToString(const utf_char8_t*, utf_char16_t)
*/  