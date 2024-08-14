A simple C++ lib for converting between Utf8, Utf16 and Utf32. Not yet complete.

Currently implemented functionality:
### Helper functions
```c++
int GetUtf8CharLenght(const utf_char8_t);
int GetUtf16CharLenght(const utf_char16_t);

// Single-character conversions
utf16_pair Utf32ToUtf16Pair(const utf_char32_t);
utf_char32_t Utf16PairToUtf32(const utf16_pair);
utf8_bytes Utf32ToUtf8Bytes(const utf_char32_t);
utf_char32_t Utf8BytesToUtf32(const utf8_bytes);
Utf16PairToUtf8Bytes(const utf16_pair);
Utf8BytesToUtf16(const utf8_bytes)
```
### Parsing
```c++
// Converts a single utf8 char contained in a C-String to utf8_bytes, requires(sizeof(CodepointType) == 0x1)
template<typename CodepointType>
utf8_bytes ParseUtf8CharFromStr(const CodepointType* Str);

// Converts a single utf16 char contained in a C-String to a utf16_pair, requires(sizeof(CodepointType) == 0x1)
template<typename CodepointType>
utf16_pair ParseUtf16CharFromStr(const CodepointType* Str);

// Converts a single utf32 char encoded in a 32-bit C-String to utf_char32_t, requires(sizeof(CodepointType) == 0x1)
template<typename CodepointType>
utf_char32_t ParseUtf32CharFromStr(const CodepointType* Str);
```
