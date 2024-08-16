#pragma once

// Lower warning-level and turn off certain warnings for STL compilation
#if (defined(_MSC_VER))
#pragma warning (push, 2) // Push warnings and set warn-level to 2
#pragma warning(disable : 4365) // signed/unsigned mismatch
#pragma warning(disable : 4710) // 'FunctionName' was not inlined
#pragma warning(disable : 4711) // 'FunctionName' selected for automatic inline expansion
#elif (defined(__CLANG__) || defined(__GNUC__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <string>
#include <limits>
#include <cstdint>
#include <type_traits>

#ifdef _DEBUG
#include <stdexcept>
#endif // _DEBUG


// Restore warnings-levels after STL includes
#if (defined(_MSC_VER))
#pragma warning (pop)
#elif (defined(__CLANG__) || defined(__GNUC__))
#pragma GCC diagnostic pop
#endif // Warnings



#if (defined(_MSC_VER))
#pragma warning (push)
#pragma warning (disable: 4514) // C4514 "unreferenced inline function has been removed"
#pragma warning (disable: 4820) // C4820 "'n' bytes padding added after data member '...'"
#pragma warning(disable : 4127) // C4127 conditional expression is constant
#pragma warning(disable : 5045) // C5045 Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable : 5246) // C5246 'ArrayVariable' the initialization of a subobject should be wrapped in braces
#elif (defined(__CLANG__) || defined(__GNUC__))
#endif // Warnings

#ifdef __cpp_constexpr
#define UTF_CONSTEXPR constexpr
#else
#define UTF_CONSTEXPR
#endif // __cpp_constexpr


#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201304L)
#define UTF_CONSTEXPR14 constexpr
#else 
#define UTF_CONSTEXPR14
#endif

#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201603L)
#define UTF_CONSTEXPR17 constexpr
#else 
#define UTF_CONSTEXPR17 inline
#endif

#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201907L)
#define UTF_CONSTEXPR20 constexpr
#else 
#define UTF_CONSTEXPR20 inline
#endif

#if (defined(__cpp_constexpr) && __cpp_constexpr >= 202211L)
#define UTF_CONSTEXPR23 constexpr
#else 
#define UTF_CONSTEXPR23 inline
#endif

#if (defined(__cpp_constexpr) && __cpp_constexpr >= 202406L)
#define UTF_CONSTEXPR26 constexpr
#else 
#define UTF_CONSTEXPR26 inline
#endif


#ifdef __cpp_nodiscard
#define UTF_NODISCARD [[nodiscard]]
#else
#define UTF_NODISCARD
#endif


namespace UtfN
{
#if defined(__cpp_char8_t)
	typedef char8_t utf_cp8_t;
	typedef char16_t utf_cp16_t;
	typedef char32_t utf_cp32_t;
#elif defined(__cpp_unicode_characters)
	typedef unsigned char utf_cp8_t;
	typedef char16_t utf_cp16_t;
	typedef char32_t utf_cp32_t;
#else
	typedef unsigned char utf_cp8_t;
	typedef uint16_t utf_cp16_t;
	typedef uint32_t utf_cp32_t;
#endif

	namespace UtfImpl
	{
		namespace Utils
		{
			template<typename ValueType, typename FlagType>
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsFlagSet(ValueType Value, FlagType Flag) noexcept
			{
				return (Value & Flag) == Flag;
			}

			template<typename ValueType, typename FlagType>
			UTF_CONSTEXPR UTF_NODISCARD
				ValueType GetWithClearedFlag(ValueType Value, FlagType Flag) noexcept
			{
				return static_cast<ValueType>(Value & static_cast<FlagType>(~Flag));
			}
		}

		// wchar_t is a utf16 codepoint on windows, utf32 on linux
		UTF_CONSTEXPR bool IsWCharUtf32 = sizeof(wchar_t) == 0x4;

		// Any value greater than this is not a valid Unicode symbol
		UTF_CONSTEXPR utf_cp32_t MaxValidUnicodeChar = 0x10FFFF;

		namespace Utf8
		{
			/*
			* Available bits, and max values, for n-byte utf8 characters
			*
			* 01111111 -> 1 byte  -> 7 bits
			* 11011111 -> 2 bytes -> 5 + 6 bits -> 11 bits
			* 11101111 -> 3 bytes -> 4 + 6 + 6 bits -> 16 bits
			* 11110111 -> 4 bytes -> 3 + 6 + 6 + 6 bits -> 21 bits
			*
			* 10111111 -> follow up byte
			*/
			UTF_CONSTEXPR utf_cp32_t Max1ByteValue = (1 <<  7) - 1; //  7 bits available
			UTF_CONSTEXPR utf_cp32_t Max2ByteValue = (1 << 11) - 1; // 11 bits available
			UTF_CONSTEXPR utf_cp32_t Max3ByteValue = (1 << 16) - 1; // 16 bits available
			UTF_CONSTEXPR utf_cp32_t Max4ByteValue = 0x10FFFF;      // 21 bits available, but not fully used

