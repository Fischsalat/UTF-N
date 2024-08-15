A simple C++ lib for converting between Utf8, Utf16 and Utf32. Not yet complete.

Currently implemented functionality:
### Struct
```c++
// Common interface
template<UtfEncodingType Encoding>
struct utf_char
{
    CharType Character = { 0 };

public:
    utf_char(const CharType* SingleCharString);

public:
    utf_char& operator=(CharType inBytse);

public:
    bool operator==(utf_char Other) const;
    bool operator!=(utf_char Other) const;

public:
    utf_char8 GetAsUtf8() const;
    utf_char16 GetAsUtf16() const;
    utf_char32 GetAsUtf32() const;

    utf_char Get() const;

    UtfEncodingType GetEncoding() const;
    uint8_t GetByteSize() const;
};

typedef utf_char<UtfEncodingType::Utf8> utf_char8;
typedef utf_char<UtfEncodingType::Utf16> utf_char16;
typedef utf_char<UtfEncodingType::Utf32> utf_char32;
```
### Helper functions
```c++
uint8_t GetUtf8CharLenght(const utf_char8_t);
uint8_t GetUtf16CharLenght(const utf_char16_t);
```
### Conversions
```c++
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
