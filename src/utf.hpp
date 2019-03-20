#pragma once

/*! \file utf.hpp
 *  \brief Unicode conversion
 *
 *  Utilities related to text encoding and unicode
 */

#include <string>

//! Unicode conversion tools
namespace utf
{
    //! Convert a utf-8 string to utf-16
    std::u16string from_utf8(const std::string& utf8string);
    //! Convert a utf-16 string to utf-8
    std::string to_utf8(const std::u16string& utf16string);

    //! Convert a codepage to a utf-8 string

    struct iconv_to_utf8
    {
      void* data;

      std::string operator()(const std::string& src) const;
      iconv_to_utf8(const std::string& codepage);
      ~iconv_to_utf8();
    };
}
