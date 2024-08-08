
// Lower warning-level and turn off certain warnings for STL compilation
#if (defined(_MSC_VER))
	#pragma warning (push, 2) // Push warnings and set warn-level to 3
	#pragma warning(disable : 4365) // signed/unsigned mismatch
#elif (defined(__CLANG__) || defined(__GNUC__))
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <string>
#include <iostream>
#include <cstdint>
#include <type_traits>

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
#pragma warning(disable : 4127) // conditional expression is constant
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
	typedef char8_t utf_char8_t;
	typedef char16_t utf_char16_t;
	typedef char32_t utf_char32_t;
#elif defined(__cpp_unicode_characters)
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

			template<uint8_t BitIndex, typename ValueType>
			UTF_CONSTEXPR UTF_NODISCARD
				bool IsBitSet(ValueType Value) noexcept
			{
				constexpr auto NumBitsInValueType = sizeof(ValueType) * 0x8;
				static_assert(BitIndex < NumBitsInValueType, "Can not test out of range bit!");

				return Value & (1 << BitIndex);
			}
		}

		UTF_CONSTEXPR bool IsWCharUtf32 = sizeof(wchar_t) == 0x4;

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
			UTF_CONSTEXPR utf_char32_t Max1ByteValue = (1 <<  7) - 1; // 7  bits available
			UTF_CONSTEXPR utf_char32_t Max2ByteValue = (1 << 11) - 1; // 11 bits available
			UTF_CONSTEXPR utf_char32_t Max3ByteValue = (1 << 16) - 1; // 16 bits available
			UTF_CONSTEXPR utf_char32_t Max4ByteValue = 0x10FFFF;      // 21 bits available, but not fully used

			// Flags for follow-up bytes of multibyte utf8 character
			UTF_CONSTEXPR utf_char8_t FollowupByteMask = 0b1000'0000;
			UTF_CONSTEXPR utf_char8_t FollowupByteDataMask = 0b0011'1111;
			UTF_CONSTEXPR utf_char8_t NumDataBitsInFollowupByte = 0x6;

			// Flags for start-bytes of multibyte utf8 characters
			UTF_CONSTEXPR utf_char8_t TwoByteFlag = 0b1100'0000;
			UTF_CONSTEXPR utf_char8_t ThreeByteFlag = 0b1110'0000;
			UTF_CONSTEXPR utf_char8_t FourByteFlag = 0b1111'0000;
		}

		namespace Utf16
		{
			// Surrogate masks and offset for multibyte utf16 characters
			UTF_CONSTEXPR utf_char16_t HighSurrogateBitmask = 0xD800;
			UTF_CONSTEXPR utf_char16_t LowerSurrogateBitmask = 0xDC00;

			UTF_CONSTEXPR utf_char32_t SurrogatePairOffset = 0x10000;
		}

		namespace Utf32
		{

		}
	}

	struct utf8_bytes
	{
		utf_char8_t Codepoints[4] = { 0 };
	};

	struct utf16_pair
	{
		utf_char16_t Upper = 0;
		utf_char16_t Lower = 0;
	};

	UTF_CONSTEXPR UTF_NODISCARD
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

	UTF_CONSTEXPR UTF_NODISCARD
		int GetUtf16CharLenght(const utf_char16_t C)
	{
		if (UtfImpl::Utils::IsFlagSet(C, UtfImpl::Utf16::HighSurrogateBitmask))
			return 0x2;

		return 0x1;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf16_pair Utf32ToUtf16Pair(const utf_char32_t Codepoint) noexcept
	{
		using namespace UtfImpl;

		utf16_pair RetCharPair;

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

	UTF_CONSTEXPR UTF_NODISCARD
		utf_char32_t Utf16PairToUtf32(const utf16_pair Character) noexcept
	{
		using namespace UtfImpl;

		if (Character.Upper)
		{
			const utf_char16_t UpperCodepointWithoutSurrogate = Utils::GetWithClearedFlag(Character.Upper, Utf16::HighSurrogateBitmask);
			const utf_char16_t LowerCodepointWithoutSurrogate = Utils::GetWithClearedFlag(Character.Lower, Utf16::LowerSurrogateBitmask);

			return ((static_cast<utf_char32_t>(UpperCodepointWithoutSurrogate) << 10) | LowerCodepointWithoutSurrogate) + Utf16::SurrogatePairOffset;
		}

		return Character.Lower;
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf8_bytes Utf32ToUtf8Bytes(const utf_char32_t Codepoint) noexcept
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		utf8_bytes RetBytes;

		if (Codepoint <= Max1ByteValue)
		{
			RetBytes.Codepoints[0] = static_cast<utf_char8_t>(Codepoint);
		}
		else if (Codepoint <= Max2ByteValue)
		{
			/* Upper 3 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = TwoByteFlag;
			RetBytes.Codepoints[0] |= Codepoint >> NumDataBitsInFollowupByte; // Lower bits stored in 2nd byte

			RetBytes.Codepoints[1] |= FollowupByteMask;
			RetBytes.Codepoints[1] |= Codepoint & FollowupByteDataMask;
		}
		else if (Codepoint <= Max3ByteValue)
		{
			/* Upper 4 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = ThreeByteFlag;
			RetBytes.Codepoints[0] |= Codepoint >> (NumDataBitsInFollowupByte * 2); // Lower bits stored in 2nd and 3rd bytes

			RetBytes.Codepoints[1] = FollowupByteMask;
			RetBytes.Codepoints[1] |= (Codepoint >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 2nd byte

			RetBytes.Codepoints[2] = FollowupByteMask;
			RetBytes.Codepoints[2] |= Codepoint & FollowupByteDataMask;
		}
		else if (Codepoint <= Max4ByteValue)
		{
			/* Upper 5 bits of first byte are reserved for byte-lengh. */
			RetBytes.Codepoints[0] = FourByteFlag;
			RetBytes.Codepoints[0] |= Codepoint >> (NumDataBitsInFollowupByte * 3); // Lower bits stored in 2nd, 3rd and 4th bytes

			RetBytes.Codepoints[1] = FollowupByteMask;
			RetBytes.Codepoints[1] |= (Codepoint >> (NumDataBitsInFollowupByte * 2)) & FollowupByteDataMask; // Lower bits stored in 3rd and 4th bytes

			RetBytes.Codepoints[2] = FollowupByteMask;
			RetBytes.Codepoints[2] |= (Codepoint >> NumDataBitsInFollowupByte) & FollowupByteDataMask; // Lower bits stored in 4th byte

			RetBytes.Codepoints[3] = FollowupByteMask;
			RetBytes.Codepoints[3] |= Codepoint & FollowupByteDataMask;
		}
		else
		{
			/* Above max allowed value. Invalid codepoint. */
			return RetBytes;
		}

		return RetBytes;
	}

	UTF_CONSTEXPR UTF_NODISCARD 
		utf_char32_t Utf8BytesToUtf32(const utf8_bytes Character) noexcept
	{
		using namespace UtfImpl;
		using namespace UtfImpl::Utf8;

		/* No flag for any other byte-count is set */
		if ((Character.Codepoints[0] & 0b1000'0000) == 0)
		{
			return Character.Codepoints[0];
		}
		else if (Utils::IsFlagSet(Character.Codepoints[0], FourByteFlag))
		{
			utf_char32_t RetChar = Utils::GetWithClearedFlag(Character.Codepoints[3], FollowupByteMask);
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[2], FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[1], FollowupByteMask) << (NumDataBitsInFollowupByte * 2); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[0], FourByteFlag) << (NumDataBitsInFollowupByte * 3); // Clear the FourByteFlag and move the bits to the right position

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character.Codepoints[0], ThreeByteFlag))
		{
			utf_char32_t RetChar = Utils::GetWithClearedFlag(Character.Codepoints[2], FollowupByteMask);
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[1], FollowupByteMask) << (NumDataBitsInFollowupByte * 1); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[0], ThreeByteFlag) << (NumDataBitsInFollowupByte * 2); // Clear the ThreeByteFlag and move the bits to the right position

			return RetChar;
		}
		else if (Utils::IsFlagSet(Character.Codepoints[0], TwoByteFlag))
		{
			utf_char32_t RetChar = Utils::GetWithClearedFlag(Character.Codepoints[1], FollowupByteMask); // Clear the FollowupByteMask and move the bits to the right position
			RetChar |= Utils::GetWithClearedFlag(Character.Codepoints[0], TwoByteFlag) << NumDataBitsInFollowupByte; // Clear the TwoByteFlag and move the bits to the right position

			return RetChar;
		}
		else
		{
			/* Invalid! This is a follow up codepoint but conversion needs to start at the start-codepoint. */
			return 0;
		}
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf8_bytes Utf16PairToUtf8Bytes(const utf16_pair Character) noexcept
	{
		const utf_char32_t As32BitChar = Utf16PairToUtf32(Character);

		return Utf32ToUtf8Bytes(As32BitChar);
	}

	UTF_CONSTEXPR UTF_NODISCARD
		utf16_pair Utf8BytesToUtf16(const utf8_bytes Character) noexcept
	{
		const utf_char32_t As32BitChar = Utf8BytesToUtf32(Character);

		return Utf32ToUtf16Pair(As32BitChar);
	}

	template<typename byte_iterator_type>
	UTF_CONSTEXPR byte_iterator_type ReplaceUtf8(byte_iterator_type Begin, byte_iterator_type End, utf_char8_t CharToReplace, utf_char8_t ReplacementChar)
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
			int OldByteCount = GetUtf8CharLenght(static_cast<utf_char8_t>(*CurrentIterator)); // ToDo: Handle invalid characters
			while (OldByteCount > 0 && CurrentIterator != EndIterator)
			{
				CurrentIterator++;
				OldByteCount--;
			}

			PopulateCurrentChar();

			return *this;
		}

	public:
		inline utf8_bytes operator*() const
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
			CurrentChar.Codepoints[0] = '\0';
			CurrentChar.Codepoints[1] = '\0';
			CurrentChar.Codepoints[2] = '\0';
			CurrentChar.Codepoints[3] = '\0';

			const int CharByteCount = GetUtf8CharLenght(static_cast<utf_char8_t>(*CurrentIterator));
			auto IteratorCopy = CurrentIterator;

			for (int i = 0; i < CharByteCount && IteratorCopy != EndIterator; i++)
			{
				CurrentChar.Codepoints[i] = static_cast<utf_char8_t>(*IteratorCopy);
				IteratorCopy++;
			}
		}

	private:
		byte_iterator_type CurrentIterator;
		byte_iterator_type EndIterator;

		utf8_bytes CurrentChar;
	};

	enum class UtfEncodingType
	{
		Invalid,
		Utf8,
		Utf16,
		Utf32
	};

	template<UtfEncodingType Encoding>
	struct utf_char;

	template<>
	struct utf_char<UtfEncodingType::Utf8>
	{
		utf8_bytes Character;

		UTF_CONSTEXPR14 utf8_bytes GetAsUtf8() const { return Character; }
		UTF_CONSTEXPR14 utf16_pair GetAsUtf16() const { return Utf8BytesToUtf16(Character); }
		UTF_CONSTEXPR14 utf_char32_t GetAsUtf32() const { return Utf8BytesToUtf32(static_cast<utf8_bytes>(Character)); }

		UTF_CONSTEXPR14 utf8_bytes Get() { return Character; }

		UTF_CONSTEXPR14 UtfEncodingType GetEncoding() { return UtfEncodingType::Utf8; }
	};

	template<>
	struct utf_char<UtfEncodingType::Utf16>
	{
		utf16_pair Character;

		UTF_CONSTEXPR14 utf8_bytes GetAsUtf8() const { return Utf16PairToUtf8Bytes(Character); }
		UTF_CONSTEXPR14 utf16_pair GetAsUtf16() const { return Character; }
		UTF_CONSTEXPR14 utf_char32_t GetAsUtf32() const { return Utf16PairToUtf32(Character); }

		UTF_CONSTEXPR14 utf16_pair Get() { return Character; }

		UTF_CONSTEXPR14 UtfEncodingType GetEncoding() { return UtfEncodingType::Utf16; }
	};

	template<>
	struct utf_char<UtfEncodingType::Utf32>
	{
		utf_char32_t Character;

		UTF_CONSTEXPR14 utf8_bytes GetAsUtf8() const { return Utf32ToUtf8Bytes(Character); }
		UTF_CONSTEXPR14 utf16_pair GetAsUtf16() const { return Utf32ToUtf16Pair(Character); }
		UTF_CONSTEXPR14 utf_char32_t GetAsUtf32() const { return Character; }

		UTF_CONSTEXPR14 utf_char32_t Get() { return Character; }

		UTF_CONSTEXPR14 UtfEncodingType GetEncoding() { return UtfEncodingType::Utf32; }
	};
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