			// Flags for follow-up bytes of multibyte utf8 character
			UTF_CONSTEXPR utf_cp8_t FollowupByteMask = 0b1000'0000;
			UTF_CONSTEXPR utf_cp8_t FollowupByteDataMask = 0b0011'1111;
			UTF_CONSTEXPR utf_cp8_t NumDataBitsInFollowupByte = 0x6;

			// Flags for start-bytes of multibyte utf8 characters
			UTF_CONSTEXPR utf_cp8_t TwoByteFlag = 0b1100'0000;
			UTF_CONSTEXPR utf_cp8_t ThreeByteFlag = 0b1110'0000;
			UTF_CONSTEXPR utf_cp8_t FourByteFlag = 0b1111'0000;

			UTF_CONSTEXPR UTF_NODISCARD
				bool IsValidFollowupCodepoint(const utf_cp8_t Codepoint) noexcept
			{
				// Test the upper 2 bytes for the FollowupByteMask
				return (Codepoint & 0b1100'0000) == FollowupByteMask;
			}

			template<int ByteSize>
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsValidUtf8Sequence(const utf_cp8_t FirstCp, const utf_cp8_t SecondCp, const utf_cp8_t ThirdCp, const utf_cp8_t FourthCp) noexcept
			{
				switch (ByteSize)
				{
				case 1:
				{
					return SecondCp == 0 && ThirdCp == 0 && FourthCp == 0;
				}
				case 4:
				{
					const bool bIsOverlongEncoding = Utils::GetWithClearedFlag(FirstCp, ~Utf8::TwoByteFlag) != 0 && SecondCp == Utf8::FollowupByteMask;
					return !bIsOverlongEncoding && IsValidFollowupCodepoint(SecondCp) && IsValidFollowupCodepoint(ThirdCp) && IsValidFollowupCodepoint(FourthCp);
				}
				case 3:
				{
					const bool bIsOverlongEncoding = Utils::GetWithClearedFlag(FirstCp, ~Utf8::ThreeByteFlag) != 0 && SecondCp == Utf8::FollowupByteMask;
					return !bIsOverlongEncoding && IsValidFollowupCodepoint(SecondCp) && IsValidFollowupCodepoint(ThirdCp) && FourthCp == 0;
				}
				case 2:
				{
					const bool bIsOverlongEncoding = Utils::GetWithClearedFlag(FirstCp, ~Utf8::FourByteFlag) != 0 && SecondCp == Utf8::FollowupByteMask;
					return !bIsOverlongEncoding && IsValidFollowupCodepoint(SecondCp) && ThirdCp == 0 && FourthCp == 0;
				}
				default:
				{
					return false;
					break;
				}
				}
			}
		}

		namespace Utf16
		{
			// Surrogate masks and offset for multibyte utf16 characters
			UTF_CONSTEXPR utf_cp16_t HighSurrogateRangeStart = 0xD800;
			UTF_CONSTEXPR utf_cp16_t LowerSurrogateRangeStart = 0xDC00;

			UTF_CONSTEXPR utf_cp32_t SurrogatePairOffset = 0x10000;

			// Unicode range for 2byte utf16 values
			UTF_CONSTEXPR utf_cp32_t SurrogateRangeLowerBounds = 0xD800;
			UTF_CONSTEXPR utf_cp32_t SurrogateRangeUpperBounds = 0xDFFF;


			UTF_CONSTEXPR UTF_NODISCARD
				bool IsHighSurrogate(const utf_cp16_t Codepoint) noexcept
			{
				// Range [0xD800 - 0xDC00[
				return Codepoint >= HighSurrogateRangeStart && Codepoint < LowerSurrogateRangeStart;
			}
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsLowSurrogate(const utf_cp16_t Codepoint) noexcept
			{
				// Range [0xDC00 - 0xDFFF]
				return Codepoint >= LowerSurrogateRangeStart && Codepoint <= SurrogateRangeUpperBounds;
			}

