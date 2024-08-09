#pragma once
#include "UtfN.hpp"


namespace UtfNTests
{
    using namespace UtfN;

	namespace Conversions
	{
		/*
		utf16_pair Utf32ToUtf16Pair(utf_char32_t);
		utf_char32_t Utf16PairToUtf32(utf16_pair);

		utf8_bytes Utf32ToUtf8Bytes(utf_char32_t);
		utf_char32_t Utf8BytesToUtf32(utf8_bytes);

		utf8_bytes Utf16PairToUtf8Bytes(utf16_pair);
		utf16_pair Utf8BytesToUtf16(utf8_bytes);
		*/

        constexpr void TestUtf32ToUtf16Pair()
        {
            // UTF-32 to UTF-16 (1-byte characters)
            static_assert(Utf32ToUtf16Pair(0x00000041) == utf16_pair{ 0x0041, 0x0000 }, "Utf32ToUtf16Pair failed for 'A'.");
            static_assert(Utf32ToUtf16Pair(0x00000061) == utf16_pair{ 0x0061, 0x0000 }, "Utf32ToUtf16Pair failed for 'a'.");
            static_assert(Utf32ToUtf16Pair(0x00000021) == utf16_pair{ 0x0021, 0x0000 }, "Utf32ToUtf16Pair failed for '!'.");

            // UTF-32 to UTF-16 (2-byte characters)
            static_assert(Utf32ToUtf16Pair(0x000000E9) == utf16_pair{ 0x00E9, 0x0000 }, "Utf32ToUtf16Pair failed for 'é'.");
            static_assert(Utf32ToUtf16Pair(0x000000F1) == utf16_pair{ 0x00F1, 0x0000 }, "Utf32ToUtf16Pair failed for 'ñ'.");
            static_assert(Utf32ToUtf16Pair(0x000003BB) == utf16_pair{ 0x03BB, 0x0000 }, "Utf32ToUtf16Pair failed for 'λ'.");

            // UTF-32 to UTF-16 (3-byte characters)
            static_assert(Utf32ToUtf16Pair(0x00000905) == utf16_pair{ 0x0905, 0x0000 }, "Utf32ToUtf16Pair failed for 'अ'.");
            static_assert(Utf32ToUtf16Pair(0x00004E2D) == utf16_pair{ 0x4E2D, 0x0000 }, "Utf32ToUtf16Pair failed for '中'.");
            static_assert(Utf32ToUtf16Pair(0x000020A4) == utf16_pair{ 0x20A4, 0x0000 }, "Utf32ToUtf16Pair failed for '₤'.");

            // UTF-32 to UTF-16 (4-byte characters)
            static_assert(Utf32ToUtf16Pair(0x0001F600) == utf16_pair{ 0xD83D, 0xDE00 }, "Utf32ToUtf16Pair failed for '😀'.");
            static_assert(Utf32ToUtf16Pair(0x0001F004) == utf16_pair{ 0xD83C, 0xDC04 }, "Utf32ToUtf16Pair failed for '🀄'.");
            static_assert(Utf32ToUtf16Pair(0x00010348) == utf16_pair{ 0xD800, 0xDF48 }, "Utf32ToUtf16Pair failed for '𐍈'.");

            // UTF-32 to UTF-16 (null terminator)
            static_assert(Utf32ToUtf16Pair(0x00000000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for null terminator.");

            // UTF-32 to UTF-16 (invalid characters)
            static_assert(Utf32ToUtf16Pair(0x00110000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (out of range).");
            static_assert(Utf32ToUtf16Pair(0x0000D800) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (lone surrogate).");
            static_assert(Utf32ToUtf16Pair(0x00110000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (beyond valid Unicode range).");
        }

        constexpr void TestUtf16PairToUtf32()
        {
            // UTF-16 to UTF-32 (1-byte characters)
            static_assert(Utf16PairToUtf32({ 0x0041, 0x0000 }) == 0x00000041, "Utf16PairToUtf32 failed for 'A'.");
            static_assert(Utf16PairToUtf32({ 0x0061, 0x0000 }) == 0x00000061, "Utf16PairToUtf32 failed for 'a'.");
            static_assert(Utf16PairToUtf32({ 0x0021, 0x0000 }) == 0x00000021, "Utf16PairToUtf32 failed for '!'.");

            // UTF-16 to UTF-32 (2-byte characters)
            static_assert(Utf16PairToUtf32({ 0x00E9, 0x0000 }) == 0x000000E9, "Utf16PairToUtf32 failed for 'é'.");
            static_assert(Utf16PairToUtf32({ 0x00F1, 0x0000 }) == 0x000000F1, "Utf16PairToUtf32 failed for 'ñ'.");
            static_assert(Utf16PairToUtf32({ 0x03BB, 0x0000 }) == 0x000003BB, "Utf16PairToUtf32 failed for 'λ'.");

            // UTF-16 to UTF-32 (3-byte characters)
            static_assert(Utf16PairToUtf32({ 0x0905, 0x0000 }) == 0x00000905, "Utf16PairToUtf32 failed for 'अ'.");
            static_assert(Utf16PairToUtf32({ 0x4E2D, 0x0000 }) == 0x00004E2D, "Utf16PairToUtf32 failed for '中'.");
            static_assert(Utf16PairToUtf32({ 0x20A4, 0x0000 }) == 0x000020A4, "Utf16PairToUtf32 failed for '₤'.");

            // UTF-16 to UTF-32 (4-byte characters)
            static_assert(Utf16PairToUtf32({ 0xD83D, 0xDE00 }) == 0x0001F600, "Utf16PairToUtf32 failed for '😀'.");
            static_assert(Utf16PairToUtf32({ 0xD83C, 0xDC04 }) == 0x0001F004, "Utf16PairToUtf32 failed for '🀄'.");
            static_assert(Utf16PairToUtf32({ 0xD800, 0xDF48 }) == 0x00010348, "Utf16PairToUtf32 failed for '𐍈'.");

            // UTF-16 to UTF-32 (null terminator)
            static_assert(Utf16PairToUtf32({ 0x0000, 0x0000 }) == 0x00000000, "Utf16PairToUtf32 failed for null terminator.");

            // UTF-16 to UTF-32 (invalid characters)
            static_assert(Utf16PairToUtf32({ 0xD800, 0x0000 }) == 0xFFFFFFFF, "Utf16PairToUtf32 failed for invalid character (lone surrogate).");
            static_assert(Utf16PairToUtf32({ 0xD83D, 0x0000 }) == 0xFFFFFFFF, "Utf16PairToUtf32 failed for invalid character (half of a surrogate pair).");
            static_assert(Utf16PairToUtf32({ 0xD800, 0xD800 }) == 0xFFFFFFFF, "Utf16PairToUtf32 failed for invalid character (invalid surrogate pair).");
        }

        constexpr void TestUtf32ToUtf8Bytes()
        {
            // UTF-32 to UTF-8 (1-byte characters)
            static_assert(Utf32ToUtf8Bytes(0x00000041) == utf8_bytes{ 0x41, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 'A'.");
            static_assert(Utf32ToUtf8Bytes(0x00000061) == utf8_bytes{ 0x61, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 'a'.");
            static_assert(Utf32ToUtf8Bytes(0x00000021) == utf8_bytes{ 0x21, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for '!'.");

            // UTF-32 to UTF-8 (2-byte characters)
            static_assert(Utf32ToUtf8Bytes(0x000000E9) == utf8_bytes{ 0xC3, 0xA9, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 'é'.");
            static_assert(Utf32ToUtf8Bytes(0x000000F1) == utf8_bytes{ 0xC3, 0xB1, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 'ñ'.");
            static_assert(Utf32ToUtf8Bytes(0x000003BB) == utf8_bytes{ 0xCE, 0xBB, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 'λ'.");

            // UTF-32 to UTF-8 (3-byte characters)
            static_assert(Utf32ToUtf8Bytes(0x00000905) == utf8_bytes{ 0xE0, 0xA4, 0x85, 0x00 }, "Utf32ToUtf8Bytes failed for 'अ'.");
            static_assert(Utf32ToUtf8Bytes(0x00004E2D) == utf8_bytes{ 0xE4, 0xB8, 0xAD, 0x00 }, "Utf32ToUtf8Bytes failed for '中'.");
            static_assert(Utf32ToUtf8Bytes(0x000020A4) == utf8_bytes{ 0xE2, 0x82, 0xA4, 0x00 }, "Utf32ToUtf8Bytes failed for '₤'.");

            // UTF-32 to UTF-8 (4-byte characters)
            static_assert(Utf32ToUtf8Bytes(0x0001F600) == utf8_bytes{ 0xF0, 0x9F, 0x98, 0x80 }, "Utf32ToUtf8Bytes failed for '😀'.");
            static_assert(Utf32ToUtf8Bytes(0x0001F004) == utf8_bytes{ 0xF0, 0x9F, 0x80, 0x84 }, "Utf32ToUtf8Bytes failed for '🀄'.");
            static_assert(Utf32ToUtf8Bytes(0x00010348) == utf8_bytes{ 0xF0, 0x90, 0x8D, 0x88 }, "Utf32ToUtf8Bytes failed for '𐍈'.");

            // UTF-32 to UTF-8 (null terminator)
            static_assert(Utf32ToUtf8Bytes(0x00000000) == UtfN::utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for null terminator.");

            // UTF-32 to UTF-8 (invalid characters)
            static_assert(Utf32ToUtf8Bytes(0x00110000) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (out of range).");
            static_assert(Utf32ToUtf8Bytes(0x0000D800) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (lone surrogate).");
            static_assert(Utf32ToUtf8Bytes(0x00110000) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (beyond valid Unicode range).");
        }

        constexpr void TestUtf8BytesToUtf32()
        {
            // UTF-8 to UTF-32 (1-byte characters)
            static_assert(Utf8BytesToUtf32({ 0x41, 0x00, 0x00, 0x00 }) == 0x00000041, "Utf8BytesToUtf32 failed for 'A'.");
            static_assert(Utf8BytesToUtf32({ 0x61, 0x00, 0x00, 0x00 }) == 0x00000061, "Utf8BytesToUtf32 failed for 'a'.");
            static_assert(Utf8BytesToUtf32({ 0x21, 0x00, 0x00, 0x00 }) == 0x00000021, "Utf8BytesToUtf32 failed for '!'.");

            // UTF-8 to UTF-32 (2-byte characters)
            static_assert(Utf8BytesToUtf32({ 0xC3, 0xA9, 0x00, 0x00 }) == 0x000000E9, "Utf8BytesToUtf32 failed for 'é'.");
            static_assert(Utf8BytesToUtf32({ 0xC3, 0xB1, 0x00, 0x00 }) == 0x000000F1, "Utf8BytesToUtf32 failed for 'ñ'.");
            static_assert(Utf8BytesToUtf32({ 0xCE, 0xBB, 0x00, 0x00 }) == 0x000003BB, "Utf8BytesToUtf32 failed for 'λ'.");

            // UTF-8 to UTF-32 (3-byte characters)
            static_assert(Utf8BytesToUtf32({ 0xE0, 0xA4, 0x85, 0x00 }) == 0x00000905, "Utf8BytesToUtf32 failed for 'अ'.");
            static_assert(Utf8BytesToUtf32({ 0xE4, 0xB8, 0xAD, 0x00 }) == 0x00004E2D, "Utf8BytesToUtf32 failed for '中'.");
            static_assert(Utf8BytesToUtf32({ 0xE2, 0x82, 0xA4, 0x00 }) == 0x000020A4, "Utf8BytesToUtf32 failed for '₤'.");

            // UTF-8 to UTF-32 (4-byte characters)
            static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0x98, 0x80 }) == 0x0001F600, "Utf8BytesToUtf32 failed for '😀'.");
            static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0x80, 0x84 }) == 0x0001F004, "Utf8BytesToUtf32 failed for '🀄'.");
            static_assert(Utf8BytesToUtf32({ 0xF0, 0x90, 0x8D, 0x88 }) == 0x00010348, "Utf8BytesToUtf32 failed for '𐍈'.");

            // UTF-8 to UTF-32 (null terminator)
            static_assert(Utf8BytesToUtf32({ 0x00, 0x00, 0x00, 0x00 }) == 0x00000000, "Utf8BytesToUtf32 failed for null terminator.");

            // UTF-8 to UTF-32 (invalid characters)
            static_assert(Utf8BytesToUtf32({ 0xC1, 0xBF, 0x00, 0x00 }) == 0xFFFFFFFF, "Utf8BytesToUtf32 failed for invalid character (overlong encoding).");
            static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0xC0, 0x80 }) == 0xFFFFFFFF, "Utf8BytesToUtf32 failed for invalid character (invalid continuation byte).");
            static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0x80, 0x00 }) == 0xFFFFFFFF, "Utf8BytesToUtf32 failed for invalid character (incomplete sequence).");
        }

        constexpr void TestUtf16PairToUtf8Bytes()
        {
            // UTF-16 to UTF-8 (1-byte characters)
            static_assert(Utf16PairToUtf8Bytes({ 0x0041, 0x0000 }) == utf8_bytes{ 0x41, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for 'A'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x0061, 0x0000 }) == utf8_bytes{ 0x61, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for 'a'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x0021, 0x0000 }) == utf8_bytes{ 0x21, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for '!'.");

            // UTF-16 to UTF-8 (2-byte characters)
            static_assert(Utf16PairToUtf8Bytes({ 0x00E9, 0x0000 }) == utf8_bytes{ 0xC3, 0xA9, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for 'é'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x00F1, 0x0000 }) == utf8_bytes{ 0xC3, 0xB1, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for 'ñ'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x03BB, 0x0000 }) == utf8_bytes{ 0xCE, 0xBB, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for 'λ'.");

            // UTF-16 to UTF-8 (3-byte characters)
            static_assert(Utf16PairToUtf8Bytes({ 0x0905, 0x0000 }) == utf8_bytes{ 0xE0, 0xA4, 0x85, 0x00 }, "Utf16PairToUtf8Bytes failed for 'अ'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x4E2D, 0x0000 }) == utf8_bytes{ 0xE4, 0xB8, 0xAD, 0x00 }, "Utf16PairToUtf8Bytes failed for '中'.");
            static_assert(Utf16PairToUtf8Bytes({ 0x20A4, 0x0000 }) == utf8_bytes{ 0xE2, 0x82, 0xA4, 0x00 }, "Utf16PairToUtf8Bytes failed for '₤'.");

            // UTF-16 to UTF-8 (4-byte characters)
            static_assert(Utf16PairToUtf8Bytes({ 0xD83D, 0xDE00 }) == utf8_bytes{ 0xF0, 0x9F, 0x98, 0x80 }, "Utf16PairToUtf8Bytes failed for '😀'.");
            static_assert(Utf16PairToUtf8Bytes({ 0xD83C, 0xDC04 }) == utf8_bytes{ 0xF0, 0x9F, 0x80, 0x84 }, "Utf16PairToUtf8Bytes failed for '🀄'.");
            static_assert(Utf16PairToUtf8Bytes({ 0xD800, 0xDF48 }) == utf8_bytes{ 0xF0, 0x90, 0x8D, 0x88 }, "Utf16PairToUtf8Bytes failed for '𐍈'.");

            // UTF-16 to UTF-8 (null terminator)
            static_assert(Utf16PairToUtf8Bytes({ 0x0000, 0x0000 }) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for null terminator.");

            // UTF-16 to UTF-8 (invalid characters)
            static_assert(Utf16PairToUtf8Bytes({ 0xD800, 0x0000 }) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for invalid character (lone surrogate).");
            static_assert(Utf16PairToUtf8Bytes({ 0xD83D, 0x0000 }) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for invalid character (half of a surrogate pair).");
            static_assert(Utf16PairToUtf8Bytes({ 0xD800, 0xD800 }) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf16PairToUtf8Bytes failed for invalid character (invalid surrogate pair).");
        }

        constexpr void TestUtf8BytesToUtf16()
        {
            // UTF-8 to UTF-16 (1-byte characters)
            static_assert(Utf8BytesToUtf16({ 0x41, 0x00, 0x00, 0x00 }) == utf16_pair{ 0x0041, 0x0000 }, "Utf8BytesToUtf16 failed for 'A'.");
            static_assert(Utf8BytesToUtf16({ 0x61, 0x00, 0x00, 0x00 }) == utf16_pair{ 0x0061, 0x0000 }, "Utf8BytesToUtf16 failed for 'a'.");
            static_assert(Utf8BytesToUtf16({ 0x21, 0x00, 0x00, 0x00 }) == utf16_pair{ 0x0021, 0x0000 }, "Utf8BytesToUtf16 failed for '!'.");

            // UTF-8 to UTF-16 (2-byte characters)
            static_assert(Utf8BytesToUtf16({ 0xC3, 0xA9, 0x00, 0x00 }) == utf16_pair{ 0x00E9, 0x0000 }, "Utf8BytesToUtf16 failed for 'é'.");
            static_assert(Utf8BytesToUtf16({ 0xC3, 0xB1, 0x00, 0x00 }) == utf16_pair{ 0x00F1, 0x0000 }, "Utf8BytesToUtf16 failed for 'ñ'.");
            static_assert(Utf8BytesToUtf16({ 0xCE, 0xBB, 0x00, 0x00 }) == utf16_pair{ 0x03BB, 0x0000 }, "Utf8BytesToUtf16 failed for 'λ'.");

            // UTF-8 to UTF-16 (3-byte characters)
            static_assert(Utf8BytesToUtf16({ 0xE0, 0xA4, 0x85, 0x00 }) == utf16_pair{ 0x0905, 0x0000 }, "Utf8BytesToUtf16 failed for 'अ'.");
            static_assert(Utf8BytesToUtf16({ 0xE4, 0xB8, 0xAD, 0x00 }) == utf16_pair{ 0x4E2D, 0x0000 }, "Utf8BytesToUtf16 failed for '中'.");
            static_assert(Utf8BytesToUtf16({ 0xE2, 0x82, 0xA4, 0x00 }) == utf16_pair{ 0x20A4, 0x0000 }, "Utf8BytesToUtf16 failed for '₤'.");

            // UTF-8 to UTF-16 (4-byte characters)
            static_assert(Utf8BytesToUtf16({ 0xF0, 0x9F, 0x98, 0x80 }) == utf16_pair{ 0xD83D, 0xDE00 }, "Utf8BytesToUtf16 failed for '😀'.");
            static_assert(Utf8BytesToUtf16({ 0xF0, 0x9F, 0x80, 0x84 }) == utf16_pair{ 0xD83C, 0xDC04 }, "Utf8BytesToUtf16 failed for '🀄'.");
            static_assert(Utf8BytesToUtf16({ 0xF0, 0x90, 0x8D, 0x88 }) == utf16_pair{ 0xD800, 0xDF48 }, "Utf8BytesToUtf16 failed for '𐍈'.");

            // UTF-8 to UTF-16 (null terminator)
            static_assert(Utf8BytesToUtf16({ 0x00, 0x00, 0x00, 0x00 }) == utf16_pair{ 0x0000, 0x0000 }, "Utf8BytesToUtf16 failed for null terminator.");

            // UTF-8 to UTF-16 (invalid characters)
            static_assert(Utf8BytesToUtf16({ 0xC1, 0xBF, 0x00, 0x00 }) == utf16_pair{ 0x0000, 0x0000 }, "Utf8BytesToUtf16 failed for invalid character (overlong encoding).");
            static_assert(Utf8BytesToUtf16({ 0xF0, 0x9F, 0xC0, 0x80 }) == utf16_pair{ 0x0000, 0x0000 }, "Utf8BytesToUtf16 failed for invalid character (invalid continuation byte).");
            static_assert(Utf8BytesToUtf16({ 0xF0, 0x9F, 0x80, 0x00 }) == utf16_pair{ 0x0000, 0x0000 }, "Utf8BytesToUtf16 failed for invalid character (incomplete sequence).");
        }

	}

	namespace Parsing
	{
		constexpr void TestParseUtf8CharFromStr()
		{
			static_assert(UtfN::ParseUtf8CharFromStr(u8"\0") == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse null-terminator.");

			static_assert(UtfN::ParseUtf8CharFromStr(u8"A") == utf8_bytes{ 0x41, 0x00, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 1-byte character 1.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"a") == utf8_bytes{ 0x61, 0x00, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 1-byte character 2.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"!") == utf8_bytes{ 0x21, 0x00, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 1-byte character 3.");

			static_assert(UtfN::ParseUtf8CharFromStr(u8"é") == utf8_bytes{ 0xC3, 0xA9, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 2-byte character 1.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"ñ") == utf8_bytes{ 0xC3, 0xB1, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 2-byte character 2.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"λ") == utf8_bytes{ 0xCE, 0xBB, 0x00, 0x00 }, "ParseUtf8CharFromStr failed to parse 2-byte character 3.");

			static_assert(UtfN::ParseUtf8CharFromStr(u8"अ") == utf8_bytes{ 0xE0, 0xA4, 0x85, 0x00 }, "ParseUtf8CharFromStr failed to parse 3-byte character 1.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"中") == utf8_bytes{ 0xE4, 0xB8, 0xAD, 0x00 }, "ParseUtf8CharFromStr failed to parse 3-byte character 2.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"₤") == utf8_bytes{ 0xE2, 0x82, 0xA4, 0x00 }, "ParseUtf8CharFromStr failed to parse 3-byte character 3.");

			static_assert(UtfN::ParseUtf8CharFromStr(u8"😀") == utf8_bytes{ 0xF0, 0x9F, 0x98, 0x80 }, "ParseUtf8CharFromStr failed to parse 4-byte character 3.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"🀄") == utf8_bytes{ 0xF0, 0x9F, 0x80, 0x84 }, "ParseUtf8CharFromStr failed to parse 4-byte character 3.");
			static_assert(UtfN::ParseUtf8CharFromStr(u8"𐍈") == utf8_bytes{ 0xF0, 0x90, 0x8D, 0x88 }, "ParseUtf8CharFromStr failed to parse 4-byte character 3.");
		}

		constexpr void TestParseUtf16CharFromStr()
		{
			static_assert(UtfN::ParseUtf16CharFromStr(u"\0") == utf16_pair{ 0x0000, 0x0000 }, "ParseUtf8CharFromStr failed to parse null-terminator.");

			static_assert(UtfN::ParseUtf16CharFromStr(u"A") == utf16_pair{ 0x0041, 0x0000 }, "ParseUtf16CharFromStr failed to parse 1-byte character 1.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"a") == utf16_pair{ 0x0061, 0x0000 }, "ParseUtf16CharFromStr failed to parse 1-byte character 2.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"!") == utf16_pair{ 0x0021, 0x0000 }, "ParseUtf16CharFromStr failed to parse 1-byte character 3.");

			static_assert(UtfN::ParseUtf16CharFromStr(u"é") == utf16_pair{ 0x00E9, 0x0000 }, "ParseUtf16CharFromStr failed to parse 2-byte character 1.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"ñ") == utf16_pair{ 0x00F1, 0x0000 }, "ParseUtf16CharFromStr failed to parse 2-byte character 2.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"λ") == utf16_pair{ 0x03BB, 0x0000 }, "ParseUtf16CharFromStr failed to parse 2-byte character 3.");


			static_assert(UtfN::ParseUtf16CharFromStr(u"अ") == utf16_pair{ 0x0905, 0x0000 }, "ParseUtf16CharFromStr failed to parse 3-byte character 1.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"中") == utf16_pair{ 0x4E2D, 0x0000 }, "ParseUtf16CharFromStr failed to parse 3-byte character 2.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"₤") == utf16_pair{ 0x20A4, 0x0000 }, "ParseUtf16CharFromStr failed to parse 3-byte character 3.");

			static_assert(UtfN::ParseUtf16CharFromStr(u"😀") == utf16_pair{ 0xD83D, 0xDE00 }, "ParseUtf16CharFromStr failed to parse 4-byte character 1.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"🀄") == utf16_pair{ 0xD83C, 0xDC04 }, "ParseUtf16CharFromStr failed to parse 4-byte character 2.");
			static_assert(UtfN::ParseUtf16CharFromStr(u"𐍈") == utf16_pair{ 0xD800, 0xDF48 }, "ParseUtf16CharFromStr failed to parse 4-byte character 3.");
		}

		constexpr void TestParseUtf16CharFromStr()
		{
			static_assert(UtfN::ParseUtf32CharFromStr(U"\0") == utf_char32_t{ 0x0000 }, "ParseUtf8CharFromStr failed to parse null-terminator.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"A") == utf_char32_t{ 0x00000041 }, "ParseUtf16CharFromStr failed to parse 1-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"a") == utf_char32_t{ 0x00000061 }, "ParseUtf16CharFromStr failed to parse 1-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"!") == utf_char32_t{ 0x00000021 }, "ParseUtf16CharFromStr failed to parse 1-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"é") == utf_char32_t{ 0x000000E9 }, "ParseUtf16CharFromStr failed to parse 2-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"ñ") == utf_char32_t{ 0x000000F1 }, "ParseUtf16CharFromStr failed to parse 2-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"λ") == utf_char32_t{ 0x000003BB }, "ParseUtf16CharFromStr failed to parse 2-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"अ") == utf_char32_t{ 0x00000905 }, "ParseUtf16CharFromStr failed to parse 3-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"中") == utf_char32_t{ 0x00004E2D }, "ParseUtf16CharFromStr failed to parse 3-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"₤") == utf_char32_t{ 0x000020A4 }, "ParseUtf16CharFromStr failed to parse 3-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"😀") == utf_char32_t{ 0x0001F600 }, "ParseUtf16CharFromStr failed to parse 4-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"🀄") == utf_char32_t{ 0x0001F004 }, "ParseUtf16CharFromStr failed to parse 4-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"𐍈") == utf_char32_t{ 0x00010348 }, "ParseUtf16CharFromStr failed to parse 4-byte character 3.");
		}
	}
}
