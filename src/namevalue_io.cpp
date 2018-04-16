#include "namevalue_io.hpp"
#include "utf.hpp"
#include "reader.hpp"
#include "stringutil.hpp"

#include <sstream>
#include <istream>
#include <iostream>


namespace assembly::namevalue::io
{
    using namespace assembly::common::io;

    int add_to_store(name_value_store& store, const std::string& key, const std::string& type, const std::string& val)
    {
        try
        {
            int t = std::stoi(type);
            any_value res(static_cast<value_type>(t));

            if (t == 2 || t == 4 || t == 6 || t > 9 && t < 13 || t > 13)
            {
                std::cout << "Invalid Argument: '" << key << "'='" << type << "':'" << val << "'" << std::endl;
                return -1;
            }

            switch (t)
            {
                case 1:
                res.v_int32 = std::stoi(val); break;
                case 3:
                res.v_float = std::stof(val); break;
                case 5:
                res.v_uint32 = std::stoul(val); break;
                case 7:
                res.v_bool = std::stoi(val) == 1; break;
                case 8:
                case 9:
                res.v_int64 = std::stoll(val); break;
                case 13:
                res.v_blob = std::vector<char>(val.begin(), val.end()); break;
                default:
                res.v_string = val;
            }

            store.insert(std::make_pair(key, res));
        }
        catch (std::invalid_argument ex)
        {
            std::cout << "Invalid Argument: '" << key << "'='" << type << "':'" << val << "'" << std::endl;
        }
    }

    //! Read a store from a stringstream
    int read_from_stream(std::istream& i_strm, name_value_store& store, char delim)
    {
        std::string s_line;
        while (std::getline(i_strm, s_line, delim))
        {
            std::stringstream str(s_line);
            std::string key;
            std::string type;
            std::string val;

            std::getline(str, key, '=');
            std::getline(str, type, ':');
            std::getline(str, val);

            add_to_store(store, key, type, val);
        }
    }

    int read_store_entry(std::istream& i_strm, name_value_store& store)
    {
        std::u16string key = get_varstring<uint8_t, char16_t>(i_strm);
        std::u16string dat = get_varstring<uint8_t, char16_t>(i_strm);

        std::string dat_s = utf::to_utf8(dat);
        std::string key_s = utf::to_utf8(key);

        std::string type, val;
        if (contains(dat_s, ":"))
        {
            std::stringstream str(dat_s);
            std::getline(str, type, ':');
            std::getline(str, val);
        }
        else
        {
            type = "0";
            val = dat_s;
            //std::cout << key_s << ": " << dat_s << std::endl;
        }

        add_to_store(store, key_s, type, val);
    }

    int read_from_pair_stream(std::istream& i_strm, name_value_store& store)
    {
        uint32_t count = get_uint32(i_strm);
        for (uint32_t i = 0; i < count; i++) read_store_entry(i_strm, store);
    }

    //! Read a store from a string
    int read_from_string(const std::string& str, name_value_store& store, char delim)
    {
        std::istringstream i_strm(str);
        return read_from_stream(i_strm, store, delim);
    }

    //! Read a store from a utff16 string
    int read_from_u16string(const std::u16string& str, name_value_store& store, char delim)
    {
        std::string data = utf::to_utf8(str);
        return read_from_string(data, store, delim);
    }
}