int Replace(char* Current, char* End, char* MaxCapacity)
{
	(void)Current;
	(void)End;
	(void)MaxCapacity;

	return 0;
}

int main()
{
	using namespace UtfN;

	std::string Str = reinterpret_cast<const char*>(u8"Hell 里成");
	std::cout << "\nstr: " << Str << "\n\n";

	utf_char<UtfEncodingType::Utf8> RandomChar;
	RandomChar.Character = utf_char32_t(0xFFFFF);

	auto A = RandomChar.GetAsUtf8();

	utf8_iterator<std::string::const_iterator> MyIterator(Str);

	for (utf8_bytes Char : MyIterator)
	{
		std::cout << "StrBytes = " << +Char.Codepoints[0] << ", " << +Char.Codepoints[1] << ", " << +Char.Codepoints[2] << ", " << +Char.Codepoints[3] << "\n";
	}
	std::cout << "\n" << std::endl;


	const char Array[] = u8"Hello 里成里成里成 word!";

	utf8_iterator<const char*> My2ndIterator(Array, Array + sizeof(Array));

	for (utf8_bytes Char : My2ndIterator)
	{
		std::cout << "ArrayBytes = " << +Char.Codepoints[0] << ", " << +Char.Codepoints[1] << ", " << +Char.Codepoints[2] << ", " << +Char.Codepoints[3] << "\n";
	}
	std::cout << "\n" << std::endl;


	utf16_pair Pair = Utf32ToUtf16Pair(0x1D11E);

	std::cout << "Pair.Upper: " << std::hex << static_cast<unsigned short>(Pair.Upper) << "\n";
	std::cout << "Pair.Lower: " << std::hex << static_cast<unsigned short>(Pair.Lower) << "\n";

	utf_char32_t OrignalChar = Utf16PairToUtf32(Pair);
	std::cout << "OrignalChar (Utf32 : 0x1D11E): " << std::hex << +OrignalChar << "\n";


	utf8_bytes Utf8Bytes1 = Utf32ToUtf8Bytes(0x41);
	std::cout << "1. Utf8Bytes = " << +Utf8Bytes1.Codepoints[0] << ", " << +Utf8Bytes1.Codepoints[1] << ", " << +Utf8Bytes1.Codepoints[2] << ", " << +Utf8Bytes1.Codepoints[3] << "\n";
	std::cout << "Orig (Utf32 : 0x41): " << +Utf8BytesToUtf32(Utf8Bytes1) << "\n\n";

	utf8_bytes Utf8Bytes2 = Utf32ToUtf8Bytes(0xA9);
	std::cout << "2. Utf8Bytes = " << +Utf8Bytes2.Codepoints[0] << ", " << +Utf8Bytes2.Codepoints[1] << ", " << +Utf8Bytes2.Codepoints[2] << ", " << +Utf8Bytes2.Codepoints[3] << "\n";
	std::cout << "Orig (Utf32 : 0xA9): " << +Utf8BytesToUtf32(Utf8Bytes2) << "\n\n";

	utf8_bytes Utf8Bytes3 = Utf32ToUtf8Bytes(0x20AC);
	std::cout << "3. Utf8Bytes = " << +Utf8Bytes3.Codepoints[0] << ", " << +Utf8Bytes3.Codepoints[1] << ", " << +Utf8Bytes3.Codepoints[2] << ", " << +Utf8Bytes3.Codepoints[3] << "\n";
	std::cout << "Orig (Utf32 : 0x20AC): " << +Utf8BytesToUtf32(Utf8Bytes3) << "\n\n";

	utf8_bytes Utf8Bytes4 = Utf32ToUtf8Bytes(0x1F600);
	std::cout << "4. Utf8Bytes = " << +Utf8Bytes4.Codepoints[0] << ", " << +Utf8Bytes4.Codepoints[1] << ", " << +Utf8Bytes4.Codepoints[2] << ", " << +Utf8Bytes4.Codepoints[3] << "\n";
	std::cout << "Orig (Utf32 : 0x1F600): " << +Utf8BytesToUtf32(Utf8Bytes4) << "\n\n";

	utf8_bytes Utf8Bytes5 = Utf32ToUtf8Bytes(0x110000);
	std::cout << "E. Utf8Bytes = " << +Utf8Bytes5.Codepoints[0] << ", " << +Utf8Bytes5.Codepoints[1] << ", " << +Utf8Bytes5.Codepoints[2] << ", " << +Utf8Bytes5.Codepoints[3] << "\n";
	std::cout << "Orig (Utf32 : 0x110000): " << +Utf8BytesToUtf32(Utf8Bytes5) << "\n\n";

}