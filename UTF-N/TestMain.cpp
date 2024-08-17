#include "UtfN.hpp"
#include "UtfNTests.hpp"

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

#include <iostream>

// Restore warnings-levels after STL includes
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
	std::cout << std::hex;

	using namespace UtfN;

	UtfNTests::Parsing::TestParseUtf8CharFromStr();
	UtfNTests::Parsing::TestParseUtf16CharFromStr();

	std::string Str = reinterpret_cast<const char*>(u8"Hell 里成");
	std::cout << "\nstr: " << Str << "\n\n";

	//constexpr utf_char<UtfEncodingType::Utf8> RandomChar = utf8_bytes{ 20, 120, 33, 00 };

	//constexpr auto FirstByte = RandomChar[0];

	//auto A = RandomChar.GetAsUtf8();

	utf8_iterator<std::string::const_iterator> MyIterator(Str);

	for (utf_char8 Char : MyIterator)
	{
		std::cout << "StrBytes = " << +Char[0] << ", " << +Char[1] << ", " << +Char[2] << ", " << +Char[3] << "\n";
	}
	std::cout << "\n" << std::endl;


	const char* Array = reinterpret_cast<const char*>(u8"Hello 里成里成里成 word!");

	utf8_iterator<const char*> My2ndIterator(Array, Array + sizeof(u8"Hello 里成里成里成 word!"));

	for (utf_char8 Char : My2ndIterator)
	{
		std::cout << "ArrayBytes = " << +Char[0] << ", " << +Char[1] << ", " << +Char[2] << ", " << +Char[3] << "\n";
	}
	std::cout << "\n" << std::endl;


	if (UtfN::ParseUtf8CharFromStr(u8"é") == UtfN::utf8_bytes{ 0xC3, 0xA9, 0x00, 0x00, })
	{
		std::cout << "yay\n";
	}

	utf_char16 C = utf16_pair{ 2000, 0000 };
	//utf_char8 C2 = C;


	std::wstring FunStr = L"αβ💡⚡🔍📝𝇎𝈋";
	utf16_iterator<std::wstring::const_iterator> MyIterator2(FunStr);

	for (utf_char16 Char : MyIterator2)
	{
		std::cout << "StrBytes: lower: " << +Char.Char.Lower << ",  upper: " << +Char.Char.Upper << std::endl;
	}
	std::cout << "\n" << std::endl;



	constexpr utf_char16 Pair = Utf32ToUtf16Pair(0x1D11E);

	//constexpr auto var1 = Pair.GetAsUtf16();
	//constexpr auto var2 = Pair.GetAsUtf8();
	//constexpr auto var3 = Pair.GetAsUtf32();
	//
	//(void)var1;
	//(void)var2;
	//(void)var3;

	std::cout << "Pair.Upper: " << std::hex << static_cast<unsigned short>(Pair.Char.Upper) << "\n";
	std::cout << "Pair.Lower: " << std::hex << static_cast<unsigned short>(Pair.Char.Lower) << "\n";

	utf_char32 OrignalChar = Utf16PairToUtf32(Pair);
	std::cout << "OrignalChar (Utf32 : 0x1D11E): " << std::hex << +OrignalChar.Char << "\n";


	utf_char8 Utf8Bytes1 = Utf32ToUtf8Bytes(0x41);
	std::cout << "1. Utf8Bytes = " << +Utf8Bytes1[0] << ", " << +Utf8Bytes1[1] << ", " << +Utf8Bytes1[2] << ", " << +Utf8Bytes1[3] << "\n";
	std::cout << "Orig (Utf32 : 0x41): " << +Utf8BytesToUtf32(Utf8Bytes1) << "\n\n";

	utf_char8 Utf8Bytes2 = Utf32ToUtf8Bytes(0xA9);
	std::cout << "2. Utf8Bytes = " << +Utf8Bytes2[0] << ", " << +Utf8Bytes2[1] << ", " << +Utf8Bytes2[2] << ", " << +Utf8Bytes2[3] << "\n";
	std::cout << "Orig (Utf32 : 0xA9): " << +Utf8BytesToUtf32(Utf8Bytes2) << "\n\n";

	utf_char8 Utf8Bytes3 = Utf32ToUtf8Bytes(0x20AC);
	std::cout << "3. Utf8Bytes = " << +Utf8Bytes3[0] << ", " << +Utf8Bytes3[1] << ", " << +Utf8Bytes3[2] << ", " << +Utf8Bytes3[3] << "\n";
	std::cout << "Orig (Utf32 : 0x20AC): " << +Utf8BytesToUtf32(Utf8Bytes3) << "\n\n";

	utf_char8 Utf8Bytes4 = Utf32ToUtf8Bytes(0x1F600);
	std::cout << "4. Utf8Bytes = " << +Utf8Bytes4[0] << ", " << +Utf8Bytes4[1] << ", " << +Utf8Bytes4[2] << ", " << +Utf8Bytes4[3] << "\n";
	std::cout << "Orig (Utf32 : 0x1F600): " << +Utf8BytesToUtf32(Utf8Bytes4) << "\n\n";

	utf_char8 Utf8Bytes5 = Utf32ToUtf8Bytes(0x110000);
	std::cout << "E. Utf8Bytes = " << +Utf8Bytes5[0] << ", " << +Utf8Bytes5[1] << ", " << +Utf8Bytes5[2] << ", " << +Utf8Bytes5[3] << "\n";
	std::cout << "Orig (Utf32 : 0x110000): " << +Utf8BytesToUtf32(Utf8Bytes5) << "\n\n";

}