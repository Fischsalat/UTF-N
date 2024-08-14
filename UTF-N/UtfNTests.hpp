#pragma once
#include "UtfN.hpp"

//#define UTFN_CONVERSION_TESTS
#define UTFN_PARSING_TESTS

#define UTFN_INTERNALS_TESTS

#define UTFN_TESTS


// Suppress warnings for UTFN Tests
#if (defined(_MSC_VER))
#pragma warning (push)
#pragma warning (disable: 4514) // C4514 "unreferenced inline function has been removed"
#elif (defined(__CLANG__) || defined(__GNUC__))
#endif // Warnings

namespace UtfNTests
{
	using namespace UtfN;

	namespace InternalsTest
	{
		namespace Utf8
		{
			template<utf_cp8_t FirstCp, utf_cp8_t SecondCp, utf_cp8_t ThirdCp, utf_cp8_t FourthCp>
			constexpr bool HelperUtf8IsValidUnicodeChar()
			{
				return UtfImpl::Utf8::IsValidUtf8Sequence<GetUtf8CharLenght(FirstCp)>(FirstCp, SecondCp, ThirdCp, FourthCp);
			}

			constexpr void TestIsValidUnicodeEncoding()
			{
#if defined(UTFN_TESTS) && defined(UTFN_INTERNALS_TESTS)
				// Overlong encoding
				static_assert(!HelperUtf8IsValidUnicodeChar<0xF0, 0x80, 0x80, 0xA4>(), "IsValidUtf8Sequence returned { true } for an invalid overlong encoding, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0xC0, 0x80, 0x00, 0x00>(), "IsValidUtf8Sequence returned { true } for an invalid overlong encoding, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0xE0, 0x80, 0xBF, 0x00>(), "IsValidUtf8Sequence returned { true } for an invalid overlong encoding, should be { false }.");


				// Invalid 3-byte UTF-8 sequence with invalid byters
				static_assert(!HelperUtf8IsValidUnicodeChar<0xE2, 0x28, 0xAC, 0x00>(), "IsValidUtf8Sequence returned { true } for an invalid UTF-8 sequence with bad second byte, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0xC2, 0x7F, 0x00, 0x00>(), "IsValidUtf8Sequence returned { true } for an invalid UTF-8 sequence with bad continuation byte, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0xF0, 0x9F, 0x20, 0x80>(), "IsValidUtf8Sequence returned { true } for an invalid UTF-8 sequence with bad third byte, should be { false }.");
				
				// Standalone continuation byte 0x80 / Invalid contination byte in multibyte sequence
				static_assert(!HelperUtf8IsValidUnicodeChar<0x80, 0x00, 0x00, 0x00>(), "IsValidUtf8Sequence returned { true } for a standalone continuation byte, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0xBF, 0x00, 0x00, 0x00>(), "IsValidUtf8Sequence returned { true } for a standalone continuation byte, should be { false }.");
				static_assert(!HelperUtf8IsValidUnicodeChar<0x80, 0xBF, 0x00, 0x00>(), "IsValidUtf8Sequence returned { true } for an invalid 2-byte sequence starting with a continuation byte, should be { false }.");

				// Out of range codepoints
				// Not covered by the encoding-check
#endif // UTFN_TESTS && UTFN_INTERNALS_TESTS
			}
		}

		namespace Utf16
		{
			constexpr void TestIsLowSurrogate()
			{
#if defined(UTFN_TESTS) && defined(UTFN_INTERNALS_TESTS)
				// Valid low surrogate tests
				static_assert(UtfImpl::Utf16::IsLowSurrogate(0xDC00), "IsLowSurrogate returned { false } for { 0xDC00 }, should be { true }.");
				static_assert(UtfImpl::Utf16::IsLowSurrogate(0xDFFF), "IsLowSurrogate returned { false } for { 0xDFFF }, should be { true }.");

				// Invalid low surrogate tests
				static_assert(!UtfImpl::Utf16::IsLowSurrogate(0xD83D), "IsLowSurrogate returned { true } for { 0xD83D }, should be { false }.");
				static_assert(!UtfImpl::Utf16::IsLowSurrogate(0xD800), "IsLowSurrogate returned { true } for { 0xD800 }, should be { false }.");
				static_assert(!UtfImpl::Utf16::IsLowSurrogate(0xDBFF), "IsLowSurrogate returned { true } for { 0xDBFF }, should be { false }.");
				static_assert(!UtfImpl::Utf16::IsLowSurrogate(0xE000), "IsLowSurrogate returned { true } for { 0xE000 }, should be { false }.");
			}

