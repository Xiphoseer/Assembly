#include "utf.hpp"
#include <locale>
#include <sstream>
#include <codecvt>
#include <iconv.h>

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

    iconv_to_utf8::iconv_to_utf8(const std::string& codepage)
    {
      this->data = malloc(sizeof(iconv_t));
      iconv_t* p_cd = (iconv_t*) this->data;
      *p_cd = iconv_open(codepage.c_str(), "UTF-8");
    }

    iconv_to_utf8::~iconv_to_utf8()
    {
      free(this->data);
    }

    std::string iconv_to_utf8::operator()(const std::string& src)
    {
      iconv_t conv = *((iconv_t*) this->data);
      const char* in = src.c_str();
      size_t src_remain = src.length();

      char buf[128];
      char* out = buf;
      size_t out_remain = 128;

      std::stringstream res("");

      while (src_remain > 0)
      {
        size_t r = iconv(conv, (char**) &in, &src_remain, &out, &out_remain);

        if (r == -1) {
          throw std::runtime_error("Conversion error");
        }
        res << std::string(buf, 128 - out_remain);
        out_remain = 128;
        out = buf;
      }

      return res.str();
    }
}