			// Tests if a utf16 value is a valid Unicode character
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsValidUnicodeChar(const utf_cp16_t LowerCodepoint, const utf_cp16_t UpperCodepoint) noexcept
			{
				const bool IsValidHighSurrogate = IsHighSurrogate(UpperCodepoint);
				const bool IsValidLowSurrogate = IsLowSurrogate(LowerCodepoint);

				// Both are valid
				if (IsValidHighSurrogate)
					return IsValidLowSurrogate;

				// Neither are valid && the codepoints are not in the wrong surrogate ranges
				return !IsValidLowSurrogate && !IsHighSurrogate(LowerCodepoint) && !IsLowSurrogate(UpperCodepoint);
			}
		}

		namespace Utf32
		{
			// Tests if a utf32 value is a valid Unicode character
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsValidUnicodeChar(const utf_cp32_t Codepoint) noexcept
			{
				// Codepoints must be within the valid unicode range and must not be within the range of Surrogate-values
				return Codepoint < MaxValidUnicodeChar && (Codepoint < Utf16::SurrogateRangeLowerBounds || Codepoint > Utf16::SurrogateRangeUpperBounds);
			}
		}
	}

	struct utf8_bytes
	{
		utf_cp8_t Codepoints[4] = { 0 };
	};

	struct utf16_pair
	{
		utf_cp16_t Lower = 0;
		utf_cp16_t Upper = 0;
	};

	UTF_CONSTEXPR inline
		bool operator==(const utf8_bytes Left, const utf8_bytes Right) noexcept
	{
		return Left.Codepoints[0] == Right.Codepoints[0]
			&& Left.Codepoints[1] == Right.Codepoints[1]
			&& Left.Codepoints[2] == Right.Codepoints[2]
			&& Left.Codepoints[3] == Right.Codepoints[3];
	}
	UTF_CONSTEXPR inline
		bool operator!=(const utf8_bytes Left, const utf8_bytes Right) noexcept
	{
		return !(Left == Right);
	}

	UTF_CONSTEXPR inline
		bool operator==(const utf16_pair Left, const utf16_pair Right) noexcept
	{
		return Left.Upper == Right.Upper && Left.Lower == Right.Lower;
	}
	UTF_CONSTEXPR inline
		bool operator!=(const utf16_pair Left, const utf16_pair Right) noexcept
	{
		return !(Left == Right);
	}


	enum class UtfEncodingType
	{
		Invalid,
		Utf8,
		Utf16,
		Utf32
	};

	template<UtfEncodingType Encoding>
	struct utf_char;

	typedef utf_char<UtfEncodingType::Utf8> utf_char8;
	typedef utf_char<UtfEncodingType::Utf16> utf_char16;
	typedef utf_char<UtfEncodingType::Utf32> utf_char32;

	template<>
	struct utf_char<UtfEncodingType::Utf8>
	{
		utf8_bytes Char = { 0 };

	public:
		UTF_CONSTEXPR utf_char() = default;
		UTF_CONSTEXPR utf_char(utf_char&&) = default;
		UTF_CONSTEXPR utf_char(const utf_char&) = default;

		UTF_CONSTEXPR utf_char(utf8_bytes InChar) noexcept;

		template<typename CharType, typename = decltype(ParseUtf8CharFromStr(std::declval<const CharType*>()))>
		UTF_CONSTEXPR utf_char(const CharType* SingleCharString) noexcept;

	public:
		UTF_CONSTEXPR utf_char& operator=(utf_char&&) = default;
		UTF_CONSTEXPR utf_char& operator=(const utf_char&) = default;

		UTF_CONSTEXPR utf_char& operator=(utf8_bytes inBytse) noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD       utf_cp8_t&  operator[](const uint8_t Index);
		UTF_CONSTEXPR UTF_NODISCARD const utf_cp8_t&  operator[](const uint8_t Index) const;

		UTF_CONSTEXPR UTF_NODISCARD bool operator==(utf_char8 Other) const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD bool operator!=(utf_char8 Other) const noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD utf_char8 GetAsUtf8() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char16 GetAsUtf16() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char32 GetAsUtf32() const noexcept;

		UTF_CONSTEXPR UTF_NODISCARD utf_char8 Get() const;

		UTF_CONSTEXPR UTF_NODISCARD UtfEncodingType GetEncoding() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD uint8_t GetByteSize() const noexcept;
	};

	template<>
	struct utf_char<UtfEncodingType::Utf16>
	{
		utf16_pair Char = { 0 };

	public:
		UTF_CONSTEXPR utf_char() = default;
		UTF_CONSTEXPR utf_char(utf_char&&) = default;
		UTF_CONSTEXPR utf_char(const utf_char&) = default;