			constexpr void TestIsHighSurrogate()
			{
				// Valid high surrogate tests
				static_assert(UtfImpl::Utf16::IsHighSurrogate(0xD800), "IsHighSurrogate returned { false } for { 0xD800 }, should be { true }.");
				static_assert(UtfImpl::Utf16::IsHighSurrogate(0xDBFF), "IsHighSurrogate returned { false } for { 0xDBFF }, should be { true }.");
				static_assert(UtfImpl::Utf16::IsHighSurrogate(0xD83D), "IsHighSurrogate returned { false } for { 0xD83D }, should be { true }.");

				// Invalid high surrogate tests
				static_assert(!UtfImpl::Utf16::IsHighSurrogate(0xDC00), "IsHighSurrogate returned { true } for { 0xDC00 }, should be { false }.");
				static_assert(!UtfImpl::Utf16::IsHighSurrogate(0xDFFF), "IsHighSurrogate returned { true } for { 0xDFFF }, should be { false }.");
				static_assert(!UtfImpl::Utf16::IsHighSurrogate(0xE000), "IsHighSurrogate returned { true } for { 0xE000 }, should be { false }.");
#endif // UTFN_TESTS && UTFN_INTERNALS_TESTS
			}
		}

		namespace Utf32
		{
		}
	}

	namespace Conversions
	{
		/*
		utf16_pair Utf32ToUtf16Pair(utf_cp32_t);
		utf_cp32_t Utf16PairToUtf32(utf16_pair);

		utf8_bytes Utf32ToUtf8Bytes(utf_cp32_t);
		utf_cp32_t Utf8BytesToUtf32(utf8_bytes);

		utf8_bytes Utf16PairToUtf8Bytes(utf16_pair);
		utf16_pair Utf8BytesToUtf16(utf8_bytes);
		*/

