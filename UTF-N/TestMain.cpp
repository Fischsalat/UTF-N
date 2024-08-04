#pragma once
#include <string>
#include <iostream>
#include <bitset>
#include <cstdint>

namespace UtfN
{
#if (__cplusplus >= 201811L)
	typedef char8_t utf_char8_t;
	typedef char16_t utf_char16_t;
	typedef char32_t utf_char32_t;
#elif(__cplusplus >= 200704L)
	typedef unsigned char utf_char8_t;
	typedef char16_t utf_char16_t;
	typedef char32_t utf_char32_t;
#else
	typedef unsigned char utf_char8_t;
	typedef uint16_t utf_char16_t;
	typedef uint32_t utf_char32_t;
#endif

	namespace UtfImpl
	{
		namespace Utils
		{
			template<typename ValueType, typename FlagType>
			constexpr bool IsFlagSet(ValueType Value, FlagType Flag)
			{
				return (Value & Flag) == Flag;
			}

			template<uint8_t BitIndex, typename ValueType>
			constexpr bool IsBitSet(ValueType Value)
			{
				constexpr auto NumBitsInValueType = sizeof(ValueType) * 0x8;
				static_assert(BitIndex < NumBitsInValueType, "Can not test out of range bit!");

				return Value & (1 << BitIndex);
			}
		}

		namespace Utf8
		{
			/*
			* 01111111 -> 1 byte  -> 7 bits
			* 11011111 -> 2 bytes -> 5 + 6 bits -> 11 bits
			* 11101111 -> 3 bytes -> 4 + 6 + 6 bits -> 16 bits
			* 11110111 -> 4 bytes -> 3 + 6 + 6 + 6 bits -> 21 bits
			*
			* 10111111 -> follow up byte
			*/
			constexpr utf_char32_t Max1ByteValue = (1 <<  7) - 1; // 7  bits available
			constexpr utf_char32_t Max2ByteValue = (1 << 11) - 1; // 11 bits available
			constexpr utf_char32_t Max3ByteValue = (1 << 16) - 1; // 16 bits available
			constexpr utf_char32_t Max4ByteValue = 0x10FFFF;      // 21 bits available, but not fully used

			constexpr utf_char8_t FollowupByteMask = 0b1000'0000;
			constexpr utf_char8_t FollowupByteDataMask = 0b0011'1111;
			constexpr utf_char8_t NumDataBitsInFollowupByte = 0x6;

			constexpr utf_char8_t OneByteFlag = 0b0000'0000;
			constexpr utf_char8_t TwoByteFlag = 0b1100'0000;
			constexpr utf_char8_t ThreeByteFlag = 0b1110'0000;
			constexpr utf_char8_t FourByteFlag = 0b1111'0000;
		}

		namespace Utf16
		{
			constexpr utf_char16_t HighSurrogateBitmask = 0xD800;
			constexpr utf_char16_t LowerSurrogateBitmask = 0xDC00;

			constexpr utf_char32_t SurrogatePairOffset = 0x10000;
		}

		namespace Utf32
		{

		}
	}

	int GetUtf8CharLenght(const utf_char8_t C)
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

	struct utf_8_bytes
	{
		utf_char8_t Chars[4] = { 0 };
	};

	struct utf_16_pair
	{
		utf_char16_t Upper = 0;
		utf_char16_t Lower = 0;
	};

	utf_16_pair Utf32ToUtf16Pair(const utf_char32_t Codepoint)
	{
		using namespace UtfImpl;

		utf_16_pair RetCharPair;

		if (Codepoint > std::numeric_limits<utf_char16_t>::max())
		{
			const utf_char32_t PreparedCodepoint = Codepoint - Utf16::SurrogatePairOffset;

			RetCharPair.Upper = (PreparedCodepoint >> 10) & 0b1111111111;
			RetCharPair.Lower = PreparedCodepoint & 0b1111111111;

			RetCharPair.Upper |= Utf16::HighSurrogateBitmask;
			RetCharPair.Lower |= Utf16::LowerSurrogateBitmask;

			return RetCharPair;
		}

		RetCharPair.Lower = static_cast<utf_char16_t>(Codepoint);

		return RetCharPair;
	}

