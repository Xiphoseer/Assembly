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

    //! Convert between codepages
    struct iconv_op_t
    {
      //! Internal data
      void* data;

      //! Actual conversion function
      std::string operator()(const std::string& src) const;

      //! Constructor from two codepages
      iconv_op_t(const std::string& toCodepage, const std::string& fromCodepage);

      //! Destructor
      ~iconv_op_t();
    };

    //! Convert a codepage to a utf-8 string
    struct iconv_to_utf8 : iconv_op_t
    {
      //! Constructor with a source codepage
      iconv_to_utf8(const std::string& fromCodepage);
    };

    //! Convert a utf-8 string to some codepage
    struct iconv_from_utf8 : iconv_op_t
    {
      //! Constructor with a target codepage
      iconv_from_utf8(const std::string& toCodepage);
    };
}