		constexpr void TestUtf32ToUtf16Pair()
		{
#if defined(UTFN_TESTS) && defined(UTFN_CONVERSION_TESTS)
			// UTF-32 to UTF-16 (1-byte characters)
			static_assert(Utf32ToUtf16Pair(0x00000041) == utf16_pair{ 0x0041, 0x0000 }, "Utf32ToUtf16Pair failed for 1-byte character { 0x00000041 }.");
			static_assert(Utf32ToUtf16Pair(0x00000061) == utf16_pair{ 0x0061, 0x0000 }, "Utf32ToUtf16Pair failed for 1-byte character { 0x00000061 }.");
			static_assert(Utf32ToUtf16Pair(0x00000021) == utf16_pair{ 0x0021, 0x0000 }, "Utf32ToUtf16Pair failed for 1-byte character { 0x00000021 }.");

			// UTF-32 to UTF-16 (2-byte characters)
			static_assert(Utf32ToUtf16Pair(0x000000E9) == utf16_pair{ 0x00E9, 0x0000 }, "Utf32ToUtf16Pair failed for 2-byte character { 0x000000E9 }.");
			static_assert(Utf32ToUtf16Pair(0x000000F1) == utf16_pair{ 0x00F1, 0x0000 }, "Utf32ToUtf16Pair failed for 2-byte character { 0x000000F1 }.");
			static_assert(Utf32ToUtf16Pair(0x000003BB) == utf16_pair{ 0x03BB, 0x0000 }, "Utf32ToUtf16Pair failed for 2-byte character { 0x000003BB }.");

			// UTF-32 to UTF-16 (3-byte characters)
			static_assert(Utf32ToUtf16Pair(0x00000905) == utf16_pair{ 0x0905, 0x0000 }, "Utf32ToUtf16Pair failed for 3-byte character { 0x00000905 }.");
			static_assert(Utf32ToUtf16Pair(0x00004E2D) == utf16_pair{ 0x4E2D, 0x0000 }, "Utf32ToUtf16Pair failed for 3-byte character { 0x00004E2D }.");
			static_assert(Utf32ToUtf16Pair(0x000020A4) == utf16_pair{ 0x20A4, 0x0000 }, "Utf32ToUtf16Pair failed for 3-byte character { 0x000020A4 }.");

			// UTF-32 to UTF-16 (4-byte characters)
			static_assert(Utf32ToUtf16Pair(0x0001F600) == utf16_pair{ 0xDE00, 0xD83D }, "Utf32ToUtf16Pair failed for 4-byte character { 0x0001F600 }.");
			static_assert(Utf32ToUtf16Pair(0x0001F004) == utf16_pair{ 0xDC04, 0xD83C }, "Utf32ToUtf16Pair failed for 4-byte character { 0x0001F004 }.");
			static_assert(Utf32ToUtf16Pair(0x00010348) == utf16_pair{ 0xDF48, 0xD800 }, "Utf32ToUtf16Pair failed for 4-byte character { 0x00010348 }.");

			// UTF-32 to UTF-16 (null terminator)
			static_assert(Utf32ToUtf16Pair(0x00000000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for null terminator.");

			// UTF-32 to UTF-16 (invalid characters)
			static_assert(Utf32ToUtf16Pair(0x00110000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (out of range).");
			static_assert(Utf32ToUtf16Pair(0x0000D800) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (lone surrogate).");
			static_assert(Utf32ToUtf16Pair(0x00110000) == utf16_pair{ 0x0000, 0x0000 }, "Utf32ToUtf16Pair failed for invalid character (beyond valid Unicode range).");
#endif // UTFN_TESTS && UTFN_CONVERSION_TESTS
		}

		constexpr void TestUtf16PairToUtf32()
		{
#if defined(UTFN_TESTS) && defined(UTFN_CONVERSION_TESTS)
			// UTF-16 to UTF-32 (1-byte characters)
			static_assert(Utf16PairToUtf32({ 0x0041, 0x0000 }) == 0x00000041, "Utf16PairToUtf32 failed for 1-byte character { 0x00000041 }.");
			static_assert(Utf16PairToUtf32({ 0x0061, 0x0000 }) == 0x00000061, "Utf16PairToUtf32 failed for 1-byte character { 0x00000061 }.");
			static_assert(Utf16PairToUtf32({ 0x0021, 0x0000 }) == 0x00000021, "Utf16PairToUtf32 failed for 1-byte character { 0x00000021 }.");

			// UTF-16 to UTF-32 (2-byte characters)
			static_assert(Utf16PairToUtf32({ 0x00E9, 0x0000 }) == 0x000000E9, "Utf16PairToUtf32 failed for 2-byte character { 0x000000E9 }.");
			static_assert(Utf16PairToUtf32({ 0x00F1, 0x0000 }) == 0x000000F1, "Utf16PairToUtf32 failed for 2-byte character { 0x000000F1 }.");
			static_assert(Utf16PairToUtf32({ 0x03BB, 0x0000 }) == 0x000003BB, "Utf16PairToUtf32 failed for 2-byte character { 0x000003BB }.");

			// UTF-16 to UTF-32 (3-byte characters)
			static_assert(Utf16PairToUtf32({ 0x0905, 0x0000 }) == 0x00000905, "Utf16PairToUtf32 failed for 3-byte character { 0x00000905 }.");
			static_assert(Utf16PairToUtf32({ 0x4E2D, 0x0000 }) == 0x00004E2D, "Utf16PairToUtf32 failed for 3-byte character { 0x00004E2D }.");
			static_assert(Utf16PairToUtf32({ 0x20A4, 0x0000 }) == 0x000020A4, "Utf16PairToUtf32 failed for 3-byte character { 0x000020A4 }.");

			// UTF-16 to UTF-32 (4-byte characters)
			static_assert(Utf16PairToUtf32({0xDE00, 0xD83D }) == 0x0001F600, "Utf16PairToUtf32 failed for 4-byte character { 0x0001F600 }.");
			static_assert(Utf16PairToUtf32({ 0xDC04, 0xD83C }) == 0x0001F004, "Utf16PairToUtf32 failed for 4-byte character { 0x0001F004 }.");
			static_assert(Utf16PairToUtf32({ 0xDF48, 0xD800 }) == 0x00010348, "Utf16PairToUtf32 failed for 4-byte character { 0x00010348 }.");

			// UTF-16 to UTF-32 (null terminator)
			static_assert(Utf16PairToUtf32({ 0x0000, 0x0000 }) == 0x00000000, "Utf16PairToUtf32 failed for null terminator.");

			static_assert(Utf16PairToUtf32({ 0xD800, 0x0000 }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (lone surrogate).");
			static_assert(Utf16PairToUtf32({ 0xD83D, 0x0000 }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (half of a surrogate pair).");
			static_assert(Utf16PairToUtf32({ 0xD800, 0xD800 }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (invalid surrogate pair).");

			static_assert(Utf16PairToUtf32({ 0x0000, 0xD800 }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (lone surrogate).");
			static_assert(Utf16PairToUtf32({ 0x0000, 0xD83D }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (half of a surrogate pair).");
			static_assert(Utf16PairToUtf32({ 0xD800, 0xD800 }) == 0x00000000, "Utf16PairToUtf32 failed for invalid character (invalid surrogate pair).");
#endif // UTFN_TESTS && UTFN_CONVERSION_TESTS
		}

		constexpr void TestUtf32ToUtf8Bytes()
		{
#if defined(UTFN_TESTS) && defined(UTFN_CONVERSION_TESTS)
			// UTF-32 to UTF-8 (1-byte characters)
			static_assert(Utf32ToUtf8Bytes(0x00000041) == utf8_bytes{ 0x41, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 1-byte character { 0x00000041 }.");
			static_assert(Utf32ToUtf8Bytes(0x00000061) == utf8_bytes{ 0x61, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 1-byte character { 0x00000061 }.");
			static_assert(Utf32ToUtf8Bytes(0x00000021) == utf8_bytes{ 0x21, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 1-byte character { 0x00000021 }.");

			// UTF-32 to UTF-8 (2-byte characters)
			static_assert(Utf32ToUtf8Bytes(0x000000E9) == utf8_bytes{ 0xC3, 0xA9, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 2-byte character { 0x000000E9 }.");
			static_assert(Utf32ToUtf8Bytes(0x000000F1) == utf8_bytes{ 0xC3, 0xB1, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 2-byte character { 0x000000F1 }.");
			static_assert(Utf32ToUtf8Bytes(0x000003BB) == utf8_bytes{ 0xCE, 0xBB, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for 2-byte character { 0x000003BB }.");

			// UTF-32 to UTF-8 (3-byte characters)
			static_assert(Utf32ToUtf8Bytes(0x00000905) == utf8_bytes{ 0xE0, 0xA4, 0x85, 0x00 }, "Utf32ToUtf8Bytes failed for 3-byte character { 0x00000905 }.");
			static_assert(Utf32ToUtf8Bytes(0x00004E2D) == utf8_bytes{ 0xE4, 0xB8, 0xAD, 0x00 }, "Utf32ToUtf8Bytes failed for 3-byte character { 0x00004E2D }.");
			static_assert(Utf32ToUtf8Bytes(0x000020A4) == utf8_bytes{ 0xE2, 0x82, 0xA4, 0x00 }, "Utf32ToUtf8Bytes failed for 3-byte character { 0x000020A4 }.");

			// UTF-32 to UTF-8 (4-byte characters)
			static_assert(Utf32ToUtf8Bytes(0x0001F600) == utf8_bytes{ 0xF0, 0x9F, 0x98, 0x80 }, "Utf32ToUtf8Bytes failed for 4-byte character { 0x0001F600 }.");
			static_assert(Utf32ToUtf8Bytes(0x0001F004) == utf8_bytes{ 0xF0, 0x9F, 0x80, 0x84 }, "Utf32ToUtf8Bytes failed for 4-byte character { 0x0001F004 }.");
			static_assert(Utf32ToUtf8Bytes(0x00010348) == utf8_bytes{ 0xF0, 0x90, 0x8D, 0x88 }, "Utf32ToUtf8Bytes failed for 4-byte character { 0x00010348 }.");

			// UTF-32 to UTF-8 (null terminator)
			static_assert(Utf32ToUtf8Bytes(0x00000000) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for null terminator.");

			// UTF-32 to UTF-8 (invalid characters)
			static_assert(Utf32ToUtf8Bytes(0x00110000) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (out of range).");
			static_assert(Utf32ToUtf8Bytes(0x0000D800) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (lone surrogate).");
			static_assert(Utf32ToUtf8Bytes(0x00110000) == utf8_bytes{ 0x00, 0x00, 0x00, 0x00 }, "Utf32ToUtf8Bytes failed for invalid character (beyond valid Unicode range).");
#endif // UTFN_TESTS && UTFN_CONVERSION_TESTS
		}

		constexpr void TestUtf8BytesToUtf32()
		{
#if defined(UTFN_TESTS) && defined(UTFN_CONVERSION_TESTS)
			// UTF-8 to UTF-32 (1-byte characters)
			static_assert(Utf8BytesToUtf32({ 0x41, 0x00, 0x00, 0x00 }) == 0x00000041, "Utf8BytesToUtf32 failed for 1-byte character { 0x00000041 }.");
			static_assert(Utf8BytesToUtf32({ 0x61, 0x00, 0x00, 0x00 }) == 0x00000061, "Utf8BytesToUtf32 failed for 1-byte character { 0x00000061 }.");
			static_assert(Utf8BytesToUtf32({ 0x21, 0x00, 0x00, 0x00 }) == 0x00000021, "Utf8BytesToUtf32 failed for 1-byte character { 0x00000021 }.");
			
			// UTF-8 to UTF-32 (2-byte characters)
			static_assert(Utf8BytesToUtf32({ 0xC3, 0xA9, 0x00, 0x00 }) == 0x000000E9, "Utf8BytesToUtf32 failed for 2-byte character { 0x000000E9 }.");
			static_assert(Utf8BytesToUtf32({ 0xC3, 0xB1, 0x00, 0x00 }) == 0x000000F1, "Utf8BytesToUtf32 failed for 2-byte character { 0x000000F1 }.");
			static_assert(Utf8BytesToUtf32({ 0xCE, 0xBB, 0x00, 0x00 }) == 0x000003BB, "Utf8BytesToUtf32 failed for 2-byte character { 0x000003BB }.");
			
			static_assert(UtfImpl::Utf8::IsValidUnicodeChar<2>(0xC3, 0xA9, 0x00, 0x00), "why not?");

			// UTF-8 to UTF-32 (3-byte characters)
			static_assert(Utf8BytesToUtf32({ 0xE0, 0xA4, 0x85, 0x00 }) == 0x00000905, "Utf8BytesToUtf32 failed for 3-byte character { 0x00000905 }.");
			static_assert(Utf8BytesToUtf32({ 0xE4, 0xB8, 0xAD, 0x00 }) == 0x00004E2D, "Utf8BytesToUtf32 failed for 3-byte character { 0x00004E2D }.");
			static_assert(Utf8BytesToUtf32({ 0xE2, 0x82, 0xA4, 0x00 }) == 0x000020A4, "Utf8BytesToUtf32 failed for 3-byte character { 0x000020A4 }.");
			
			// UTF-8 to UTF-32 (4-byte characters)
			static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0x98, 0x80 }) == 0x0001F600, "Utf8BytesToUtf32 failed for 4-byte character { 0x0001F600 }.");
			static_assert(Utf8BytesToUtf32({ 0xF0, 0x9F, 0x80, 0x84 }) == 0x0001F004, "Utf8BytesToUtf32 failed for 4-byte character { 0x0001F004 }.");
			static_assert(Utf8BytesToUtf32({ 0xF0, 0x90, 0x8D, 0x88 }) == 0x00010348, "Utf8BytesToUtf32 failed for 4-byte character { 0x00010348 }.");
			
			// UTF-8 to UTF-32 (null terminator)
			static_assert(Utf8BytesToUtf32({ 0x00, 0x00, 0x00, 0x00 }) == 0x00000000, "Utf8BytesToUtf32 failed for null terminator.");
			
			// UTF-8 to UTF-32 (invalid characters)
			static_assert(Utf8BytesToUtf32({ 0xC0, 0x00, 0x00, 0x00 }) == 0x00000000, "Utf8BytesToUtf32 failed for invalid character (overlong sequence).");
			static_assert(Utf8BytesToUtf32({ 0x80, 0x00, 0x00, 0x00 }) == 0x00000000, "Utf8BytesToUtf32 failed for invalid character (lone continuation byte).");
			static_assert(Utf8BytesToUtf32({ 0xF8, 0x00, 0x00, 0x00 }) == 0x00000000, "Utf8BytesToUtf32 failed for invalid character (out of range).");
#endif // UTFN_TESTS && UTFN_CONVERSION_TESTS
		}
	}

	namespace Parsing
	{
		constexpr void TestParseUtf8CharFromStr()
		{
#if defined(UTFN_TESTS) && defined(UTFN_PARSING_TESTS)
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
#endif // UTFN_TESTS && UTFN_PARSING_TESTS
		}

		constexpr void TestParseUtf16CharFromStr()
		{
#if defined(UTFN_TESTS) && defined(UTFN_PARSING_TESTS)
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
#endif // UTFN_TESTS && UTFN_PARSING_TESTS
		}

		constexpr void TestParseUtf32CharFromStr()
		{
#if defined(UTFN_TESTS) && defined(UTFN_PARSING_TESTS)
			static_assert(UtfN::ParseUtf32CharFromStr(U"\0") == utf_cp32_t{ 0x0000 }, "ParseUtf8CharFromStr failed to parse null-terminator.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"A") == utf_cp32_t{ 0x00000041 }, "ParseUtf16CharFromStr failed to parse 1-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"a") == utf_cp32_t{ 0x00000061 }, "ParseUtf16CharFromStr failed to parse 1-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"!") == utf_cp32_t{ 0x00000021 }, "ParseUtf16CharFromStr failed to parse 1-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"é") == utf_cp32_t{ 0x000000E9 }, "ParseUtf16CharFromStr failed to parse 2-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"ñ") == utf_cp32_t{ 0x000000F1 }, "ParseUtf16CharFromStr failed to parse 2-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"λ") == utf_cp32_t{ 0x000003BB }, "ParseUtf16CharFromStr failed to parse 2-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"अ") == utf_cp32_t{ 0x00000905 }, "ParseUtf16CharFromStr failed to parse 3-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"中") == utf_cp32_t{ 0x00004E2D }, "ParseUtf16CharFromStr failed to parse 3-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"₤") == utf_cp32_t{ 0x000020A4 }, "ParseUtf16CharFromStr failed to parse 3-byte character 3.");

			static_assert(UtfN::ParseUtf32CharFromStr(U"😀") == utf_cp32_t{ 0x0001F600 }, "ParseUtf16CharFromStr failed to parse 4-byte character 1.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"🀄") == utf_cp32_t{ 0x0001F004 }, "ParseUtf16CharFromStr failed to parse 4-byte character 2.");
			static_assert(UtfN::ParseUtf32CharFromStr(U"𐍈") == utf_cp32_t{ 0x00010348 }, "ParseUtf16CharFromStr failed to parse 4-byte character 3.");
#endif // UTFN_TESTS && UTFN_PARSING_TESTS
		}
	}
}

// Restore all warnings suppressed for UTFN Tests
#if (defined(_MSC_VER))
#pragma warning (pop)
#elif (defined(__CLANG__) || defined(__GNUC__))
#pragma GCC diagnostic pop
#endif // Warnings
