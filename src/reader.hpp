#pragma once

/*! \file reader.hpp
 *  \brief Utilities for reading files
 *
 *  Utility collection used in reading binary formats
 */

#include "types.hpp"
#include <istream>
#include <vector>
#include <functional>


//! Standard IO functions for streams
namespace assembly::common::io
{
    /* Reader functions */
    inline int check_eof(std::istream& strm)
    {
        if (strm.eof()) throw std::runtime_error("EOF reached");
    }

    int read_uint8(std::istream& strm, uint8_t& data);
    int read_uint32(std::istream& strm, uint32_t& data);
    int read_uint64(std::istream& strm, uint64_t& data);

    template <typename type>
    inline int read(std::istream& strm, type& data)
    {
        strm.read((char*) &data, sizeof(type));
        check_eof(strm);
    }

    template <typename type>
    inline type get(std::istream& strm)
    {
        type val;
        read<type>(strm, val);
        return val;
    }

    template <typename T>
    inline bool get_bool(std::istream& strm)
    {
        return get<T>(strm) != 0;
    }

    template <typename T>
    inline int read_bool(std::istream& strm, bool& data)
    {
        data = get_bool<T>(strm);
    }

    template <typename enum_type, typename size_type>
    inline int read_enum(std::istream& strm, enum_type& data)
    {
        data = static_cast<enum_type>(get<size_type>(strm));
    }

    template <typename enum_type, typename size_type>
    inline enum_type get_enum(std::istream& strm)
    {
        enum_type val;
        read_enum<enum_type, size_type>(strm, val);
        return val;
    }

    int read_int32(std::istream& strm, int32_t& data);
    int read_int64(std::istream& strm, int64_t& data);
    int read_float(std::istream& strm, float& f);

    /* Getter functions */
    uint8_t get_uint8(std::istream& strm);
    uint32_t get_uint32(std::istream& strm);
    float get_float(std::istream& strm);

    //int read_string(std::istream& strm, std::string& str, uint32_t n);
    int read_u16string(std::istream& strm, std::u16string& str, uint32_t n);

    int read_lu8_string(std::istream& strm, std::string& str);
    int read_lu32_string(std::istream& strm, std::string& str);
    int read_lu8_u16string(std::istream& strm, std::u16string& str);
    int read_lu32_u16string(std::istream& strm, std::u16string& str);

    template <typename char_type>
    inline int read_string(std::istream& strm, std::basic_string<char_type>& str, uint32_t n)
    {
        str.resize(n);
        strm.read((char*) &str[0], n * sizeof(char_type));
        check_eof(strm);
    }

    template <typename char_type>
    inline std::basic_string<char_type> get_string(std::istream& strm, uint32_t n)
    {
        std::basic_string<char_type> str;
        read_string<char_type>(strm, str, n);
        return str;
    }

    template <typename size_type, typename char_type>
    inline int read_varstring(std::istream& strm, std::basic_string<char_type>& str)
    {
        size_type count = get<size_type>(strm);
        read_string<char_type>(strm, str, count);
    }

    template <typename size_type, typename char_type>
    inline std::basic_string<char_type> get_varstring(std::istream& strm)
    {
        std::basic_string<char_type> str;
        read_varstring<size_type, char_type>(strm, str);
        return str;
    }

    int read_buffer(std::istream& strm, char* data, uint32_t n);

    template<typename T, int(*reader)(std::istream&, std::vector<T>&)>
    inline int read_vector_lu32(std::istream& strm, std::vector<T>& data)
    {
        uint32_t count = get_uint32(strm);
        for (uint32_t i = 0; i < count; i++)
        {
            reader(strm, data);
        }
    }

    template<typename type, typename size_type, int(*reader)(std::istream&, type&)>
    inline int read_vector(std::istream& strm, std::vector<type>& vec)
    {
        size_type count = get<size_type>(strm);
        for (size_type i = 0; i < count; i++)
        {
            type val;
            reader(strm, val);
            vec.push_back(val);
        }
    }

    template<typename type, typename size_type, typename extra_type, int(*reader)(std::istream&, type&, extra_type extra)>
    inline int read_vector(std::istream& strm, std::vector<type>& vec, extra_type extra)
    {
        size_type count = get<size_type>(strm);
        for (size_type i = 0; i < count; i++)
        {
            type val;
            reader(strm, val, extra);
            vec.push_back(val);
        }
    }

    //std::string get_string(std::istream& strm, uint32_t n);
    std::u16string get_lu8_u16string(std::istream& strm);

    /* Read Structures functions */
    int read_position(std::istream& strm, position& pos);
    int read_position_wxyz(std::istream& strm, position& pos);
    int read_vector3f(std::istream& strm, vector3f& vec);
    int read_quaternion(std::istream& strm, quaternion& quat);
    int read_quaternion_wxyz(std::istream& strm, quaternion& quat);

    /* Writer functions */
    int write_uint8(std::ostream& strm, uint8_t data);
    int write_uint16(std::ostream& strm, uint16_t data);
    int write_uint32(std::ostream& strm, uint32_t data);
    int write_uint64(std::ostream& strm, uint64_t data);

    int write_int8(std::ostream& strm, int8_t data);
    int write_int16(std::ostream& strm, int16_t data);
    int write_int32(std::ostream& strm, int32_t data);
    int write_int64(std::ostream& strm, int64_t data);

    int write_buffer(std::ostream& strm, const char* data, uint32_t n);

    template<typename T, int(*writer)(std::ostream&,const T&)>
    inline int write_vector_lu32(std::ostream& strm, std::vector<T> data)
    {
        write_uint32(strm, data.size());
        for (const T& val : data)
        {
            writer(strm, val);
        }
    }
}