	utf_char32_t Utf16PairToUtf32(const utf_16_pair Character)
	{
		using namespace UtfImpl;

		if (Character.Upper)
		{
			const utf_char16_t UpperCodepointWithoutSurrogate = Character.Upper & ~Utf16::HighSurrogateBitmask;
			const utf_char16_t LowerCodepointWithoutSurrogate = Character.Lower & ~Utf16::LowerSurrogateBitmask;

			return ((static_cast<utf_char32_t>(UpperCodepointWithoutSurrogate) << 10) | LowerCodepointWithoutSurrogate) + Utf16::SurrogatePairOffset;
		}

		return Character.Lower;
	}

	utf_8_bytes Utf32ToUtf8Bytes(const utf_char32_t Codepoint)
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		utf_8_bytes RetBytes;

		if (Codepoint <= Max1ByteValue)
		{
			RetBytes.Chars[0] = static_cast<utf_char8_t>(Codepoint);
		}
		else if (Codepoint <= Max2ByteValue)
		{
			/* Upper 3 bits of first byte are reserved for byte-lengh. */
			RetBytes.Chars[0] = TwoByteFlag;
			RetBytes.Chars[0] |= Codepoint >> NumDataBitsInFollowupByte; // Lower bits stored in 2nd byte

			RetBytes.Chars[1] |= FollowupByteMask;
			RetBytes.Chars[1] |= Codepoint & FollowupByteDataMask;
		}
		else if (Codepoint <= Max3ByteValue)
		{
			/* Upper 4 bits of first byte are reserved for byte-lengh. */
			RetBytes.Chars[0] = ThreeByteFlag;
			RetBytes.Chars[0] |= Codepoint >> (NumDataBitsInFollowupByte * 2); // Lower bits stored in 2nd and 3rd bytes

			RetBytes.Chars[1] = FollowupByteMask;
			RetBytes.Chars[1] |= (Codepoint >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 2nd byte

			RetBytes.Chars[2] = FollowupByteMask;
			RetBytes.Chars[2] |= Codepoint & FollowupByteDataMask;
		}
		else if (Codepoint <= Max4ByteValue)
		{
			/* Upper 5 bits of first byte are reserved for byte-lengh. */
			RetBytes.Chars[0] = FourByteFlag;
			RetBytes.Chars[0] |= Codepoint >> (NumDataBitsInFollowupByte * 3); // Lower bits stored in 2nd, 3rd and 4th bytes

			RetBytes.Chars[1] = FollowupByteMask;
			RetBytes.Chars[1] |= (Codepoint >> (NumDataBitsInFollowupByte * 2)) & FollowupByteDataMask; // Lower bits stored in 3rd and 4th bytes

			RetBytes.Chars[2] = FollowupByteMask;
			RetBytes.Chars[2] |= (Codepoint >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 4th byte

			RetBytes.Chars[3] = FollowupByteMask;
			RetBytes.Chars[3] |= Codepoint & FollowupByteDataMask;
		}
		else
		{
			/* Above max allowed value. Invalid codepoint. */
			return RetBytes;
		}

		return RetBytes;
	}

	utf_char32_t Utf8BytesToUtf32(const utf_8_bytes Character)
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		/* No flag for any other byte-count is set */
		if ((Character.Chars[0] & 0b1000'0000) == 0)
		{
			return Character.Chars[0];
		}
		else if (Utils::IsFlagSet(Character.Chars[0], FourByteFlag))
		{
			utf_char32_t RetChar = Character.Chars[3] & ~FollowupByteMask;
			RetChar |= (Character.Chars[2] & ~FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= (Character.Chars[1] & ~FollowupByteMask) << (NumDataBitsInFollowupByte * 2); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= (Character.Chars[0] & ~FourByteFlag) << (NumDataBitsInFollowupByte * 3); // Clear the FourByteFlag and move the bits to the right position

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character.Chars[0], ThreeByteFlag))
		{
			utf_char32_t RetChar = Character.Chars[2] & ~FollowupByteMask;
			RetChar |= (Character.Chars[1] & ~FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= (Character.Chars[0] & ~ThreeByteFlag) << (NumDataBitsInFollowupByte * 2); // Clear the ThreeByteFlag and move the bits to the right position

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character.Chars[0], TwoByteFlag))
		{
			utf_char32_t RetChar = Character.Chars[1] & ~FollowupByteMask; // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= (Character.Chars[0] & ~TwoByteFlag) << NumDataBitsInFollowupByte; // Clear the TwoByteFlag and move the bits to the right position

			return RetChar;
		}
		else
		{
			/* Invalid! This is a follow up codepoint but conversion needs to start at the start-codepoint. */
			return 0;
		}
	}

	utf_8_bytes Utf16PairToUtf8Bytes(const utf_16_pair Character)
	{
		const utf_char32_t As32BitChar = Utf16PairToUtf32(Character);

		return Utf32ToUtf8Bytes(As32BitChar);
	}

	utf_16_pair Utf8BytesToUtf16(const utf_8_bytes Character)
	{
		const utf_char32_t As32BitChar = Utf8BytesToUtf32(Character);

		return Utf32ToUtf16Pair(As32BitChar);
	}
}



int main()
{
	using namespace UtfN;

	std::string Str = reinterpret_cast<const char*>(u8"Hell 里成");

	std::cout << "\nstr: " << Str << "\n\n";

	utf_16_pair Pair = Utf32ToUtf16Pair(0x1D11E);

	std::cout << "Pair.Upper: " << std::hex << Pair.Upper << "\n";
	std::cout << "Pair.Lower: " << std::hex << Pair.Lower << "\n";

	utf_char32_t OrignalChar = Utf16PairToUtf32(Pair);
	std::cout << "OrignalChar (Utf32 : 0x1D11E): " << std::hex << +OrignalChar << "\n";


	utf_8_bytes Utf8Bytes1 = Utf32ToUtf8Bytes(0x41);
	std::cout << "1. Utf8Bytes = " << +Utf8Bytes1.Chars[0] << ", " << +Utf8Bytes1.Chars[1] << ", " << +Utf8Bytes1.Chars[2] << ", " << +Utf8Bytes1.Chars[3] << "\n";
	std::cout << "Orig (Utf32 : 0x41): " << +Utf8BytesToUtf32(Utf8Bytes1) << "\n\n";

	utf_8_bytes Utf8Bytes2 = Utf32ToUtf8Bytes(0xA9);
	std::cout << "2. Utf8Bytes = " << +Utf8Bytes2.Chars[0] << ", " << +Utf8Bytes2.Chars[1] << ", " << +Utf8Bytes2.Chars[2] << ", " << +Utf8Bytes2.Chars[3] << "\n";
	std::cout << "Orig (Utf32 : 0xA9): " << +Utf8BytesToUtf32(Utf8Bytes2) << "\n\n";

	utf_8_bytes Utf8Bytes3 = Utf32ToUtf8Bytes(0x20AC);
	std::cout << "3. Utf8Bytes = " << +Utf8Bytes3.Chars[0] << ", " << +Utf8Bytes3.Chars[1] << ", " << +Utf8Bytes3.Chars[2] << ", " << +Utf8Bytes3.Chars[3] << "\n";
	std::cout << "Orig (Utf32 : 0x20AC): " << +Utf8BytesToUtf32(Utf8Bytes3) << "\n\n";

	utf_8_bytes Utf8Bytes4 = Utf32ToUtf8Bytes(0x1F600);
	std::cout << "4. Utf8Bytes = " << +Utf8Bytes4.Chars[0] << ", " << +Utf8Bytes4.Chars[1] << ", " << +Utf8Bytes4.Chars[2] << ", " << +Utf8Bytes4.Chars[3] << "\n";
	std::cout << "Orig (Utf32 : 0x1F600): " << +Utf8BytesToUtf32(Utf8Bytes4) << "\n\n";

	utf_8_bytes Utf8Bytes5 = Utf32ToUtf8Bytes(0x110000);
	std::cout << "E. Utf8Bytes = " << +Utf8Bytes5.Chars[0] << ", " << +Utf8Bytes5.Chars[1] << ", " << +Utf8Bytes5.Chars[2] << ", " << +Utf8Bytes5.Chars[3] << "\n";
	std::cout << "Orig (Utf32 : 0x110000): " << +Utf8BytesToUtf32(Utf8Bytes5) << "\n\n";

}