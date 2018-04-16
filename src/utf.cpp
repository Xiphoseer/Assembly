#include "utf.hpp"
#include <locale>
#include <codecvt>

namespace utf
{
    std::u16string from_utf8(const std::string& utf8string)
    {
        //std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
        //return converterX.from_bytes(utf8string);

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
        return conv.from_bytes(utf8string);
    }

    std::string to_utf8(const std::u16string& utf16string)
    {
        //std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        //return converter.to_bytes(widestring);

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
        return conv.to_bytes(utf16string);
    }
}