		UTF_CONSTEXPR utf_char(utf16_pair InChar) noexcept;

		template<typename CharType, typename = decltype(ParseUtf16CharFromStr(std::declval<const CharType*>()))>
		UTF_CONSTEXPR utf_char(const CharType* SingleCharString) noexcept;

	public:
		UTF_CONSTEXPR utf_char& operator=(utf_char&&) = default;
		UTF_CONSTEXPR utf_char& operator=(const utf_char&) = default;

		UTF_CONSTEXPR utf_char& operator=(utf16_pair inBytse) noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD bool operator==(utf_char16 Other) const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD bool operator!=(utf_char16 Other) const noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD utf_char8 GetAsUtf8() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char16 GetAsUtf16() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char32 GetAsUtf32() const noexcept;

		UTF_CONSTEXPR UTF_NODISCARD utf_char16 Get() const noexcept;

		UTF_CONSTEXPR UTF_NODISCARD UtfEncodingType GetEncoding() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD uint8_t GetByteSize() const noexcept;
	};

	template<>
	struct utf_char<UtfEncodingType::Utf32>
	{
		utf_cp32_t Char = { 0 };

	public:
		UTF_CONSTEXPR utf_char() = default;
		UTF_CONSTEXPR utf_char(utf_char&&) = default;
		UTF_CONSTEXPR utf_char(const utf_char&) = default;

		UTF_CONSTEXPR utf_char(utf_cp32_t InChar) noexcept;

		template<typename CharType, typename = decltype(ParseUtf32CharFromStr(std::declval<const CharType*>()))>
		UTF_CONSTEXPR utf_char(const CharType* SingleCharString) noexcept;

	public:
		UTF_CONSTEXPR utf_char& operator=(utf_char&&) = default;
		UTF_CONSTEXPR utf_char& operator=(const utf_char&) = default;

		UTF_CONSTEXPR utf_char& operator=(utf_cp32_t inBytse) noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD bool operator==(utf_char32 Other) const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD bool operator!=(utf_char32 Other) const noexcept;

	public:
		UTF_CONSTEXPR UTF_NODISCARD utf_char8 GetAsUtf8() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char16 GetAsUtf16() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD utf_char32 GetAsUtf32() const noexcept;

		UTF_CONSTEXPR UTF_NODISCARD utf_char32 Get() const noexcept;

		UTF_CONSTEXPR UTF_NODISCARD UtfEncodingType GetEncoding() const noexcept;
		UTF_CONSTEXPR UTF_NODISCARD uint8_t GetByteSize() const noexcept;
	};

