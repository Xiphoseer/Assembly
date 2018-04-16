#include "namevalue.hpp"

namespace assembly::namevalue
{
    any_value::any_value(const value_type& type) : type(type)
    {
        switch (type)
        {
            case value_type::string_v: new(&v_string) std::string(); break;
            case value_type::binary_v: new(&v_blob) std::vector<char>(); break;
        }
    }

    any_value::any_value(const any_value& val) : type(val.type)
    {
        switch (type)
        {
            case value_type::string_v: new(&v_string) std::string(val.v_string); break;
            case value_type::int32_v: v_int32 = val.v_int32; break;
            case value_type::float_v: v_float = val.v_float; break;
            case value_type::uint32_v: v_uint32 = val.v_uint32; break;
            case value_type::bool_v: v_bool = val.v_bool; break;
            case value_type::int64_v:
            case value_type::id64_v: v_int64 = val.v_int64; break;
            case value_type::binary_v: new(&v_blob) std::vector<char>(val.v_blob); break;
        }
    }

    any_value::any_value(any_value&& val) : type(val.type)
    {
        switch (type)
        {
            case value_type::string_v: new(&v_string) std::string(val.v_string); break;
            case value_type::int32_v: v_int32 = val.v_int32; break;
            case value_type::float_v: v_float = val.v_float; break;
            case value_type::uint32_v: v_uint32 = val.v_uint32; break;
            case value_type::bool_v: v_bool = val.v_bool; break;
            case value_type::int64_v:
            case value_type::id64_v: v_int64 = val.v_int64; break;
            case value_type::binary_v: new(&v_blob) std::vector<char>(val.v_blob); break;
        }
    }

    any_value::~any_value()
    {
        switch (type)
        {
            case value_type::string_v: v_string.~basic_string(); break;
            case value_type::binary_v: v_blob.~vector(); break;
        }
    }
}