	UTF_CONSTEXPR UTF_NODISCARD
		uint8_t GetUtf8CharLenght(const utf_cp8_t C) noexcept
	{
		using namespace UtfImpl;

		/* No flag for any other byte-count is set */
		if ((C & 0b1000'0000) == 0)
		{
			return 0x1;
		}
		else if (Utils::IsFlagSet(C, Utf8::FourByteFlag))
		{
			return 0x4;
		}
		else if (Utils::IsFlagSet(C, Utf8::ThreeByteFlag))
		{
			return 0x3;
		}
		else if (Utils::IsFlagSet(C, Utf8::TwoByteFlag))
		{
			return 0x2;
		}
		else
		{
			/* Invalid! This is a follow up codepoint but conversion needs to start at the start-codepoint. */
			return 0x0;
		}
	}

	UTF_CONSTEXPR UTF_NODISCARD
		uint8_t GetUtf16CharLenght(const utf_cp16_t UpperCodepoint) noexcept
	{
		if (UtfImpl::Utf16::IsHighSurrogate(UpperCodepoint))
			return 0x2;

		return 0x1;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char16 Utf32ToUtf16Pair(const utf_char32 Character) noexcept
	{
		using namespace UtfImpl;

		if (!Utf32::IsValidUnicodeChar(Character.Char))
			return utf16_pair{};

		utf16_pair RetCharPair;

		if (Character.Char > std::numeric_limits<utf_cp16_t>::max())
		{
			const utf_cp32_t PreparedCodepoint = Character.Char - Utf16::SurrogatePairOffset;

			RetCharPair.Upper = (PreparedCodepoint >> 10) & 0b1111111111;
			RetCharPair.Lower = PreparedCodepoint & 0b1111111111;

			// Surrogate-pair starting ranges for higher/lower surrogates
			RetCharPair.Upper += Utf16::HighSurrogateRangeStart;
			RetCharPair.Lower += Utf16::LowerSurrogateRangeStart;

			return RetCharPair;
		}

		RetCharPair.Lower = static_cast<utf_cp16_t>(Character.Char);

		return RetCharPair;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32 Utf16PairToUtf32(const utf_char16 Character) noexcept
	{
		using namespace UtfImpl;

		// The surrogate-values are not valid Unicode codepoints
		if (!Utf16::IsValidUnicodeChar(Character.Char.Lower, Character.Char.Upper))
			return utf_cp32_t{ 0 };

		if (Character.Char.Upper)
		{
			// Move the characters back from the surrogate range to the normal range
			const utf_cp16_t UpperCodepointWithoutSurrogate = static_cast<utf_cp16_t>(Character.Char.Upper - Utf16::HighSurrogateRangeStart);
			const utf_cp16_t LowerCodepointWithoutSurrogate = static_cast<utf_cp16_t>(Character.Char.Lower - Utf16::LowerSurrogateRangeStart);

			return ((static_cast<utf_cp32_t>(UpperCodepointWithoutSurrogate) << 10) | LowerCodepointWithoutSurrogate) + Utf16::SurrogatePairOffset;
		}

		return Character.Char.Lower;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 Utf32ToUtf8Bytes(const utf_char32 Character) noexcept
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		if (!Utf32::IsValidUnicodeChar(Character.Char))
			return utf_char8{};

		utf8_bytes RetBytes;

		if (Character.Char <= Max1ByteValue)
		{
			RetBytes.Codepoints[0] = static_cast<utf_cp8_t>(Character.Char);
		}
		else if (Character.Char <= Max2ByteValue)
		{
			/* Upper 3 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = TwoByteFlag;
			RetBytes.Codepoints[0] |= Character.Char >> NumDataBitsInFollowupByte; // Lower bits stored in 2nd byte

			RetBytes.Codepoints[1] |= FollowupByteMask;
			RetBytes.Codepoints[1] |= Character.Char & FollowupByteDataMask;
		}
		else if (Character.Char <= Max3ByteValue)
		{
			/* Upper 4 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = ThreeByteFlag;
			RetBytes.Codepoints[0] |= Character.Char >> (NumDataBitsInFollowupByte * 2); // Lower bits stored in 2nd and 3rd bytes

			RetBytes.Codepoints[1] = FollowupByteMask;
			RetBytes.Codepoints[1] |= (Character.Char >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 2nd byte

			RetBytes.Codepoints[2] = FollowupByteMask;
			RetBytes.Codepoints[2] |= Character.Char & FollowupByteDataMask;
		}
		else if (Character.Char <= Max4ByteValue)
		{
			/* Upper 5 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = FourByteFlag;
			RetBytes.Codepoints[0] |= Character.Char >> (NumDataBitsInFollowupByte * 3); // Lower bits stored in 2nd, 3rd and 4th bytes

			RetBytes.Codepoints[1] = FollowupByteMask;
			RetBytes.Codepoints[1] |= (Character.Char >> (NumDataBitsInFollowupByte * 2)) & FollowupByteDataMask; // Lower bits stored in 3rd and 4th bytes

			RetBytes.Codepoints[2] = FollowupByteMask;
			RetBytes.Codepoints[2] |= (Character.Char >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 4th byte

			RetBytes.Codepoints[3] = FollowupByteMask;
			RetBytes.Codepoints[3] |= Character.Char & FollowupByteDataMask;
		}
		else
		{
			/* Above max allowed value. Invalid codepoint. */
			return RetBytes;
		}

		return RetBytes;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_cp32_t Utf8BytesToUtf32(const utf_char8 Character) noexcept
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		/* No flag for any other byte-count is set */
		if ((Character[0] & 0b1000'0000) == 0)
		{
			if (!Utf8::IsValidUtf8Sequence<1>(Character[0], Character[1], Character[2], Character[3])) // Verifies encoding
				return utf_cp32_t{ 0 };

			return Character[0];
		}
		else if (Utils::IsFlagSet(Character[0], FourByteFlag))
		{
			utf_cp32_t RetChar = Utils::GetWithClearedFlag(Character[3], FollowupByteMask);
			RetChar |= Utils::GetWithClearedFlag(Character[2], FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character[1], FollowupByteMask) << (NumDataBitsInFollowupByte * 2); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character[0], FourByteFlag) << (NumDataBitsInFollowupByte * 3); // Clear the FourByteFlag and move the bits to the right position

			if (!Utf8::IsValidUtf8Sequence<4>(Character[0], Character[1], Character[2], Character[3])  // Verifies encoding
				|| !Utf32::IsValidUnicodeChar(RetChar)) // Verifies ranges
				return utf_cp32_t{ 0 };

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character[0], ThreeByteFlag))
		{
			utf_cp32_t RetChar = Utils::GetWithClearedFlag(Character[2], FollowupByteMask);
			RetChar |= Utils::GetWithClearedFlag(Character[1], FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character[0], ThreeByteFlag) << (NumDataBitsInFollowupByte * 2); // Clear the ThreeByteFlag and move the bits to the right position

			if (!Utf8::IsValidUtf8Sequence<3>(Character[0], Character[1], Character[2], Character[3]) // Verifies encoding
				|| !Utf32::IsValidUnicodeChar(RetChar)) // Verifies ranges
				return utf_cp32_t{ 0 };

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character[0], TwoByteFlag))
		{
			utf_cp32_t RetChar = Utils::GetWithClearedFlag(Character[1], FollowupByteMask); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character[0], TwoByteFlag) << NumDataBitsInFollowupByte; // Clear the TwoByteFlag and move the bits to the right position

			if (!Utf8::IsValidUtf8Sequence<2>(Character[0], Character[1], Character[2], Character[3]) // Verifies encoding
				|| !Utf32::IsValidUnicodeChar(RetChar)) // Verifies ranges
				return utf_cp32_t{ 0 };

			return RetChar;
		}
		else
		{
			/* Invalid! This is a follow up codepoint but conversion needs to start at the start-codepoint. */
			return 0;
		}
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 Utf16PairToUtf8Bytes(const utf_char16 Character) noexcept
	{
		const utf_char32 As32BitChar = Utf16PairToUtf32(Character);

		return Utf32ToUtf8Bytes(As32BitChar);
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char16 Utf8BytesToUtf16(const utf_char8 Character) noexcept
	{
		const utf_char32 As32BitChar = Utf8BytesToUtf32(Character);

		return Utf32ToUtf16Pair(As32BitChar);
	}

	template<typename byte_iterator_type>
	UTF_CONSTEXPR byte_iterator_type ReplaceUtf8(byte_iterator_type Begin, byte_iterator_type End, utf_cp8_t CharToReplace, utf_cp8_t ReplacementChar)
	{
		using namespace UtfImpl;

		if (Begin == End)
			return End;

		const auto ToReplaceSize = GetUtf8CharLenght(CharToReplace);
		const auto ReplacementSize = GetUtf8CharLenght(ReplacementChar);

		if (ToReplaceSize == ReplacementSize) // Trivial replacement
		{

		}
		else if (ToReplaceSize < ReplacementSize) // 
		{

		}
		else /* if (ToReplaceSize > ReplacementSize) */ // Replace and move following bytes back
		{

		}
	}

	template<typename char_type>
	class Utf8Iterator
	{
	};

	template<typename byte_iterator_type>
	class utf8_iterator
	{
	public:
		UTF_CONSTEXPR utf8_iterator(byte_iterator_type Begin, byte_iterator_type End)
			: CurrentIterator(Begin), EndIterator(End)
		{
			PopulateCurrentChar();
		}

		template<typename container_type,
			typename = decltype(std::begin(std::declval<container_type>())),
			typename = decltype(std::end(std::declval<container_type>()))
		>
			explicit UTF_CONSTEXPR utf8_iterator(container_type& Container)
			: CurrentIterator(std::begin(Container)), EndIterator(std::end(Container))
		{
			PopulateCurrentChar();
		}

	public:
		inline utf8_iterator operator++()
		{
			// Advance over the bytes of the old character
			int OldByteCount = CurrentChar.GetByteSize();

			if (OldByteCount == 0)
				CurrentIterator = EndIterator;

			while (OldByteCount > 0 && CurrentIterator != EndIterator)
			{
				CurrentIterator++;
				OldByteCount--;
			}

			PopulateCurrentChar();

			return *this;
		}

	public:
		inline utf_char8 operator*() const
		{
			return CurrentChar;
		}

		inline bool operator==(const utf8_iterator& Other) const
		{
			return CurrentIterator == Other.CurrentIterator;
		}
		inline bool operator!=(const utf8_iterator& Other) const
		{
			return CurrentIterator != Other.CurrentIterator;
		}

	public:
		utf8_iterator begin()
		{
			return *this;
		}

		utf8_iterator end()
		{
			return utf8_iterator(EndIterator, EndIterator);
		}

	private:
		inline void PopulateCurrentChar()
		{
			if (CurrentIterator == EndIterator)
				return;

			// Reset the bytes of the character
			CurrentChar = "\0";

			const int CharByteCount = GetUtf8CharLenght(static_cast<utf_cp8_t>(*CurrentIterator));
			auto IteratorCopy = CurrentIterator;

			for (int i = 0; i < CharByteCount && IteratorCopy != EndIterator; i++)
			{
				CurrentChar[i] = static_cast<utf_cp8_t>(*IteratorCopy);
				IteratorCopy++;
			}
		}

	private:
		byte_iterator_type CurrentIterator;
		byte_iterator_type EndIterator;

		utf_char8 CurrentChar;
	};


	template<typename CodepointType,
		typename std::enable_if<sizeof(CodepointType) == 0x1 && std::is_integral<CodepointType>::value, int>::type = 0>
	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 ParseUtf8CharFromStr(const CodepointType* Str)
	{
		if (!Str)
			return utf8_bytes{};

		const utf_cp8_t FirstCodepoint = static_cast<utf_cp8_t>(Str[0]);
		const auto CharLength = GetUtf8CharLenght(FirstCodepoint);

		if (CharLength == 0)
			return utf8_bytes{};

		utf8_bytes RetChar;
		RetChar.Codepoints[0] = FirstCodepoint;

		for (int i = 1; i < CharLength; i++)
		{
			const utf_cp8_t CurrentCodepoint = static_cast<utf_cp8_t>(Str[i]);

			// Filters the null-terminator and other invalid followup bytes
			if (!UtfImpl::Utf8::IsValidFollowupCodepoint(CurrentCodepoint))
				return utf8_bytes{};

			RetChar.Codepoints[i] = CurrentCodepoint;
		}

		return RetChar;
	}

	template<typename CodepointType,
		typename std::enable_if<sizeof(CodepointType) == 0x2 && std::is_integral<CodepointType>::value, int>::type = 0>
	UTF_CONSTEXPR UTF_NODISCARD
		utf_char16 ParseUtf16CharFromStr(const CodepointType* Str)
	{
		if (!Str)
			return utf_char16{};

		const utf_cp16_t FirstCodepoint = static_cast<utf_cp16_t>(Str[0]);

		if (GetUtf16CharLenght(FirstCodepoint) == 1)
			return utf16_pair{ FirstCodepoint };

		return utf16_pair{ FirstCodepoint,  static_cast<utf_cp16_t>(Str[1]) };
	}

	template<typename CodepointType,
		typename std::enable_if<sizeof(CodepointType) == 0x4 && std::is_integral<CodepointType>::value, int>::type = 0>
	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32 ParseUtf32CharFromStr(const CodepointType* Str)
	{
		if (!Str)
			return utf_char32{};

		return static_cast<utf_cp32_t>(Str[0]);
	}



	// utf_char spezialization-implementation for Utf8
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf8>::utf_char(utf8_bytes InChar) noexcept
		: Char(InChar)
	{
	}

	template<typename CharType, typename>
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf8>::utf_char(const CharType* SingleCharString) noexcept
		: utf_char<UtfEncodingType::Utf8>(ParseUtf8CharFromStr(SingleCharString))
	{
	}

	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf8>& utf_char<UtfEncodingType::Utf8>::operator=(utf8_bytes inBytse) noexcept
	{
		Char.Codepoints[0] = inBytse.Codepoints[0];
		Char.Codepoints[1] = inBytse.Codepoints[1];
		Char.Codepoints[2] = inBytse.Codepoints[2];
		Char.Codepoints[3] = inBytse.Codepoints[3];

		return *this;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf8>::operator==(utf_char8 Other) const noexcept
	{
		return Char == Other.Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_cp8_t& utf_char<UtfEncodingType::Utf8>::operator[](const uint8_t Index)
	{
#ifdef _DEBUG
		if (Index >= 0x4)
			throw std::out_of_range("Index was greater than 4!");
#endif // _DEBUG
		return Char.Codepoints[Index];
	}

	UTF_CONSTEXPR UTF_NODISCARD
		const utf_cp8_t& utf_char<UtfEncodingType::Utf8>::operator[](const uint8_t Index) const
	{
#ifdef _DEBUG
		if (Index >= 0x4)
			throw std::out_of_range("Index was greater than 4!");
#endif // _DEBUG
		return Char.Codepoints[Index];
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf8>::operator!=(utf_char8 Other) const noexcept
	{
		return Char != Other.Char;
	}
	
	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 utf_char<UtfEncodingType::Utf8>::GetAsUtf8() const noexcept
	{
		return Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char16 utf_char<UtfEncodingType::Utf8>::GetAsUtf16() const noexcept
	{
		return Utf8BytesToUtf16(Char);
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32 utf_char<UtfEncodingType::Utf8>::GetAsUtf32() const noexcept
	{
		return Utf8BytesToUtf32(static_cast<utf8_bytes>(Char));
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 utf_char<UtfEncodingType::Utf8>::Get() const
	{
		return Char;
	}
	
	UTF_CONSTEXPR UTF_NODISCARD
		UtfEncodingType utf_char<UtfEncodingType::Utf8>::GetEncoding() const noexcept
	{
		return UtfEncodingType::Utf8;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		uint8_t utf_char<UtfEncodingType::Utf8>::GetByteSize() const noexcept
	{
		return GetUtf8CharLenght(Char.Codepoints[0]);
	}



	// utf_char spezialization-implementation for Utf8
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf16>::utf_char(utf16_pair InChar) noexcept
		: Char(InChar)
	{
	}

	template<typename CharType, typename>
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf16>::utf_char(const CharType* SingleCharString) noexcept
		: utf_char<UtfEncodingType::Utf16>(ParseUtf16CharFromStr(SingleCharString))
	{
	}

	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf16>& utf_char<UtfEncodingType::Utf16>::operator=(utf16_pair inBytse) noexcept
	{
		Char.Upper = inBytse.Upper;
		Char.Lower = inBytse.Lower;

		return *this;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf16>::operator==(utf_char16 Other) const noexcept
	{
		return Char == Other.Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf16>::operator!=(utf_char16 Other) const noexcept
	{
		return Char != Other.Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_char8 utf_char<UtfEncodingType::Utf16>::GetAsUtf8() const noexcept
	{
		return Utf16PairToUtf8Bytes(Char);
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_char16 utf_char<UtfEncodingType::Utf16>::GetAsUtf16() const noexcept
	{
		return Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_char32 utf_char<UtfEncodingType::Utf16>::GetAsUtf32() const noexcept
	{
		return Utf16PairToUtf32(Char);
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_char16 utf_char<UtfEncodingType::Utf16>::Get() const noexcept
	{
		return Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		UtfEncodingType utf_char<UtfEncodingType::Utf16>::GetEncoding() const noexcept
	{
		return UtfEncodingType::Utf16;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		uint8_t utf_char<UtfEncodingType::Utf16>::GetByteSize() const noexcept
	{
		return GetUtf16CharLenght(Char.Upper);
	}



	// utf_char spezialization-implementation for Utf32
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf32>::utf_char(utf_cp32_t InChar) noexcept
		: Char(InChar)
	{
	}

	template<typename CharType, typename>
	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf32>::utf_char(const CharType* SingleCharString) noexcept
		: Char(*SingleCharString)
	{
	}

	UTF_CONSTEXPR utf_char<UtfEncodingType::Utf32>& utf_char<UtfEncodingType::Utf32>::operator=(utf_cp32_t inBytse) noexcept
	{
		Char = inBytse;
		return *this;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf32>::operator==(utf_char32 Other) const noexcept
	{
		return Char == Other.Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		bool utf_char<UtfEncodingType::Utf32>::operator!=(utf_char32 Other) const noexcept
	{
		return Char != Other.Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char8 utf_char<UtfEncodingType::Utf32>::GetAsUtf8() const noexcept
	{
		return Utf32ToUtf8Bytes(Char);
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char16 utf_char<UtfEncodingType::Utf32>::GetAsUtf16() const noexcept
	{
		return Utf32ToUtf16Pair(Char);
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32 utf_char<UtfEncodingType::Utf32>::GetAsUtf32() const noexcept
	{
		return Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32 utf_char<UtfEncodingType::Utf32>::Get() const noexcept
	{
		return Char;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		UtfEncodingType utf_char<UtfEncodingType::Utf32>::GetEncoding() const noexcept
	{
		return UtfEncodingType::Utf32;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		uint8_t utf_char<UtfEncodingType::Utf32>::GetByteSize() const noexcept
	{
		return 0x4;
	}
}

#undef UTF_CONSTEXPR
#undef UTF_CONSTEXPR14
#undef UTF_CONSTEXPR17
#undef UTF_CONSTEXPR20
#undef UTF_CONSTEXPR23
#undef UTF_CONSTEXPR26


// Restore all warnings suppressed for the UTF-N implementation
#if (defined(_MSC_VER))
#pragma warning (pop)
#elif (defined(__CLANG__) || defined(__GNUC__))
#pragma GCC diagnostic pop
#endif // Warnings